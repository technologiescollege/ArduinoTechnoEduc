/* Virtuino bluetooth library ver 1.1
 * Created by Ilias Lamprou
 * Updated Jul 01 2016
 * 
 * Download last Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino
 * Getting starting link:
 * Video tutorial link: https://www.youtube.com/watch?v=CYR_jigRkgk
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

/*========= VirtuinoBluetooth Class methods  
*  vPinMode(int pin, int state)                                  set pin as digital OUTPUT or digital INPUT.  (Insteed default pinMode method
*
*========= Virtuino General methods  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
*  run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins
*/


#define VirtuinoBluetooth_h

#include "Arduino.h"

#define BLUETOOTH_USE_SOFTWARE_SERIAL  // disable this command if you want to use hardware serial

#ifdef BLUETOOTH_USE_SOFTWARE_SERIAL
#include "SoftwareSerial.h"
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define arduinoAnalogPinsSize 16  
      #define analogInputPinsMap_ { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 }
    
#else
      #define arduinoAnalogPinsSize 6  
      #define analogInputPinsMap_ { A0, A1, A2, A3, A4, A5 }
     
#endif

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

const float firmwareCode =1.1;                  // ver 1.1
const int arduinoPinsSize = 54;               // max pins of all arduino boards
const int virtualDigitalMemorySize = 32;
const int virtualAnalogMemorySize = 32;




 class VirtuinoBluetooth {
  public:

  #ifdef BLUETOOTH_USE_SOFTWARE_SERIAL
    VirtuinoBluetooth(SoftwareSerial &uart, uint32_t baud = 9600);
  #else 
    VirtuinoBluetooth(HardwareSerial &uart, uint32_t baud = 9600);
  #endif

 
  void run();
  void vDigitalMemoryWrite(int digitalMemoryIndex, int value);
  int vDigitalMemoryRead(int digitalMemoryIndex);
  void vMemoryWrite(int memoryIndex, float value);
  int vMemoryRead(int memoryIndex);
  int getPinValue(int pin);
  void vPinMode(int pin, int mode); 
  boolean DEBUG=false;
 
  
  private:
  void checkIfIOsHaveChanged();
  String checkBluetoothCommand(String command);
  void sendCommandResponse(char commandType, int commandPin , String commandValueString);
  void  readBluetoothSerialData();
  String executeReceivedCommand(char activeCommandType, int activeCommandPin ,int activeCommandValue, boolean returnInfo);
  String getErrorCommand(byte code);
  float getCommandValue(String aCommand);
  int  getPinformString(String value);
  char  getCommandType(String aCommand);
  int  getCommandPin(String aCommand);
  
  int arduinoPinsMap[arduinoPinsSize] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int arduinoPinsValue[arduinoPinsSize] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int virtualDigitalMemory[virtualDigitalMemorySize]    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                      
  int virtualDigitalMemoryIdol[virtualDigitalMemorySize]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                          
  float virtualFloatMemory[virtualAnalogMemorySize]     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                         
  int analogInputPinsMap[arduinoAnalogPinsSize]  = analogInputPinsMap_  ;
  String commandBuffer="";                                         // Store the active command received from app
  boolean newCommand=false;                                        // This flag takes the value 1 every time we have a commplete command from app
  int   commandState=0;                                            // to manage if command exist or command errors   0= no command     1= execute the command   -1 error in command
  char  activeCommandType;                                         //Temporary stored the type of last command received from app
  int   activeCommandPin;                                          //Temporary stored the pin of last command received from app
  int   activeCommandValue;                                        //Temporary stored the value of last command received from app
  bool  connectedStatus=true;
 

  
  #ifdef BLUETOOTH_USE_SOFTWARE_SERIAL
      SoftwareSerial *BTserial;
  #else
      HardwareSerial *BTserial;
  #endif                                                                 

 };





