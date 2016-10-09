/* Virtuino example No3 (Bluetooth)
 * Example name = "Read - write to Virtuino memory"
 * Created by Ilias Lamprou
 * Updated Jul 01 2016
 * This is the code you need to run on your arduino board to cummunicate whith VirtUino app via bluetooth
 * Before  running this code config the settings below as the instructions on the right
 * 
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



#include "VirtuinoBluetooth.h"                           // Include VirtuinoBluetooth library to your code

// Code to use SoftwareSerial
#include <SoftwareSerial.h>                              //  Disable this line if you want to use hardware serial 
SoftwareSerial bluetoothSerial =  SoftwareSerial(2,3);   // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to bluetooth module TX pin   -  connect the arduino TX pin to bluetooth module RX pin.  Disable this line if you want to use hardware serial 
VirtuinoBluetooth virtuino(bluetoothSerial,9600);       // Set SoftwareSerial baud rate.  -  Disable this line if you want to use hardware serial 


// Code to use HardwareSerial
// VirtuinoBluetooth virtuino(Serial1);            // enable this line and disable all SoftwareSerial lines
                                                   // Open VirtuinoBluetooth.h file on the virtuino library folder ->  disable the line: #define BLUETOOTH_USE_SOFTWARE_SERIAL 
                                                  


                                              


//================================================================== setup
//==================================================================
//==================================================================
void setup() 
{
  virtuino.DEBUG=true;               // set this value TRUE to enable the serial monitor status
  Serial.begin(9600);                // Set serial monitor baud rate

  
  // Use virtuino.vPinMode insteed default pinMode method for digital input or digital output pins.
  virtuino.vPinMode(12,OUTPUT);    // Connect a LED to pin 12 as digital output
  virtuino.vPinMode(5,INPUT);      // Connect a switch or button to pin as digtial input 
                                   
  pinMode(10,OUTPUT);              // Connect a LED to pin 10 as analog output. It is not neccesary to use virtuino.vPinMode command for analog outputs
  
                                   // Connect a potentiometer to analog pin A0                                 
   
//------ enter your setup code below
  

 
    
    
}


//================================================================== loop
//==================================================================
//==================================================================


void loop(){
   virtuino.run();           //  neccesary command to communicate with Virtuino android app
   
    //------ enter your loop code below here
    //------ avoid to use delay() function in your code
    //------ your code .....

    
    //--------  Example 1: Write to virtual memory
    //--------  Change a virtual memory value every time Pin 5 changed
    //--------  Add a Value Display on Virtuino app to virtual memory V1
    int value1 = digitalRead(5);                          // read  the value of Pin 5
    if (value1 == HIGH)  virtuino.vMemoryWrite(1,100);   // change the virtual memory value 
    else virtuino.vMemoryWrite(1,240);                   // change the virtual memory value


    //-------- Example 2: Read a virtual memory
    //-------- Add a regulator on Virtuino app to analog virtual memory V2
    int value2 = virtuino.vMemoryRead(2);       // read analog virtual memory V2
    if (value2>122) {
      // do something
      digitalWrite(12,HIGH);
    }
    else digitalWrite(12,LOW);
    analogWrite(10,value2);                    // write to analog output

     

 }


