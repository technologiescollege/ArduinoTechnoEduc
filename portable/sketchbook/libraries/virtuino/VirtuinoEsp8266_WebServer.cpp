/* Virtuino ESP8266 web server Library ver 1.1
 * Created by Ilias Lamprou
 * Updated Jul 01 2016
 * 
 * Download last Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.javapapers.android.agrofarmlitetrial
 * Getting starting link:
 * Video tutorial link: 
 * Contact address for questions or comments: iliaslampr@gmail.com
 */




//========= VirtuinoEsp8266 Class methods  
// boolean connectESP8266_toInternet(String wifiNetworkName,String wifiNetworkPassword, int port);  Set your home wifi network SSID and PASSWORD  (Put this function on start of void setup)

// boolean createLocalESP8266_wifiServer(String wifiNetworkName,String wifiNetworkPassword, int port, int mode); Use this function to create an ESP8266 wifi local network
//                                                                    set port to 80 
//                                                                    set mode=2 to use ESP8266 only as access point 
//                                                                    set mode=3 to use ESP8266 as access point and internet station.

//  bool esp8266_setIP(byte a1, byte a2, byte a3, byte a4);           set ESP8266 local IP. Use this function after connectESP8266_toInternet function 

//========= Virtuino general methods  
//  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
//  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
//  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
//  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
//  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
//  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin


#include "VirtuinoEsp8266_WebServer.h"


#ifdef ESP8266_USE_SOFTWARE_SERIAL
VirtuinoEsp8266_WebServer::VirtuinoEsp8266_WebServer(SoftwareSerial &uart, uint32_t baud): espSerial(&uart)
{
    espSerial->begin(baud);
    while (espSerial->available()) espSerial->read();
 
}
#else
VirtuinoEsp8266_WebServer::VirtuinoEsp8266_WebServer(HardwareSerial &uart, uint32_t baud): espSerial(&uart)
{
    espSerial->begin(baud);
    while (espSerial->available()) espSerial->read();
}
#endif

//================================================================== virtuinoRun
void  VirtuinoEsp8266_WebServer::run() {
   if (networkState>0) {
   if(espSerial->available())   wifiRun();
  }
    
  
}

//================================================================== connectESP8266_toInternet
//================================================================== 
boolean VirtuinoEsp8266_WebServer::connectESP8266_toInternet(String wifiNetworkName,String wifiNetworkPassword, int port){
   int oldNetworkState=networkState;
   networkState=0;
   if (wifiInternetConnect(wifiNetworkName,wifiNetworkPassword, port)) networkState=1;        // first attempt to connect 
   else {
          delay(20000);
          if (wifiInternetConnect(wifiNetworkName,wifiNetworkPassword, port)) networkState=1;        // second attempt to connect 
              else {
                delay(20000);
                if (wifiInternetConnect(wifiNetworkName,wifiNetworkPassword, port)) networkState=1;        // third attempt to connect 
             }
   }
   if (networkState==0) networkState=oldNetworkState;
   if (networkState==0) return false; 
   else return true;
}

//================================================================== createLocalESP8266_wifiServer
//================================================================== 
boolean  VirtuinoEsp8266_WebServer::createLocalESP8266_wifiServer(String wifiNetworkName,String wifiNetworkPassword, int port, int mode){
  int oldNetworkState=networkState;
  networkState=0;
  if (initLocalWifiServer(wifiNetworkName,wifiNetworkPassword,port, mode)) networkState=1;        // first attempt to connect 
   else {
          delay(5000);
          if (initLocalWifiServer(wifiNetworkName,wifiNetworkPassword,port,mode)) networkState=1;      // second attempt to connect 
              else {
                delay(5000);
                if (initLocalWifiServer(wifiNetworkName,wifiNetworkPassword,port,mode)) networkState=1;        // third attempt to connect 
             }
   }
   if (networkState==0) networkState=oldNetworkState;
   if (networkState==0) return false; 
   else return true;
}


