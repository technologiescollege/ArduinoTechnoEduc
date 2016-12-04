/************************************************************************
 *  
 *  NAME:  mowayduinorf.cpp
 *
 *  DESCRIPTION: RF library for mOwayduino robot.
 *
 *  DATE: 12/11/2013    Rev 1.0    Minirobots, S.L.
 *
 ************************************************************************/

//*******************************************************
//    Libraries
//*******************************************************
#include <SPI.h>
#include "MowayduinoRf.h"
#include "MowayduinoIo.h"

#define ENABLE 1
#define DISABLE 0

// Function for RF interruption.
// It must be defined with this name.
extern void RfDataReady();

//*******************************************
//    Constructor
//*******************************************
Rf::Rf()
{
  flgInt = false;
}


//***************************************************************
//  NAME: beginRF
//  DESCRIPTION: Configures the RF module.
//  PARAMETERS: chan (0-255) -> RF transmission channel
//		add (0-255)  -> RF module address 
//  RETURNS: "1" if correct, "0" if not.
//***************************************************************
unsigned char Rf::beginRF(unsigned char chan, unsigned char add)
{
  unsigned char configTest;
  
  // Disable RF interrupt
  if(flgInt)
  {
    InterruptsRF(0);
    flgInt = true;
  }

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  delay(10);

  // Configure SPI ports
  pinMode(RF_CE,OUTPUT);
  pinMode(RF_CS,OUTPUT);
  pinMode(RF_IRQ,INPUT);

  // Initialize SPI ports
  digitalWrite(RF_CE,LOW);      // chip enable
  digitalWrite(RF_CS,HIGH);    // Spi disable	

  // Chip enable
  digitalWrite(RF_CE,LOW);

  // RX_ADDR_P0 (0xFF) ACK
  // 5 bytes (0xC2)
  digitalWrite(RF_CS,LOW);            // CSN low, init SPI transaction
  SPI.transfer(0x2A);
  SPI.transfer(0xC2);	
  digitalWrite(RF_CS,HIGH);           // CSN high
  delayMicroseconds(1); 

  // RX_ADDR_P1 (add)
  // Configuration of Pipe 1 to receive data
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x2B);
  SPI.transfer(add);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // EN_AA
  // Enable AutoAck in Pipe 0,Pipe 1 and Pipe 2
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x21);
  SPI.transfer(0x07);  
  digitalWrite(RF_CS,HIGH); 
  delayMicroseconds(1);

  // EN_RXADDR
  // Enable Pipe 0 and Pipe 1
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x22);
  SPI.transfer(0x03);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // SETUP_AW
  // Configuration of address lenght.
  // Address lenght: 5 bytes.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x23);
  SPI.transfer(0x03);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);    
	
	//SETUP_RETR
  // Configuration of retransmission.
  // 10 retransmission each 336us.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x24);
  SPI.transfer(0x0A);
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RF_SETUP
  // Configuration of RF module features.
  // LNA maximun gain, 0dBm output power y 2Mbps transmission air speed.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x26);
  SPI.transfer(0x0F);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // STATUS
  // Status register clear
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x27);
  SPI.transfer(0x70);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RX_PW_P0
  // Number of bytes in Pipe 0
  // 1 byte (ACK).
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x31);
  SPI.transfer(0x01);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RX_PW_P1
  // Number of bytes in Pipe 1
  // 10 bytes (number of the data, address of the transsmisor and data).
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x32);
  SPI.transfer(0x0A);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RF_CH
  // RF channel
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x25);
  SPI.transfer(chan);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RF_CH
  // Check if the RF channel is correcty initilized.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x05);
  configTest = SPI.transfer(0);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  if(configTest != chan)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);    
    return(0);
  }
  else
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);    
    return(1);
  }
}


