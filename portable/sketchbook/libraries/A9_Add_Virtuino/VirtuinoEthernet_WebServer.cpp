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
 


#include "VirtuinoEthernet_WebServer.h"
//#include <SPI.h>
#include <Ethernet.h>



 

  VirtuinoEthernet_WebServer::VirtuinoEthernet_WebServer(int port){
  serverPort=port;
}

 
 
 String lineBuffer="";
 void VirtuinoEthernet_WebServer::run(){
    EthernetServer server(serverPort);  
    EthernetClient client = server.available();
  if (client) {
    char c;
   if (DEBUG) Serial.println(clientConnected);
    
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        lineBuffer.concat(c);
        if (DEBUG)  Serial.write(c);
      }
      if (c == '\n' ) {
              int pos = lineBuffer.indexOf("GET /");
              if (pos!=-1){                                   // We have a GET message
                   if (DEBUG) Serial.println("\n\r LineBuffer="+lineBuffer);
                  String responce = checkNetworkCommand(lineBuffer.substring(pos+5));
                   if (DEBUG) Serial.println("\n\r Responce="+responce);
                  delay(10);
                  client.flush();
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: close");
                  client.println();
                  client.println(responce);
                  lineBuffer="";
                  break;
                } // if pos
             lineBuffer="";
        } // if c=='\n'
    } // if client.available
    delay(1);
    client.stop();  // close the connection:
    if (DEBUG) Serial.println(clientDisconnected);
  }
  }
  
//======================================================================================
String  VirtuinoEthernet_WebServer::checkNetworkCommand(String command){
  String returnedString="";
  
  int pos=command.lastIndexOf(COMMAND_END_CHAR);                      
  if (pos>5){                                          // check the size of command
      String command1= command.substring(0,pos+1);     // clear the command
      if (DEBUG) Serial.println("\r\nCommand = "+ command1);

       //------------------  get command password
       String commandPassword="";
       int k=command1.indexOf(COMMAND_START_CHAR);
       if (k>0) commandPassword = command1.substring(0,k);
       if (DEBUG) Serial.println("\r\nCommand password:"+commandPassword ); 
       
        
      if ((password.length()==0) || (commandPassword.equals(password))) {                       // check if password correct
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
      } else returnedString=getErrorCommand(ERROR_PASSWORD);     // the password is not correct
  }
  else  returnedString=getErrorCommand(ERROR_SIZE);         // the size of command is not correct
 
  return returnedString;
}


 //================================================================= getCommandType
  //This function returns the command type which was received from app
  //The second character of command string determines the command type
  // I  digital input read command    
  // Q  digital ouput write
  // D  digital memory read - write commang
  // A  analog input analog input read command
  // O  analog output write command
  // V  float memory read - write command
  // C  connect or disconect command    
  // E  error
  
  // Other characters are recognized as an error and function returns the character E
  //This is a system fuction. Don't change this code
  
  char  VirtuinoEthernet_WebServer::getCommandType(String aCommand){
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
 
  int  VirtuinoEthernet_WebServer::getCommandPin(String aCommand){
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

//================================================================= getCommandType
float VirtuinoEthernet_WebServer::getCommandValue(String aCommand){
     int value=0;
     String valueString=aCommand.substring(4);
     boolean er=false;
    //   Serial.println("\r\nvalueString.length= "+String(valueString.length()));
       for (int i=0;i<valueString.length();i++){
        char c=valueString.charAt(i);
        if (! ((c=='.') || (isDigit(valueString.charAt(i))))) er=true;
       }
       if (er==false) value=valueString.toInt();
      
    return value;
  }

String VirtuinoEthernet_WebServer::getErrorCommand(byte code){
  return COMMAND_START_CHAR+COMMAND_ERROR+String(code)+COMMAND_END_CHAR;
}
//==================================================================
//================================================================= executeCommandFromApp
  String VirtuinoEthernet_WebServer::executeReceivedCommand(char activeCommandType, int activeCommandPin ,int activeCommandValue, boolean returnInfo){
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
      case 'I':                         
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
                          responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+COMMAND_END_CHAR;  // response 
                   } else    responce =getErrorCommand(ERROR_VALUE);
            } else   responce =getErrorCommand(ERROR_PIN);;  // response  error pin number   !E00=1$   
          break; 

       case 'D':
            if ((activeCommandPin>=0) & (activeCommandPin<virtualDigitalMemorySize)){ 
                if (returnInfo) responce =COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+COMMAND_END_CHAR;  // response 
                else
                if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
                      virtualDigitalMemory[activeCommandPin]= activeCommandValue; 
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
                      //arduinoPinsMap[activeCommandPin]=3; 
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
        //         connectedStatus=true;
                 responce ="!C"+pinString+"="+firmwareCode+"$";
               }
              else if (activeCommandValue==0) {
            //    connectedStatus=false;
                responce =COMMAND_START_CHAR+"C"+pinString+"=0"+COMMAND_END_CHAR;
               }
              else      responce =getErrorCommand(ERROR_VALUE);;
              
            } else   responce =getErrorCommand(ERROR_COMMAND);        
          break;  
           }
           
        return responce;
       
  }

//=================================================================== getPinValue
int VirtuinoEthernet_WebServer::getPinValue(int pin){
  if (pin>=0 && pin<arduinoPinsSize) return arduinoPinsValue[pin];
  else return 0;
  }

//=================================================================== vmDigitalWrite
 void VirtuinoEthernet_WebServer::vDigitalMemoryWrite(int digitalMemoryIndex, int value){
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<virtualDigitalMemorySize)){
    if (value==0) virtualDigitalMemory[digitalMemoryIndex]=0;
      else virtualDigitalMemory[digitalMemoryIndex]=1;
  }
  
}
//=================================================================== vmDigitalRead
 int VirtuinoEthernet_WebServer::vDigitalMemoryRead(int digitalMemoryIndex){
  if ((digitalMemoryIndex>=0) & digitalMemoryIndex<virtualDigitalMemorySize){
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//=================================================================== vmAnalogWrite
// This void must not be called more than two times per second
void VirtuinoEthernet_WebServer::vMemoryWrite(int memoryIndex, float value){
  if ((memoryIndex>=0) & memoryIndex<virtualAnalogMemorySize){
      virtualFloatMemory[memoryIndex]=value;
  }
}

//=================================================================== vmAnalogRead
 float VirtuinoEthernet_WebServer::vMemoryRead(int memoryIndex){
  if ((memoryIndex>=0) & memoryIndex<arduinoAnalogPinsSize){
    return virtualFloatMemory[memoryIndex];
  }
  else return 0;    // error
}
