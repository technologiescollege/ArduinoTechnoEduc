#include "Arduino.h"
#include "AdvancedSerial.h"

//Instantiate a new advanced serial object
AdvancedSerial AdvSerial(&Serial, 500);

//signals
float sine_value = 0;
float cosine_value = 0;
float square_value = -5;

void setup(){
    //Initialize Serial port
    Serial.begin(9600);

    //Add signals to Advanced Serial which will be transmitted
    AdvSerial.addSignal("sine",&sine_value);
    AdvSerial.addSignal("cosine",&cosine_value);
    AdvSerial.addSignal("square",&square_value);
}
unsigned long lastupdate = 0;
unsigned long lastSquareTransition = 0;

void loop(){
  //Update the calculated cosine and sine values;
  sine_value = 2.0 * sin( 0.5*3.1415*((double)(millis()))/1000);
  cosine_value = cos( 0.25*3.1415*((double)(millis()))/1000);

  if((millis() - lastSquareTransition) > 1000){
      lastSquareTransition = millis();
      square_value = -1*square_value;
  }
  
  //Print an ascii string once per second.
  if((millis() - lastupdate) > 1000){
      lastupdate = millis();
      Serial.print("Advanced Serial Test.  Current MS:");
      Serial.println(lastupdate);
  }

  //Execute the advanced serial code
  AdvSerial.exec();
}