//====================================================================== resetESP8266
bool VirtuinoEsp8266_WebServer::esp8266_reset(void){
    unsigned long start;
    clearESP_buffer(100);    // clear esp buffer
    espSerial->println("AT+RST");
    return (responceString("OK",3000));
}

//====================================================================== setESP8266_mode
bool VirtuinoEsp8266_WebServer::esp8266_mode(int mode){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->print("AT+CWMODE=");
    espSerial->println(mode);
    return responceString("OK", "no change",3000);
}
//====================================================================== setESP8266_mode
bool VirtuinoEsp8266_WebServer::esp8266_info(){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->println("AT+GMR");
    return responceString("OK", 3000); 
}
//====================================================================== setESP8266_mode
bool VirtuinoEsp8266_WebServer::esp8266_baud(int baud){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->print("AT+CIOBAUD=");
    espSerial->println(baud);
    return responceString("OK", "no change",3000);
}
//====================================================================== esp8266_connect
bool VirtuinoEsp8266_WebServer::esp8266_connect(String ssid, String pwd){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->print("AT+CWJAP=\"");
    espSerial->print(ssid);
    espSerial->print("\",\"");
    espSerial->print(pwd);
    espSerial->println("\"");
    return responceString("OK",10000);
}
//====================================================================== 
bool VirtuinoEsp8266_WebServer::esp8266_printIP(){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->println("AT+CIFSR");
    return responceString("OK",3000);
}
//====================================================================== 
bool VirtuinoEsp8266_WebServer::esp8266_multiple(int mode){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->print("AT+CIPMUX=");
    espSerial->println(mode);
    return responceString("OK",3000);
}
//=======================================================================
bool VirtuinoEsp8266_WebServer::esp8266_serverPort(int port){
        clearESP_buffer(100);    // clear esp buffer
        espSerial->print("AT+CIPSERVER=1,");
        espSerial->println(port);
        return responceString("OK", "no change",3000);
}
//=======================================================================
//=======================================================================
bool VirtuinoEsp8266_WebServer::esp8266_softAccessPoint(String ssid, String pw){
        clearESP_buffer(100);    // clear esp buffer
        espSerial->print("AT+CWSAP=\"");
        espSerial->print(ssid);
        espSerial->print("\",\"");
        espSerial->print(pw);
        espSerial->println("\",2,3");
        return responceString("OK", "no change",3000);
}

//====================================================================== setESP8266_mode
bool VirtuinoEsp8266_WebServer::esp8266_setIP(byte a1, byte a2, byte a3, byte a4){
    clearESP_buffer(100);    // clear esp buffer
    espSerial->println("AT+CIPSTA=\""+String(a1)+"."+String(a2)+"."+String(a3)+"."+String(a4)+"\"");
    return responceString("OK",3000);
}
//========================================================================================
boolean VirtuinoEsp8266_WebServer::wifiInternetConnect(String wifiNetworkName,String wifiNetworkPassword, int port=80){
    if (DEBUG) Serial.println(END_LINE+serverInitString);
    esp8266_reset(); 
    delay(2000);
    esp8266_info();
    delay(1000);
    esp8266_baud(9600);
    delay(1000);
    esp8266_mode(1);     // configure as client
    if (esp8266_connect(wifiNetworkName,wifiNetworkPassword)){
         delay(2000);
         esp8266_printIP();                       // get ip address
         delay(1000);
         esp8266_multiple(1);                      // configure for multiple connections
         delay(1000);
         esp8266_serverPort(port);                  // turn on server on port 80  
        if (DEBUG) Serial.println(END_LINE+serverReadyString);
        return true;
    }
    else {
      if (DEBUG) Serial.println(END_LINE+serverNotConnectedString);
      return false;
    }
}

