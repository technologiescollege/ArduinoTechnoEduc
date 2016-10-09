
/* Virtuino ESP8266 web server example No2   
 * Example name = "Control arduino outputs - read arduino inputs via internet"
  * Created by Ilias Lamprou
 * Updated Jul 01 2016
 * Before  running this code config the settings below as the instructions on the right
 * 
 * 
 * Download last Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.javapapers.android.agrofarmlitetrial
 * Getting starting link:
 * Video tutorial link: 
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

/*========= VirtuinoEsp8266 Class methods  
 
 * boolean connectESP8266_toInternet(String wifiNetworkName,String wifiNetworkPassword, int port);  Set your home wifi network SSID and PASSWORD  (Put this function on start of void setup)
 *
 * boolean createLocalESP8266_wifiServer(String wifiNetworkName,String wifiNetworkPassword, int port, int mode); Use this function to create an ESP8266 wifi local network
 *                                                                   set port to 80 
 *                                                                   set mode=2 to use ESP8266 only as access point 
 *                                                                   set mode=3 to use ESP8266 as access point and internet station.
 *
 *  bool esp8266_setIP(byte a1, byte a2, byte a3, byte a4);           set ESP8266 local IP. Use this function after connectESP8266_toInternet function 
 *
 * ========= Virtuino general methods  
 *  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
 *  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
 *  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
 *  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
 *  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
 *  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
 *
 */
 
#include "VirtuinoEsp8266_WebServer.h"



// Code to use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin
VirtuinoEsp8266_WebServer virtuino(espSerial,9600);   // Your esp8266 device's speed is probably at 115200. For this reason the first time set the speed to 115200 or your esp8266 configured speed and upload
                                                        // then change the serial speed to 9600 and upload again
                                                      
// Code to use HardwareSerial
// VirtuinoEsp8266_WebServer virtuino(Serial3,9600);    // enable this line and disable all Software serial lines
                                                        // Open VirtuinoESP8266_WebServer.h file on the virtuino library folder
                                                        // and disable th line: #define ESP8266_USE_SOFTWARE_SERIAL 




//================================================================== setup
//==================================================================
//==================================================================
void setup() 
{
  virtuino.DEBUG=true;                                            // set this value TRUE to enable the serial monitor status.It is neccesary to get your esp8266 local ip
  Serial.begin(9600);                                             // Enable this line only if DEBUG=true

  virtuino.connectESP8266_toInternet("MY WIFI NETWORK NAME","PASSWORD",80);  // Set your home wifi router SSID and PASSWORD. ESP8266 will connect to Internet. Port=80
  virtuino.esp8266_setIP(192,168,2,140);                                     // Set a local ip. Forward port 80 to this IP on your router

  //virtuino.createLocalESP8266_wifiServer("ESP8266 NETWORK NAME","PASSWORD",80,2);   //Enable this line to create a wifi local netrork using ESP8266 as access point
                                                                                      //Do not use less than eight characters for the password. Port=80
                                                                                      //Default access point ESP8266 ip=192.168.4.1. 
 
  virtuino.password="1234";                                     // Set a password to your web server for more protection 
                                                                // avoid special characters like ! $ = @ # % & * on your password. Use only numbers or text characters
                              
                  
  
  
//------ enter your setup code below
                              // Make the following connections to your arduino board to test this example
   pinMode(12,OUTPUT);        // Connect a LED to pin 12 as digital output
   pinMode(5,INPUT);          // Connect a switch or button to pin 5 as digtial input 
   pinMode(10,OUTPUT);        // Connect a LED to pin 10 as analog output.
  
                              // Connect a potentiometer to analog pin A0                                 
  
 
    
    
}


//================================================================== loop
//==================================================================
//==================================================================


void loop(){
   virtuino.run();           //  neccesary command to communicate with Virtuino android app
   
    //------ enter your loop code below here
    //------ avoid to use delay() function in your code

    // your code .....

   
    // in this example you don't have to add any code 
    


  
        
     //----- end of your code
 }


