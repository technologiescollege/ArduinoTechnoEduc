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


#include "VirtuinoBluetooth.h"

 
char tempChar = ' ';    
 


#ifdef BLUETOOTH_USE_SOFTWARE_SERIAL
VirtuinoBluetooth::VirtuinoBluetooth(SoftwareSerial &uart, uint32_t baud): BTserial(&uart)
{
    BTserial->begin(baud);
    while (BTserial->available()) BTserial->read();
 
}
#else
VirtuinoBluetooth::VirtuinoBluetooth(HardwareSerial &uart, uint32_t baud): BTserial(&uart)
{
    BTserial->begin(baud);
    while (BTserial->available()) BTserial->read();
}
#endif

//================================================================== virtuinoRun
void  VirtuinoBluetooth::run() {
  checkIfIOsHaveChanged();                                   // This void informs VirtUino app every time a pin's status is changed 
  if (BTserial->available()) readBluetoothSerialData();    // while serial buffer is not empty we read and store the character to commandBuffer
  
}


//================================================================== bluetoothInit
//void VirtuinoBluetooth::bluetoothInit(long bluetoothSerialSpeed){
//   BTserial->begin(bluetoothSerialSpeed);     // Set to your bluetooth serial speed
// 
//}

//================================================================= checkForIOsChanges
void VirtuinoBluetooth::checkIfIOsHaveChanged(){
   int pin;
    int p;
    int pinValue;
       if (!connectedStatus) return;
        //---- check if digitalInput state has changed
         for (int i=0;i<arduinoPinsSize; i++){
           if ((arduinoPinsMap[i]>0) && (arduinoPinsMap[i]<3)) {
              pinValue=digitalRead(i);
              if (arduinoPinsValue[i]!=pinValue){
                  arduinoPinsValue[i]=pinValue;
                  if (arduinoPinsMap[i]==1) sendCommandResponse('Q',i,String(pinValue));
                  else  if (arduinoPinsMap[i]==2) sendCommandResponse('I',i,String(pinValue));
        //          delay(10);
               }
           }
         } // end for  
        
       //---- check if digitalMemory has changed
          for (int i=0;i<virtualDigitalMemorySize; i++){
           if (virtualDigitalMemoryIdol[i] != virtualDigitalMemory[i]){
              virtualDigitalMemoryIdol[i] = virtualDigitalMemory[i];
              sendCommandResponse('D',i,String(virtualDigitalMemory[i]));
           } 
          }

}
//================================================================= readBluetoothSerialData
//This void reads data from bluetooth serial and stores them to commandBuffer
//If we have a valid  command from app, void then calls the void executeReceivedCommand to execute the command
//If command doesn't have a valid format it returns an error response to App  - error code = 0
//This is a system fuction. Don't change this code

                               // temp char
 void VirtuinoBluetooth::readBluetoothSerialData(){
        tempChar=BTserial->read();
        if (DEBUG) Serial.print(tempChar);
        if (tempChar==COMMAND_START_CHAR) {                           // a new command is starting...
           if (newCommand==false) newCommand=true;   
           else sendCommandResponse('E',0,String(ERROR_COMMAND));
           commandBuffer="";                             //clear buffer for the new command
        }
        if (newCommand){                                // add serial charracters to  new command
            commandBuffer.concat(tempChar); 
            if (commandBuffer.length() > 100){             //error  while reading data
              newCommand=false;                           // cancel active command 
              commandBuffer="";                           // clear buffer for the next command
              sendCommandResponse('E',0,String(ERROR_COMMAND));
            } 
            if (tempChar==COMMAND_END_CHAR){                          // end of command
                String responce=checkBluetoothCommand(commandBuffer);
                if (DEBUG) Serial.print("\r\nResponce= "+responce+"\r\n");
                BTserial->print(responce);
                newCommand=false;
                commandBuffer="";
            }
        }
  }
  
