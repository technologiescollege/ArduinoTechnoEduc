/* Virtuino Ethernet Shield web server library
 * Created by Ilias Lamprou
 * Updated Jul/01/2016
 * 
 * Download last Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.javapapers.android.agrofarmlitetrial
 * Getting starting link:
 * Video tutorial link: 
 * Contact address for questions or comments: iliaslampr@gmail.com
 */



//========= Virtuino General methods  

//  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
//  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
//  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
//  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
//  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
//  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
 




#define VirtuinoEthernet_WebServer_h

#include "Arduino.h"
#include <Ethernet.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define arduinoAnalogPinsSize 16  
      #define analogInputPinsMap_ { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 }
    
#else
      #define arduinoAnalogPinsSize 6  
      #define analogInputPinsMap_ { A0, A1, A2, A3, A4, A5 }
     
#endif
  const float firmwareCode =1.1;                  // ver 1.1
  const int arduinoPinsSize = 54;               // max pins of all arduino boards
  const int virtualDigitalMemorySize = 32;
  const int virtualAnalogMemorySize = 32;

const byte ERROR_PIN      = 1;
const byte ERROR_VALUE    = 2;
const byte ERROR_TYPE     = 3;
const byte ERROR_SIZE     = 4;
const byte ERROR_PASSWORD = 5;
const byte ERROR_COMMAND  = 6;
const byte ERROR_UNKNOWN  = 7;

const char COMMAND_START_CHAR ='!';
const char COMMAND_END_CHAR   ='$';
const String COMMAND_ERROR      = "E00=";

const String clientConnected = "Connected";
const String clientDisconnected = "Disconnected";

 class VirtuinoEthernet_WebServer {
  public:
    VirtuinoEthernet_WebServer(int port);
    void run();
    void vDigitalMemoryWrite(int digitalMemoryIndex, int value);
    int  vDigitalMemoryRead(int digitalMemoryIndex);
    void vMemoryWrite(int memoryIndex, float value);
    float vMemoryRead(int memoryIndex);
    int getPinValue(int pin);
    //void vPinMode(int pin, int mode); 
    boolean DEBUG=false;
    String password=""; 
  
    private:
    String checkNetworkCommand(String command);
    char  getCommandType(String aCommand);
    int  getCommandPin(String aCommand);
    float getCommandValue(String aCommand);
    String getErrorCommand(byte code);
    String executeReceivedCommand(char activeCommandType, int activeCommandPin ,int activeCommandValue, boolean returnInfo);

    byte arduinoPinsValue[arduinoPinsSize] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    byte virtualDigitalMemory[virtualDigitalMemorySize]    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                      
    float virtualFloatMemory[virtualAnalogMemorySize]     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                         
    byte analogInputPinsMap[arduinoAnalogPinsSize]  = analogInputPinsMap_  ;
    int serverPort=80;
   


  
 };


