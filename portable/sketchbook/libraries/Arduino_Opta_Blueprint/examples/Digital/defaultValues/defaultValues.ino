/* -------------------------------------------------------------------------- */
/* FILE NAME:   defaultValues.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240228
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */




#include "OptaBlue.h"

using namespace Opta;


/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();

  DigitalExpansion::setDefault(OptaController,0,DigitalExpansion::calcDefault(true,true,false,false,false,false,
  false,false),4000);
  DigitalExpansion::setDefault(OptaController,1,DigitalExpansion::calcDefault(false,false,true,
  true,false,false,false,false),3000);
  DigitalExpansion::setDefault(OptaController,2,DigitalExpansion::calcDefault(true,false,false,
  false,true,true,false,false),2000);
  DigitalExpansion::setDefault(OptaController,3,DigitalExpansion::calcDefault(false,false,false,
  false,false,false,true,true),1000);
  DigitalExpansion::setDefault(OptaController,4,DigitalExpansion::calcDefault(true,false,false,
  true,false,false,false,true),5000);
  delay(1000);

  
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
 OptaController.update();

  static long int start_m = millis();
  static bool st = true;
      /* non blocking delay, this will run every 1000 ms */
  if(millis() - start_m > 500) {
    start_m = millis();

  for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    /* always check for the validity of the expansion */
	 DigitalMechExpansion mechExp = OptaController.getExpansion(i);
    if(mechExp) {

        /* this implement 2 states in the first one 
         * pin 0 2 4 6 are turned off and pin 1 3 5 7 are turned on */
        if(st) {
          mechExp.digitalWrite(0,HIGH);  //turn off pin 0
          mechExp.digitalWrite(1,LOW); //turn on pin 1
          mechExp.digitalWrite(2,LOW);  //turn off pin 2
          mechExp.digitalWrite(3,LOW); //turn on pin 3
          mechExp.digitalWrite(4,LOW);  //turn off pin 4
          mechExp.digitalWrite(5,LOW); //turn on pin 5
          mechExp.digitalWrite(6,LOW);  //turn off pin 6
          mechExp.digitalWrite(7,HIGH); //turn on pin 7

          /* once all pin are set send the new status to the
           * expansion */
          mechExp.updateDigitalOutputs();

          /* pin status can be sent to the expansion also setting the 
           * last parameter of digitalWrite to true (default is false) 
           * however this involves a lot of unnecessary I2C transaction */
        }
        else {
        /* in the second state 
         * pin 0 2 4 6 are turned on and pin 1 3 5 7 are turned off */
          mechExp.digitalWrite(0,LOW);  //turn off pin 0
          mechExp.digitalWrite(1,LOW); //turn on pin 1
          mechExp.digitalWrite(2,LOW);  //turn off pin 2
          mechExp.digitalWrite(3,LOW); //turn on pin 3
          mechExp.digitalWrite(4,HIGH);  //turn off pin 4
          mechExp.digitalWrite(5,LOW); //turn on pin 5
          mechExp.digitalWrite(6,HIGH);  //turn off pin 6
          mechExp.digitalWrite(7,LOW); //turn on pin 7

          /* once all pin are set send the new status to the
           * expansion */
          mechExp.updateDigitalOutputs();

          /* pin status can be sent to the expansion also setting the 
           * last parameter of digitalWrite to true (default is false) 
           * however this involves a lot of unnecessary I2C transaction */
        }
    }

  
	 DigitalStSolidExpansion stsolidExp = OptaController.getExpansion(i);
    /* always check for the validity of the expansion */
    if(stsolidExp) {

        /* if present state solid expansion will use a different pattern */
        if(st) {
          stsolidExp.digitalWrite(0,HIGH);  
          stsolidExp.digitalWrite(1,LOW);
          stsolidExp.digitalWrite(2,LOW); 
          stsolidExp.digitalWrite(3,HIGH);
          stsolidExp.digitalWrite(4,HIGH);
          stsolidExp.digitalWrite(5,LOW);
          stsolidExp.digitalWrite(6,LOW); 
          stsolidExp.digitalWrite(7,HIGH);

          /* once all pin are set send the new status to the
           * expansion */
          stsolidExp.updateDigitalOutputs();

          /* pin status can be sent to the expansion also setting the 
           * last parameter of digitalWrite to true (default is false) 
           * however this involves a lot of unnecessary I2C transaction */
        }
        else {
        /* in the second state 
         * pin 0 2 4 6 are turned on and pin 1 3 5 7 are turned off */
          stsolidExp.digitalWrite(0,LOW); 
          stsolidExp.digitalWrite(1,HIGH);
          stsolidExp.digitalWrite(2,HIGH); 
          stsolidExp.digitalWrite(3,LOW); 
          stsolidExp.digitalWrite(4,LOW); 
          stsolidExp.digitalWrite(5,HIGH); 
          stsolidExp.digitalWrite(6,HIGH); 
          stsolidExp.digitalWrite(7,LOW);

          /* once all pin are set send the new status to the
           * expansion */
          stsolidExp.updateDigitalOutputs();
        }
    } // if(stsolidExp[i]) {
  }
  if(st) {
    st = false;
  }
  else {
    st = true;
  }
 }
}