//======================================================================================
String  VirtuinoBluetooth::checkBluetoothCommand(String command){
  String returnedString="";
  int pos=command.lastIndexOf(COMMAND_END_CHAR);     
  if (pos>5){                                          // check the size of command
      String command1= command.substring(0,pos+1);     // clear the command
      if (DEBUG) Serial.println("\r\nCommand = "+ command1);

           while (command1.indexOf(COMMAND_START_CHAR)>=0){
              int cStart=command1.indexOf(COMMAND_START_CHAR);
              int cEnd=command1.indexOf(COMMAND_END_CHAR);
              if (cEnd-cStart>5){
              String oneCommand = command1.substring(cStart+1,cEnd);                               // get one command
                char commandType = getCommandType(oneCommand);
                  if (commandType!='E') {
                     int pin= getCommandPin(oneCommand);
                     if (pin!=-1){
                        boolean returnInfo=false;
                        float value=0;
                        if (oneCommand.charAt(4)=='?') returnInfo=true;
                        else value = getCommandValue(oneCommand);
                        String commandResponce=executeReceivedCommand(commandType,pin , value,returnInfo);
                        returnedString += commandResponce;
                     } else  returnedString +=getErrorCommand(ERROR_PIN);  // response  error pin number   !E00=1$   
                  } else returnedString +=getErrorCommand(ERROR_TYPE);  // response  error type   !E00=3$   
          
              } else{
                returnedString +=getErrorCommand(ERROR_SIZE);  // response  error size of command   !E00=4$   
              }
              command1=command1.substring(cEnd+1);
           }  // while
     
  }
  else  returnedString=getErrorCommand(ERROR_SIZE);         // the  command is not correct
 
  return returnedString;
}



