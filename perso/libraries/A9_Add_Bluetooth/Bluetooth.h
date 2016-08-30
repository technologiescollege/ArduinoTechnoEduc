/***************************************************************************************
 *
 * Title:       Bluetooth library v1.0
 * File:        Bluetooth.h
 * Date:        2013-08-22
 * Author:      Karl Kangur
 *
 ***************************************************************************************/
 
#ifndef blueooth_h
#define blueooth_h

#include <Arduino.h>

// serial port to use for the Bluetooth module
#define btSerial Serial1

// definitions
#define MASTER 1
#define SLAVE 0

// Bluetooth class
class BT : public Stream
{
  private:
  // the buffer must be that big mainly because of the search method
  char buffer[128];
  unsigned char mode;
  
  void debug(const char *message);
  void clearBuffer(void);
  
  public:
  uint8_t verbose;
  
  BT(void);
  
  void begin(uint16_t baud = NULL);
  
  bool connect(const char *address);
  bool disconnect(void);
  uint16_t findBaud(void);
  
  bool setName(const char *name);
  bool setPass(const char *pass);
  bool setMode(unsigned char mode);
  bool command(const char *cmd, const char *expReturn = "");
  bool search(void);
  bool setBaud(const char *baud);
  
  // virtual functions from Stream class
  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual void flush(void);
  virtual size_t write(uint8_t);
  using Print::write;
};

// the instance is declared in Bluetooth.cpp
//extern BT Bluetooth;
#endif