//***************************************************************
//  NAME: On
//  DESCRIPTION: Turns on the RF module.
//  RETURNS: "1" if correct, "0" if not.
//***************************************************************
unsigned char Rf::On()
{
  unsigned char        CONFIG_TEST;

  // Disable RF interrupt
  if(flgInt)
  {
    InterruptsRF(0);
    flgInt = true;
  }
  
  // Chip enable            
  digitalWrite(RF_CE,LOW);

  // CONFIG
  // Activate RF module and put it into reception mode.
  // The CRC is activated to use 2 bytes.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x20);
  SPI.transfer(0x0F);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // CONFIG
  // Check if the module is configured correctly.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x00);
  CONFIG_TEST = SPI.transfer(0);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  delay(2);                    //Delay of 2ms
  digitalWrite(RF_CE, HIGH);
  delayMicroseconds(150);

  if(CONFIG_TEST != 0x0F)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);
    return(0);
  }
  else
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);
    return(1);
  }
}

//***************************************************************
//  NAME: Off
//  DESCRIPTION: Turns off the RF module.
//  RETURNS: "1" if correct, "0" if not.
//***************************************************************
unsigned char Rf::Off()
{
  unsigned char        CONFIG_TEST;

  // Disable RF interrupt
  if(flgInt)
  {
    InterruptsRF(0);
    flgInt = true;
  }

  // Chip enable            
  digitalWrite(RF_CE,LOW);

  // CONFIG
  // Deactivate RF module
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x20);
  SPI.transfer(0x0C);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // CONFIG
  // Check if the module is correctly deactivated.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x00);
  CONFIG_TEST = SPI.transfer(0);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  if(CONFIG_TEST != 0x0C)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);
    return(0);
  }
  else
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);
    return(1);
  }
}

//***************************************************************
//  NAME: Send
//  DESCRIPTION: Sends RF message.
//  PARAMETERS: RF_DIR_OUT (0-255)  -> Address of the receiver
//      	RF_DATA_OUT  	    -> 8 bytes of data to be sent 
//  RETURNS: 	"0" if correctly sent
//		"1" if ACK not received
//***************************************************************
unsigned char Rf::Send(unsigned char RF_DIR_OUT, unsigned char RF_DATA_OUT[])
{
  unsigned char i;
  unsigned char estado;
  unsigned char RF_PAY_OUT[8];
  unsigned char RF_PAY_DIR;
  int NO_RF;  
  
  // Disable RF interrupt
  if(flgInt)
  {
    InterruptsRF(0);
    flgInt = true;
  }

  // Start
  digitalWrite(RF_CE,LOW); 

  // STATUS
  // STATUS register clear
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x27);
  SPI.transfer(0x70);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // EN_RXADDR
  // Pipe 0 enable to receive the ACK
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x22);
  SPI.transfer(0x01);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // TX_ADDR
  // Receiver address configuration: RF_DIR_OUT (User defined)
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x30);	
  SPI.transfer(RF_DIR_OUT);
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RX_ADDR_P0
  // To receive the ACK, the address of the Pipe 0 has to be the equal to
  // transmission address.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x2A);
  SPI.transfer(RF_DIR_OUT);	
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	
  SPI.transfer(0xC2);	    
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // RX_ADDR_P1
  // The own address is packaged in the data string and this way
  // the receiver knows the address of the transmitter.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x0B);
  RF_PAY_DIR = SPI.transfer(0);	
  SPI.transfer(0);	
  SPI.transfer(0);	
  SPI.transfer(0);	
  SPI.transfer(0);	    
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // W_TX_PAYLOAD
  // The data is sent to the output payload
  digitalWrite(RF_CS,LOW); 
  SPI.transfer(0xA0);

  DATA_N_SND++;

  for(i=0;i<8;i++)
    RF_PAY_OUT[i]=RF_DATA_OUT[i];

  SPI.transfer(DATA_N_SND);
  SPI.transfer(RF_PAY_DIR);    

  for(i=0;i<8;i++)
    SPI.transfer(RF_PAY_OUT[i]);

  digitalWrite(RF_CS,HIGH); 

  // CONFIG
  // Transmission mode
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x20);
  SPI.transfer(0x0E);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // Start sending signal
  digitalWrite(RF_CE,HIGH);
  delayMicroseconds(12); 
  digitalWrite(RF_CS,LOW);

  NO_RF=0;

  while(digitalRead(RF_IRQ)) 
  {
    NO_RF++;
    // If no response in 7ms, it didn't send
    if(NO_RF==500)
      break; 
  }  

  // STATUS
  // Read the status register to check the status.
  digitalWrite(RF_CS,LOW);
  estado = SPI.transfer(0x27);
  SPI.transfer(0x70);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);
  
  // EN_RXADDR
  // Enable Pipe 0,Pipe 1 and Pipe 2.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x22);
  SPI.transfer(0x07);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // TX_FLUSH
  // Flush output payload
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0xE1);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // CONFIG
  // Reception mode
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x20);
  SPI.transfer(0x0F);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // End
  digitalWrite(RF_CE,HIGH);
  delayMicroseconds(150);
  
  // Clear INT0 flag
  EIFR |= 0x01;
  
  // If no response in 7ms, it didn't send
  if(NO_RF==500 || NO_RF==0)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);    
    return(2);
  }

  // STATUS
  // Check the status register to check if the data have been sent or not.
  if ((estado&(1<<4))==0 && (estado&(1<<5))!=0)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);      
    return(0);
  }
  else 
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);      
    return(1);
  }
}


