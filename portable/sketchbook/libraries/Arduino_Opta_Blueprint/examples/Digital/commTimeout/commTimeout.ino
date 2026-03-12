/* -------------------------------------------------------------------------- */
/* FILE NAME:   commTimeout.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240220
   DESCRIPTION: This sketch shows the communication timeout function on 
	             Opta Digital (when communication with Digital Expansion
					 fails a callback is called)
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:       This sketch is a revision of the setDigital one with some
	             different implementation 
					 This sketch switches on and off the Output in a different way
					 on Digital Mechanical Rele expansions and Digital State Solid
					 expansion 
					 Besides it used the communication callback */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"

using namespace Opta;

DigitalMechExpansion mechExp;
DigitalStSolidExpansion stsolidExp;

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

/* this callback will be called every time an expansion does not
 * answer to an I2C transaction */
void comm_timeout_cb(int index, int code) {
	Serial.println("FAILED COMMUNICATION with expansion: " + String(index));
	Serial.println("FAILED message code: " + String(code));

	/* handle the problem ....*/
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
      mechExp = OptaController.getExpansion(i); 
      stsolidExp = OptaController.getExpansion(i);


      /* always check for the validity of the expansion */
      if(mechExp) {
			
	/* !!!!! set the communication callback */
	mechExp.setFailedCommCb(comm_timeout_cb);

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
			/* !!!!! set the communication callback */
		  stsolidExp.setFailedCommCb(comm_timeout_cb);

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