//==================================================================
//================================================================= executeCommandFromApp
  String VirtuinoBluetooth::executeReceivedCommand(char activeCommandType, int activeCommandPin ,int activeCommandValue, boolean returnInfo){
    //------------------------------------
   // Every time we have a complete command from bluetooth the global value commandState is set to 1 
   // The value activeCommandType contains command type such as Digital output, Analog output etc.
   // The value activeCommandPin contains the pin number of the command
   // The value activeCommandValue contains the value (0 or 1 for digital, 0-255 for analog outputs, 0-1023 for analog memory) 
   // In this void we have to check if the command is correct and then execute the command 
   // After executing the command we have to send a response to app
   // At last we must reset the command state value to zero 
   //This is a system fuction. Don't change this code
      
    String responce=getErrorCommand(ERROR_UNKNOWN); 
    String pinString="";
    
    if (activeCommandPin<10) pinString = "0"+String(activeCommandPin);
    else pinString=String(activeCommandPin);
   
    switch (activeCommandType) {
      case 'I':                           //digital output command = +Q05=0000#   or  = +Q13=0001#
            if ((activeCommandPin>=0) & (activeCommandPin < arduinoPinsSize))
              responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+COMMAND_END_CHAR;  // response 
            else   responce =getErrorCommand(ERROR_PIN);  // response  error pin number   !E00=1$       
          break;
      case 'Q': 
            if ((activeCommandPin>=0) & (activeCommandPin < arduinoPinsSize)){
                 if (returnInfo) responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+COMMAND_END_CHAR;  // response 
                 else 
                   if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
                          digitalWrite(activeCommandPin,activeCommandValue);
                          arduinoPinsValue[activeCommandPin]=activeCommandValue;
                          //virtualDigitalMemoryIdol[activeCommandPin]=activeCommandValue;
                          responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+COMMAND_END_CHAR;  // response 
                   } else    responce =getErrorCommand(ERROR_VALUE);
            } else   responce =getErrorCommand(ERROR_PIN);  // response  error pin number   !E00=1$   
          break; 

       case 'D':
            if ((activeCommandPin>=0) & (activeCommandPin<virtualDigitalMemorySize)){ 
                if (returnInfo) responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+COMMAND_END_CHAR;  // response 
                else
                if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
                      virtualDigitalMemory[activeCommandPin]= activeCommandValue; 
                      virtualDigitalMemoryIdol[activeCommandPin]= activeCommandValue; 
                      responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+COMMAND_END_CHAR;  // response 
                } else    responce =getErrorCommand(ERROR_VALUE);
            } else   responce =getErrorCommand(ERROR_PIN);  // response  error pin number   !E00=1$   
            break; 
      
       case 'V': 
           if ((activeCommandPin>=0) & (activeCommandPin<virtualAnalogMemorySize)){
               if (!returnInfo) virtualFloatMemory[activeCommandPin]= activeCommandValue; 
               responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+COMMAND_END_CHAR;  // response 
           } else   responce =getErrorCommand(ERROR_PIN);
          break;
       case 'O': 
           if ((activeCommandPin>=0) & (activeCommandPin < arduinoPinsSize)){
                   
                   if (returnInfo) {
                      int pwm_value = pulseIn(activeCommandPin, HIGH);
                      pwm_value = pwm_value /7.85;
                      responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(pwm_value)+COMMAND_END_CHAR;  // response 
                  }
                   else if ((activeCommandValue>=0) && (activeCommandValue<256)) {
                      arduinoPinsMap[activeCommandPin]=3; 
                      arduinoPinsValue[activeCommandPin]=(int) activeCommandValue;
                      analogWrite(activeCommandPin,(int)activeCommandValue);
                      responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String((int)activeCommandValue)+COMMAND_END_CHAR;  // response 
                  } else responce =getErrorCommand(ERROR_VALUE);
           } else   responce =getErrorCommand(ERROR_PIN);;
          break;

         case 'A':                        
            if ((activeCommandPin>=0) & (activeCommandPin < arduinoAnalogPinsSize))
              responce ="!"+String(activeCommandType)+pinString+"="+String(analogRead(analogInputPinsMap[activeCommandPin]))+"$";  // response 
            else   responce =getErrorCommand(ERROR_PIN);  // response  error pin number   !E00=1$       
          break;  
         
           case 'C':                   
            if ((activeCommandPin>=0) & (activeCommandPin <=1)){          //0 connect via bluetooth - 1 connect via network 
               if (activeCommandValue==1) {
                  connectedStatus=true;
                 responce ="!C"+pinString+"="+String(firmwareCode)+"$";
               }
              else if (activeCommandValue==0) {
                connectedStatus=false;
                responce =COMMAND_START_CHAR+"C"+pinString+"=0"+COMMAND_END_CHAR;
               }
              else      responce =getErrorCommand(ERROR_VALUE);;
              
            } else   responce =getErrorCommand(ERROR_COMMAND);        
          break;  
           }
           
        return responce;
       
  }

  //============================================================================== getCommandType
 //This function returns the value of the command string which was received from app
 //For digital output commands returns 0 or 1. 
 //For digital memory commands returns 0 or 1. 
 //For analog output commands returns a value between 0-255 
 //For analog memory commands returns a value between 0-1023
 //If value is out of the corect range it returns -1 as error code
 //This is a system function. Don't change this code
 
