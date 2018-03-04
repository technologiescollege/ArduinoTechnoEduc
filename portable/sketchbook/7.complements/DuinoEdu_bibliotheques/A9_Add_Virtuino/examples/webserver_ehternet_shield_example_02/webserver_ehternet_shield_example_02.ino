
/* Virtuino Ethernet Shield web server example No2  
 * Example name = "Read digital or analog inputs - Write to analog outputs"
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


/*========= Virtuino General methods  
*
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
*  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
*/ 

#include "VirtuinoEthernet_WebServer.h"                           // Neccesary virtuino library for ethernet shield

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};                // Set the ethernet shield mac address.
IPAddress ip(192, 168, 2, 150);                                   // Set the ethernet shield ip address. Check your gateway ip address first
VirtuinoEthernet_WebServer virtuino(80);                          // default port=80
 


//================================================================== setup
//==================================================================
//==================================================================
void setup() 
{
  
  virtuino.DEBUG=true;                                            // set this value TRUE to enable the serial monitor status
  Serial.begin(9600);                                             // Enable this line only if DEBUG=true
                                     
  Ethernet.begin(mac, ip);
  
  virtuino.password="1234";                                     // Set a password to your web server for more protection
                                                                // avoid special characters like ! $ = @ # % & * on your password. Use only numbers or text characters
                              
 
  
//------ enter your setup code below

                              // Make the following connections to your arduino board to test this example
   pinMode(5,INPUT);          // Connect a switch or button to pin 5 as digtial input 
   pinMode(6,OUTPUT);         // Connect a LED to pin 12 as digital output
   pinMode(9,OUTPUT);         // Connect a LED to pin 10 as analog output.
  
                              // Connect a potentiometer to analog pin A0                                 
                              // Don't use pins 10,11,12,13 in your code. They used by Ethernet Shield

   
                                                  
}


//================================================================== loop
//==================================================================
//==================================================================


void loop(){
   virtuino.run();           //  neccesary command to communicate with Virtuino android app
   
    //------ enter your loop code below here
    //------ avoid to use delay() function in your code

    // your code .....





        
     //----- end of your code
 }