//=======================================================================
 boolean VirtuinoEsp8266_WebServer::initLocalWifiServer(String wifiNetworkName,String wifiNetworkPassword,int port,int mode=2){
      if (DEBUG) Serial.println(END_LINE+serverInitString);
      if ((mode<2)|| (mode>3)) mode=2; 
      esp8266_reset(); 
      delay(2000);
      esp8266_info();
      delay(1000);
      esp8266_baud(9600);
      delay(1000);
     
      if (esp8266_mode(mode)){     // configure as access point
         delay(1000);
         if (esp8266_printIP()){                       // get ip address  
           esp8266_softAccessPoint(wifiNetworkName,wifiNetworkPassword);
            if (esp8266_multiple(1)){                      // configure for multiple connections    
              if (esp8266_serverPort(port)){                  // turn on server on port 80  
                if (DEBUG) Serial.println(END_LINE+serverReadyString);
                return true;
              }
            
            }
         }
      }
      if (DEBUG) Serial.println(END_LINE+serverNotConnectedString);
      return false;
  }

//======================================================================================== wifiRun()
//========================================================================================
//========================================================================================
 String commandBuffer="";

void VirtuinoEsp8266_WebServer::wifiRun(){
   if(espSerial->available()){                       
    char c=espSerial->read();
    commandBuffer.concat(c);
    if (c=='\n') {                                            // check command line to line
          if (DEBUG) Serial.print(commandBuffer);
          int pos = commandBuffer.indexOf("+IPD,");
          if (pos>=0){                                        // check for GET command from Virtuino app
              clearESP_buffer(500);
              int connectionId = commandBuffer.charAt(pos+5)-48;  // get connection ID
              if (DEBUG) Serial.println("ID="+ String(connectionId));
              pos = commandBuffer.indexOf("GET /");
              if (pos!=-1){                                   // We have a GET message
                  String responce = checkNetworkCommand(commandBuffer.substring(pos+5));
                  boolean b=wifiSendData(connectionId,responce);
                  // -----------  Close client connection     -------------
                  clearESP_buffer(100);
                  closeClientConnection(connectionId);           
                    
                  
              } 
          } 
        
        commandBuffer="";                                     // clear buffer for the next line
  
      }  
    
   }
}



//======================================================================================
String  VirtuinoEsp8266_WebServer::checkNetworkCommand(String command){
  String returnedString="";
  
  int pos=command.lastIndexOf(COMMAND_END_CHAR);                      
  if (pos>5){                                          // check the size of command
      String command1= command.substring(0,pos+1);     // clear the command
      if (DEBUG) Serial.println(END_LINE+"cmd:"+ command1);

      //------------------  get command password
       String commandPassword="";
       int k=command1.indexOf(COMMAND_START_CHAR);
       if (k>0) commandPassword = command1.substring(0,k);
      if (DEBUG) Serial.println(END_LINE+"ps:"+commandPassword ); 
       
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

//======================================================================================== recvString
boolean VirtuinoEsp8266_WebServer::responceString(String target1,  int timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    boolean rValue=false;
    while (millis() - start < timeout) {
        while(espSerial->available() > 0) {
            a = espSerial->read();
            if (DEBUG) Serial.print(a);
            if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            rValue=true;
            break;
        } 
    }
    return rValue;
}
//======================================================================================== recvString
boolean VirtuinoEsp8266_WebServer::responceString(String target1, String target2, int timeout)
{
    String data;
    char a;
    boolean rValue=false;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(espSerial->available() > 0) {
            a = espSerial->read();
             if (DEBUG) Serial.print(a);
             if(a == '\0') continue;
             data += a;
        }
        if (data.indexOf(target1) != -1) {
            rValue=true;
            break;
        } else if (data.indexOf(target2) != -1) {
            rValue=true;
            break;
        }
    }
    return rValue;
}
//========================================================================================  wifiSendData
//========================================================================================
//========================================================================================
boolean VirtuinoEsp8266_WebServer::wifiSendData(int connectionId, String content){

  long time = millis();
  boolean returnedValue=false;
  // build HTTP response
  String httpResponse="";
  String httpHeader;
  // HTTP Header
  httpHeader = "HTTP/1.1 200 OK"+END_LINE+"Content-Type: text/html; charset=UTF-8"+END_LINE;
  httpHeader += "Content-Length: ";
  httpHeader += content.length();
  httpHeader += "\r\n";
  httpHeader += "Connection: close\r\n\r\n";
  httpResponse=httpHeader;
  httpResponse.concat(content);
  httpResponse.concat(" ");
   //---Send command
  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += httpResponse.length();
  cipSend += "\r\n";

  if (DEBUG) Serial.println("\r\n"+cipSend);
  for (int i=0;i<cipSend.length();i++) espSerial->write(cipSend.charAt(i));

  if (responceString(">",2000)) {
      for (int i=0;i<httpResponse.length();i++) {
         espSerial->write(httpResponse.charAt(i));
        if (DEBUG) Serial.print(httpResponse.charAt(i));
       }

      if (responceString("SEND OK",10000)) returnedValue=true;
  }
  return returnedValue;
}




//========================================================================================  sendCommand
//========================================================================================
//========================================================================================
  String VirtuinoEsp8266_WebServer::sendCommand(String command, const int timeout){
  String response = "";
  espSerial->print(command); // send the read character to the esp8266
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial->available()){
      char c = espSerial->read(); // read the next character.
      response += c;
    }
  }
