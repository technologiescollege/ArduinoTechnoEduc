/* Virtuino example No4 (Bluetooth)
 * Example name = "Read - write to Virtuino digital virtual memory via bluetooth"
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

    

    //--------  Example 1: Write to digital virtual memory
    //--------  Add a LED on Virtuino app to virtual memory DV1

    int value=digitalRead(5);                           // read  the value of Pin 5.

    //--- do something
    if (value == HIGH) { 
        virtuino.vDigitalMemoryWrite(1,LOW);            // disable the virtual memory value DV1. 
    }   
    else {
      virtuino.vDigitalMemoryWrite(1,HIGH);             // enable the virtual memory value DV1
    }






    //--------  Example 2: Write to digital virtual memory
    //--------  Add three LEDs on Virtuino app to digital virtual memories DV5,DV6,DV7

    int analogValue =  analogRead(0);                 // read  value of analog pin A0 - potentiometer
    
    //--- do something
    if (analogValue<341) {
        virtuino.vDigitalMemoryWrite(5,1);                 // enable the virtual memory value DV5. 
        virtuino.vDigitalMemoryWrite(6,0);                 // disable the virtual memory value DV6. 
        virtuino.vDigitalMemoryWrite(7,0);                 // disable the virtual memory value DV7. 
    }
    else 
    if ((analogValue>=341) && (analogValue<682)) {
        virtuino.vDigitalMemoryWrite(5,0);                 // disable the virtual memory value DV5. 
        virtuino.vDigitalMemoryWrite(6,1);                 // enable the virtual memory value DV6. 
        virtuino.vDigitalMemoryWrite(7,0);                 // disable the virtual memory value DV7. 
    }
    else {
        virtuino.vDigitalMemoryWrite(5,0);                 // disable the virtual memory value DV5. 
        virtuino.vDigitalMemoryWrite(6,0);                 // disable the virtual memory value DV6. 
        virtuino.vDigitalMemoryWrite(7,1);                 // enable the virtual memory value DV7. 
    }



    //--------  Example 3: Read digital virtual memory
    //--------  Add a switch on Virtuino app to digital virtual memory DV20

    int  digitalMemoryValue =  virtuino.vDigitalMemoryRead(20); // read  value of analog pin A0 - potentiometer
    
    //--- do something
    if (digitalMemoryValue==0) {
      digitalWrite(12,LOW);                   // disable pin 12
      analogWrite(10,50);                     // write a value to analog output
    }
      else {
        digitalWrite(12,HIGH);
        analogWrite(10,200);
      }
    
 }


