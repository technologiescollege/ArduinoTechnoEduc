#include "MoleGraph.h"

#define VERSION "ATG_4" //arduino to graph version
#define MESSAGE_SIZE 1 + sizeof(float)

namespace
{
  enum Instructions
  {
    INS_NONE = 0, //data
    INS_GET_VERSION = 1,
    INS_SET_TIME = 2,
    INS_SET_FREQUENCY = 3,
    INS_ENABLED_CHANNELS = 4,
    INS_START = 5,
    INS_STOP = 6,
    INS_SET_TYPE = 7,
    INS_GET_SAMLPE = 8,
    INS_PAUSE = 9,
    INS_CONTINUE = 10,
    INS_INITIALIZE = 11
  };
  
  enum Type
  {
      typePeriodical = 0,
      typeOnDemand = 1
  } g_type = typePeriodical;
  
  unsigned char g_enabledChannels = 0;
  unsigned char g_channelCount = 0;
  unsigned g_requiredTime = 0;
  unsigned g_currentTime = 0;
  float g_channels[8];
  bool g_fullWriteBufferDetected = false;
  float g_timeFromStart = 0;
  bool g_sampleRequest = false;
  bool g_measurementInProgress = false;
  void (*g_updateFunction)(void);
  void (*g_measurementStartedCallback)(void);
  void (*g_measurementStoppedCallback)(void);
  void (*g_measurementPausedCallback)(void);
  void (*g_measurementContinuedCallback)(void);
  
  void InitTimer()
  {
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
  
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    interrupts();             // enable all interrupts
  }

  unsigned char GetCheckSum(unsigned char input)
  {
    unsigned char output = 0;
    for (unsigned char i = 0; i < 8; ++i)
    {
      if (input & (1 << i))
        output++;
    }
    return output;
  }
  
  /*returns true bits count*/
  unsigned WriteHeader(unsigned char commandId)
  {
    unsigned char mixture = commandId;
    mixture |= g_fullWriteBufferDetected << 7;
    
    Serial.write(mixture);
    return GetCheckSum(mixture);  
  }
  
  void SendData(bool timestamp)
  {
    //it can happen when user set to high frequency or too many channels
    //number of data is then higher then baud rate.
    // I try to check Serial.availableForWrite() < g_channelCount * MESSAGE_SIZE but it happend always
    // the buffer is probably less then 40 and => I could 
    
    bool bufferIsFull = (Serial.availableForWrite() < g_channelCount * MESSAGE_SIZE + (timestamp ? sizeof(float) : 0)); 
  
    g_fullWriteBufferDetected |= bufferIsFull;
    
    //RX LED satay to light but application was not able to read all the data 
    //when I try to send data when I try to write data in this moment.
    //When a new messurment was started, data from previous mesurment was delivered WTF?
    
    if (bufferIsFull)
      return; //have to throw data form this sample :(

    unsigned char checkSum = WriteHeader(0 /*INS_NONE*/);
    
    if (timestamp)
    {
      Serial.write((char *)&g_timeFromStart, sizeof(float));
      checkSum += GetCheckSum(g_timeFromStart);
    }
    for (int i = 0; i < 8; i++)
    {
      if (0 != ((g_enabledChannels >> i) & 1)) 
      {
        Serial.write((char *)&g_channels[i], sizeof(float));
        for (int j = 0; j < sizeof(float); j++)
          checkSum += GetCheckSum(((char *)&g_channels[i])[j]);
      }
    }
     
    Serial.write(checkSum);
      
    g_fullWriteBufferDetected = false;
  }

  void FillFromSerial(unsigned char &value)
  {
    while (0 == Serial.available())
    {}
    value = Serial.read();
  }

  void FillFromSerial(unsigned &value)
  {
    while (0 == Serial.available())
    {}
    value = Serial.read();
    while (0 == Serial.available())
    {}
    value |= Serial.read() << 8;
  }

  void Start()
  {
    g_fullWriteBufferDetected = false;
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
    g_measurementInProgress = true;
  }