if (DEBUG) Serial.print(response);
  return response;
}

//========================================================================================  closeClientConnection
//========================================================================================
//========================================================================================
void VirtuinoEsp8266_WebServer::closeClientConnection(int connectionID){
    String closeCommand = "AT+CIPCLOSE=";       // close AT command 
    closeCommand+=connectionID;                 // append connection id
    closeCommand+="\r\n";
    espSerial->print(closeCommand); 
  
}

//======================================================================================== clearESP_buffer
//========================================================================================
//========================================================================================
void VirtuinoEsp8266_WebServer::clearESP_buffer(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial->available()){
        c=espSerial->read();
        if (DEBUG) Serial.print(c);
      }
    }
                    
//================================================================== formatValue - util function

 
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
  
  char  VirtuinoEsp8266_WebServer::getCommandType(String aCommand){
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
 
  int  VirtuinoEsp8266_WebServer::getCommandPin(String aCommand){
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
float VirtuinoEsp8266_WebServer::getCommandValue(String aCommand){
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

String VirtuinoEsp8266_WebServer::getErrorCommand(byte code){
  return COMMAND_START_CHAR+COMMAND_ERROR+String(code)+COMMAND_END_CHAR;
}
//==================================================================
//================================================================= executeCommandFromApp
  String VirtuinoEsp8266_WebServer::executeReceivedCommand(char activeCommandType, int activeCommandPin ,int activeCommandValue, boolean returnInfo){
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
int VirtuinoEsp8266_WebServer::getPinValue(int pin){
  if (pin>=0 && pin<arduinoPinsSize) return arduinoPinsValue[pin];
  else return 0;
  }
  //=================================================================== vmDigitalWrite
 void VirtuinoEsp8266_WebServer::vDigitalMemoryWrite(int digitalMemoryIndex, int value){
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<virtualDigitalMemorySize)){
    if (value==0) virtualDigitalMemory[digitalMemoryIndex]=0;
      else virtualDigitalMemory[digitalMemoryIndex]=1;
  }
  
}
//=================================================================== vmDigitalRead
 int VirtuinoEsp8266_WebServer::vDigitalMemoryRead(int digitalMemoryIndex){
  if ((digitalMemoryIndex>=0) & digitalMemoryIndex<virtualDigitalMemorySize){
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//=================================================================== vmAnalogWrite
// This void must not be called more than two times per second
void VirtuinoEsp8266_WebServer::vMemoryWrite(int memoryIndex, float value){
  if ((memoryIndex>=0) & memoryIndex<virtualAnalogMemorySize){
      virtualFloatMemory[memoryIndex]=value;
  }
}

//=================================================================== vmAnalogRead
 float VirtuinoEsp8266_WebServer::vMemoryRead(int memoryIndex){
  if ((memoryIndex>=0) & memoryIndex<arduinoAnalogPinsSize){
    return virtualFloatMemory[memoryIndex];
  }
  else return 0;    // error
}
