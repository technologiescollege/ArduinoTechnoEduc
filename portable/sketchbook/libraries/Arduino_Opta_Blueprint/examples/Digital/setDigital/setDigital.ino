/* -------------------------------------------------------------------------- */
/* FILE NAME:   setDigital.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231116
   DESCRIPTION: This sketch shows how to set the digital values of Digital
                Expansion digital output pins 
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"

using namespace Opta;

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */  
  if(Serial) {
    if(t == EXPANSION_NOT_VALID) {
      Serial.print("Unknown!");
    }
    else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
      Serial.print("DIGITAL [Mechanical]");
    }
    else if(t == EXPANSION_OPTA_DIGITAL_STS) {
      Serial.print("DIGITAL [Solid State]");
    }
    else if(t == EXPANSION_DIGITAL_INVALID) {
      Serial.print("DIGITAL [!!Invalid!!]");
    }
    else if(t == EXPANSION_OPTA_ANALOG) {
      Serial.print("ANALOG");
    }
    else {
      Serial.print("Unknown!");
    }
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo(uint8_t index, ExpansionType_t type, uint8_t i2c_address) {
/* -------------------------------------------------------------------------- */
  if(Serial) {
    Serial.print("Expansion[" + String(index) + "]:");
    Serial.print(" type ");
    printExpansionType(type);
    Serial.print(", I2C address: ");
    Serial.println(i2c_address);
  }

}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();
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
    /* ask for a digital expansion 
     * just one of these will be a valid expansion */
    DigitalMechExpansion mechExp = OptaController.getExpansion(i); 
    DigitalStSolidExpansion stsolidExp = OptaController.getExpansion(i);
    /* always check for the validity of the expansion */
    if(mechExp) {
      /* get and print information about expansion */
      printExpansionInfo(mechExp.getIndex(), mechExp.getType(),
      mechExp.getI2CAddress());

        /* this implement 2 states in the first one 
         * pin 0 2 4 6 are turned off and pin 1 3 5 7 are turned on */
        if(st) {
          mechExp.digitalWrite(0,LOW);  //turn off pin 0
          mechExp.digitalWrite(1,HIGH); //turn on pin 1
          mechExp.digitalWrite(2,LOW);  //turn off pin 2
          mechExp.digitalWrite(3,HIGH); //turn on pin 3
          mechExp.digitalWrite(4,LOW);  //turn off pin 4
          mechExp.digitalWrite(5,HIGH); //turn on pin 5
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
          mechExp.digitalWrite(0,HIGH);  //turn off pin 0
          mechExp.digitalWrite(1,LOW); //turn on pin 1
          mechExp.digitalWrite(2,HIGH);  //turn off pin 2
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

  
    /* always check for the validity of the expansion */
    if(stsolidExp) {
      /* get and print information about expansion */
        printExpansionInfo(stsolidExp.getIndex(), stsolidExp.getType(),
        stsolidExp.getI2CAddress());

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