  void Stop()
  {
    TIMSK1 &= ~(1 << OCIE1A);  // disable timer compare interrupt
    g_measurementInProgress = false;
  }
} //namespace


ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  //CheckInput();
  if (g_type == typeOnDemand)
  {
    g_timeFromStart += 1.0/62500.0;
    if (!g_sampleRequest)  
      return;

     g_sampleRequest = false;
  }
  else //type periodical
  {
    if (0 != g_requiredTime && (++g_currentTime) != g_requiredTime)
      return;
    g_currentTime = 0;
  }
    
  g_updateFunction();  
  SendData(g_type == typeOnDemand);
}

void MoleGraph::CheckInput()
  {
    if (0 == Serial.available())
      return;
    
    unsigned char instruction = Serial.read();
    switch (instruction) 
    {
    case INS_GET_VERSION:
      Serial.write(VERSION);
    break;
    case INS_SET_FREQUENCY:
    {
      unsigned frequency;
      FillFromSerial(frequency);
      
      InitTimer(); //workaround there was a 1s lag after start when there had been set 1 Hz period and user set 200 Hz period   
      OCR1A = 62500/frequency;            // compare match register 16MHz/256/2Hz
    }
    break;
    case INS_SET_TIME:
    {
      FillFromSerial(g_requiredTime);
  
      InitTimer(); //workaround as in set frequenc case
      OCR1A = 62500;            // compare match register 16MHz/256/2Hz
    }
    break;
    case INS_ENABLED_CHANNELS:
      FillFromSerial(g_enabledChannels);
      g_channelCount = 0;
      for (int i = 0; i < 8; i++)
        if (0 != (g_enabledChannels & (1 << i)))
          g_channelCount++;       
    break;
    case INS_START:
      Start();
      g_measurementStartedCallback();
    break;
    case INS_STOP:
      Stop();
      g_measurementStoppedCallback();
    break;
    case INS_CONTINUE:
      Start();
      g_measurementContinuedCallback();
    break;
    case INS_PAUSE:
      Stop();
      g_measurementPausedCallback();
    break;
    case INS_SET_TYPE:
      unsigned char type;
      FillFromSerial(type);
      g_type = (Type)type;
      if (g_type == typeOnDemand)
      {
        g_sampleRequest = false;
        g_timeFromStart = 0;
        InitTimer();
        OCR1A = 1; //1/62500 s
      }
      else
        g_currentTime = g_requiredTime - 1; //to be data send immediately
    break;
    case INS_GET_SAMLPE:
      g_sampleRequest = true;
    break;    
    case INS_INITIALIZE: //when desktop application crash, this stay in sending data
      Stop();
    break;
    }
  }
  
void MoleGraph::Setup()
{ 
  Serial.begin(115200);
  while (!Serial) 
  {}
  
  InitTimer();
}

void MoleGraph::SetSendingCallback(void (*function)(void))
{
  g_updateFunction = function;
}

void MoleGraph::SetMeasurementStartedCallback(void (*function)(void))
{
  g_measurementStartedCallback = function;
}

void MoleGraph::SetMeasurementStoppedCallback(void (*function)(void))
{
  g_measurementStoppedCallback = function;
}

void MoleGraph::SetMeasurementPausedCallback(void (*function)(void))
{
  g_measurementPausedCallback = function;
}

void MoleGraph::SetMeasurementContinuedCallback(void (*function)(void))
{
  g_measurementContinuedCallback = function;
}

bool MoleGraph::SetChannelValue(int channel, float value)
{
  if (channel < 1 && channel > 8)
    return false;
    
  g_channels[channel - 1] = value; //uses count channels from 1 
  
  return true;
}

float MoleGraph::GetChannelValue(int channel)
{
  if (channel < 1 && channel > 8)
    return 0;
    
  return g_channels[channel - 1];
}

void MoleGraph::SampleRequest()
{
  g_sampleRequest = true;
}

void SendCommand(unsigned char commandId)
{
  unsigned char checkSum = WriteHeader(commandId);
  Serial.write(checkSum);
}

void MoleGraph::StartMeasurement()
{
  SendCommand(INS_START);
}

void MoleGraph::StopMeasurement()
{
  SendCommand(INS_STOP);
}

bool MoleGraph::IsMeasurementInProgress()
{
  return g_measurementInProgress;   
}
