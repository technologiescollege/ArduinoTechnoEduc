/***************************************************************************************
 *
 * Title:       Bluetooth library v1.0
 * File:        Bluetooth.cpp
 * Date:        2013-08-22
 * Author:      Karl Kangur
 *
 ***************************************************************************************/

#include <Bluetooth.h>

// the actual bluetooth instance
BT Bluetooth;

BT::BT(void)
{
  // verbose mode returns all sent, received and error data back to serial (usb)
  this->verbose = 0;
}

void BT::debug(const char *message)
{
  if(this->verbose)
  {
    Serial.println(message);
  }
}

void BT::begin(uint16_t baud)
{
  if(baud > 57600)
  {
    // higher baud rates do not work for some reason
    this->debug("Maximum working baud rate is 57600");
    return;
  }
  
  // set AT mode pin to output
  pinMode(4, OUTPUT);
  
  if(!baud)
  {
    this->findBaud();
  }
  else
  {
    btSerial.begin(baud);
  }
  
  // set slave mode by default
  this->setMode(SLAVE);
}

int BT::available(void)
{
  return btSerial.available();
}

int BT::read(void)
{
  return btSerial.read();
}

int BT::peek(void)
{
  return btSerial.peek();
}

void BT::flush(void)
{
  btSerial.flush();
}

size_t BT::write(uint8_t byte)
{
  btSerial.write(byte);
  // othwerise it throws a warning
  return 1;
}

uint16_t BT::findBaud(void)
{
  // the MCU has trouble with baud rates higher than 57600
  uint16_t baudRates[] = {9600, 38400, 57600, 19200, 4800};
  for(uint8_t i = 0; i < 5; i++)
  {
    btSerial.begin(baudRates[i]);
    // clear the last try "AT" at wrong baud rate or it returns an error message
    // sending "\r\n" for the first value will make it ignore it for some reason
    if(i != 0)
    {
      btSerial.write("\r\n");
    }
    // try the "AT" command
    if(this->command("AT", "OK"))
    {
      return baudRates[i];
    }
  }
  btSerial.end();
  return 0;
}

bool BT::setBaud(const char *baud)
{
  // transform char array to long int and compare with maximum baud rate
  if(atol(baud) > 57600)
  {
    // higher baud rates do not work for some reason
    this->debug("Maximum working baud rate is 57600");
    return false;
  }
  
  this->clearBuffer();
  strcpy(this->buffer, "AT+UART=");
  strcat(this->buffer, baud);
  // set partity to "no" and stop bits to 1
  strcat(this->buffer, ",0,0");
  if(!this->command(this->buffer, "OK"))
  {
    return false;
  }
  this->debug("New baud rate set, restart module to take effect");
  return true;
}

// sets local buffer to 0
void BT::clearBuffer(void)
{
  memset(this->buffer, 0, sizeof(this->buffer));
}

bool BT::connect(const char *address)
{
  if(this->mode == SLAVE)
  {
    this->debug("Master mode not enabled");
    return false;
  }
  
  // initialize Serial Port Profile (SPP)
  this->command("AT+INIT", "OK");
  // this sometimes returns an error as SPP can only be initialized once
  
  // pair slave device if needed
  this->clearBuffer();
  strcpy(this->buffer, "AT+PAIR=");
  strcat(this->buffer, address);
  // set connecting time to 5 seconds
  strcat(this->buffer, ",5");
  if(!this->command(this->buffer, "OK"))
  {
    return false;
  }
  
  // link to slave device
  this->clearBuffer();
  strcpy(this->buffer, "AT+LINK=");
  strcat(this->buffer, address);
  strcat(this->buffer, "\r\n");
  // this command will not return a value, so call it directly here
  btSerial.write(this->buffer);
  
  // wait for the connection to establish, new commands shouldn't be sent at this time
  delay(5000);
  return true;
}

bool BT::disconnect(void)
{
  return this->command("AT+DISC", "SUCCESS");
}

bool BT::setName(const char *name)
{
  this->clearBuffer();
  strcpy(this->buffer, "AT+NAME=");
  strcat(this->buffer, name);
  return this->command(this->buffer, "OK");
}

bool BT::setPass(const char *pass)
{
  this->clearBuffer();
  strcpy(this->buffer, "AT+PSWD=");
  strcat(this->buffer, pass);
  return this->command(this->buffer, "OK");
}

bool BT::setMode(unsigned char mode)
{
  if((mode == MASTER && this->command("AT+ROLE=1", "OK")) || (mode == SLAVE && this->command("AT+ROLE=0", "OK")))
  {
    this->mode = mode;
    return true;
  }
  return false;
}

bool BT::search(void)
{
  if(this->mode == SLAVE)
  {
    this->debug("Master mode not enabled");
    return false;
  }
  
  // wait 3 seconds while searching for devices
  btSerial.setTimeout(3000);

  // search (inquire) devices, may overflow the buffer if there are too many devices
  bool search = this->command("AT+INQ", "OK");
  
  btSerial.setTimeout(1000);
  return search;
}

bool BT::command(const char *cmd, const char *expReturn)
{
  bool returnValue = false;
  // enter AT mode
  digitalWrite(4, HIGH);
  
  // wait for the bluetooth module to enable AT mode, 100ms seems to be enough
  delay(100);
  
  this->debug(cmd);
  // actually send the command
  btSerial.write(cmd);
  // send command end
  btSerial.write("\r\n");
  
  // clear buffer for reply
  this->clearBuffer();
  
  // wait for the bluetooth module to reply
  uint32_t timeout = millis();
  while(!btSerial.available())
  {
    if(millis() - timeout > 3000)
    {
      this->debug("No response timeout");
      break;
    }
  }
  btSerial.readBytes(this->buffer, 128);
  
  // show returned data
  this->debug(this->buffer);

  // check for expected return
  if(expReturn != NULL && strstr(this->buffer, expReturn))
  {
    returnValue = true;
  }
  
  // exit AT mode
  digitalWrite(4, LOW);
  
  // give some time to reliably exit AT mode
  delay(150);
  
  return returnValue;
}