float VirtuinoBluetooth::getCommandValue(String aCommand){
     int value=0;
     String valueString=aCommand.substring(4);
     boolean er=false;
       for (int i=0;i<valueString.length();i++){
        char c=valueString.charAt(i);
        if (! ((c=='.') || (isDigit(valueString.charAt(i))))) er=true;
       }
       if (er==false) value=valueString.toInt();
    return value;
  }

 //================================================================== formatValue - util function
  //==================================================================
  //This is a system function. Don't change this code
  
   int   VirtuinoBluetooth::getPinformString(String value){
     int pin=-1;
     if (value.length()==2){
      char c1 = value.charAt(0);
      char c2 = value.charAt(1);
      if (((c1>='0') & (c1<='9')) & ((c2>='0') & (c2<='9'))) {
        String s = "";
        s.concat(c1);
        s.concat(c2);
        pin=s.toInt();
      }
    return pin;
     }
 
   }
   //================================================================= getCommandType
  //This function returns the command type which was received from app
  //The second character of command string determines the command type
  // I  digital input read command    
  // Q  digital ouput write
  // D  digital memory read - write commang
  // A  analog input analog input read command
  // O  analog output write command
  // V  analog memory read - write command
  // C  connect or disconect command    
  // E  error
  
  // Other characters are recognized as an error and function returns the character E
  //This is a system fuction. Don't change this code
  
  char  VirtuinoBluetooth::getCommandType(String aCommand){
   char commandType= aCommand.charAt(0);
    if (!(commandType == 'I' || commandType == 'Q' || commandType == 'D' ||commandType == 'A' ||commandType == 'O' || commandType == 'V' || commandType == 'C')){
      commandType='E'; //error  command
    }
    return commandType;
  }
  //================================================================= getCommandType
  //This function returns the pin number of the command string which was received from app
  //Fuction checks if pin number is correct. If not it returns -1 as error code
  //This is a system fuction. Don't change this code
 
  int  VirtuinoBluetooth::getCommandPin(String aCommand){
    char p1= aCommand.charAt(1);
    char p2= aCommand.charAt(2);
    String s="-1";
    if (isDigit(p1) && isDigit(p2)) {
       s="";
       s.concat(p1);
       s.concat(p2);
    }
     return s.toInt();
  }


 //==================================================================  sendCommandResponse  
 //================================================================== 
 void  VirtuinoBluetooth::sendCommandResponse(char commandType, int commandPin , String commandValueString){
  // command format examples  +Q13=0001#    +I05=000?  +D10=0000#  +A03=000?#   +O10=0090#   +V02=0456#
    if (!connectedStatus) return;
    String s="";
    s.concat(COMMAND_START_CHAR);
    s.concat(commandType);
    if (commandPin<10) s.concat('0');
    s=s+String(commandPin);
    s.concat('=');
    s.concat(commandValueString);
    s.concat(COMMAND_END_CHAR);
    if (DEBUG) Serial.print("\r\nResponce= "+s+"\r\n");
    BTserial->print(s);
   
    delay(10);              
}



String VirtuinoBluetooth::getErrorCommand(byte code){
  return COMMAND_START_CHAR+COMMAND_ERROR+String(code)+COMMAND_END_CHAR;
}
//=================================================================== vmDigitalWrite
 void VirtuinoBluetooth::vDigitalMemoryWrite(int digitalMemoryIndex, int value){
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<virtualDigitalMemorySize)){
    if (value==0) virtualDigitalMemory[digitalMemoryIndex]=0;
      else virtualDigitalMemory[digitalMemoryIndex]=1;
  }
  
}
//=================================================================== vmDigitalRead
 int VirtuinoBluetooth::vDigitalMemoryRead(int digitalMemoryIndex){
  if ((digitalMemoryIndex>=0) & digitalMemoryIndex<virtualDigitalMemorySize){
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//=================================================================== vmAnalogWrite
// This void must not be called more than two times per second
void VirtuinoBluetooth::vMemoryWrite(int memoryIndex, float value){
  if ((memoryIndex>=0) & memoryIndex<virtualAnalogMemorySize){
      virtualFloatMemory[memoryIndex]=value;
  }
}

//=================================================================== vmAnalogRead
 int VirtuinoBluetooth::vMemoryRead(int memoryIndex){
  if ((memoryIndex>=0) & memoryIndex<arduinoAnalogPinsSize){
    return virtualFloatMemory[memoryIndex];
  }
  else return 0;    // error
}


//===================================================================
int VirtuinoBluetooth::getPinValue(int pin){
  if (pin>=0 && pin<arduinoPinsSize) return arduinoPinsValue[pin];
  else return 0;
  }

//===================================================================
void VirtuinoBluetooth::vPinMode(int pin, int mode){
  if (pin>=0 && pin<arduinoPinsSize){
    pinMode(pin,mode);
    if (mode == OUTPUT ) arduinoPinsMap[pin] =  1;
      else if (mode == INPUT ) arduinoPinsMap[pin] =  2;
  }
}