//***************************************************************
//  NAME: Receive
//  DESCRIPTION: Waits for a RF message.
//  PARAMETERS: RF_DIR_IN (0-255) -> Address of the receiver
//		RF_DATA_IN        -> 8 bytes of data to receibe
//  RETURNS: 	"0" if one RF message has been received
//		"1" if there are more RF messages to read
//		"2" if not received
//***************************************************************
unsigned char Rf::Receive(unsigned char* RF_DIR_IN, unsigned char* RF_DATA_IN)
{
  unsigned char i;
  unsigned char mas;
  unsigned char estado;	

  // Disable RF interrupt
  if(flgInt)
  {
    InterruptsRF(0);
    flgInt = true;
  }
    
  // FIFO_STATUS
  // Check the input FIFO
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x17);
  mas = SPI.transfer(0);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // Check if RF message has been received
  if ((mas&1 != 0) && digitalRead(RF_IRQ) == 1)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);      
    return (2);
  }
  
  // STATUS
  // Read STATUS register
  digitalWrite(RF_CS,LOW);
  estado = SPI.transfer(0x07);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  if(digitalRead(RF_IRQ) == 0 && (estado&(1<<6) == 0))
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);
    return(2);	
  }
  
  // Clear INT0 flag
  EIFR |= 0x01;
  
  // R_RX_PAYLOAD
  // Read the received data.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x61);	
  DATA_N_RCV = SPI.transfer(0);
  *RF_DIR_IN = SPI.transfer(0);
  for (i=0;i<8;i++)
  {
    RF_DATA_IN[i]=SPI.transfer(0);      
  }
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // STATUS
  // Reset and read STATUS
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x27);
  SPI.transfer(0x70);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  // FIFO_STATUS
  // Check the reception FIFO to see if more data have been received during this function.
  digitalWrite(RF_CS,LOW);
  SPI.transfer(0x17);
  mas = SPI.transfer(0);	
  digitalWrite(RF_CS,HIGH);
  delayMicroseconds(1);

  if (mas&1==0)
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);  
    return(1);
  }
  else
  {
    // Enable RF interrupt
    if(flgInt)    
      InterruptsRF(1);  
    return(0);
  }  
}


//***************************************************************
//  NAME: InterruptsRF
//  DESCRIPTION: Enable(/Disable Interrupts on RF
//  PARAMETERS: ENABLE_DISABLE (0-1) ENABLE - 1 - DISABLE - 0 -
//***************************************************************
void Rf::InterruptsRF(byte ENABLE_DISABLE)
{
  if (ENABLE_DISABLE == ENABLE)
  {   
    flgInt = true;
    attachInterrupt(0, RfDataReady, FALLING);
  }
  else
  {   
    flgInt = false;
    detachInterrupt(0);    
  }
}




