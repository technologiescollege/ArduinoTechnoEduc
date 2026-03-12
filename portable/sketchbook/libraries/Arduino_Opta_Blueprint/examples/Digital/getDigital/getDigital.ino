/* -------------------------------------------------------------------------- */
/* FILE NAME:   getDigital.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231116
   DESCRIPTION: This sketch shows how to get the digital values of Digital
                Expansion digital input pins 
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

/* -------------------------------------------------------------------------- */
void printExpansionInfo(uint8_t index, ExpansionType_t type, uint8_t i2c_address) {
/* -------------------------------------------------------------------------- */
  Serial.print("Expansion[" + String(index) + "]:");
  Serial.print(" type ");
  printExpansionType(type);
  Serial.print(", I2C address: ");
  Serial.println(i2c_address);

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
void printUint16(uint16_t v) {
/* -------------------------------------------------------------------------- */  
  if(v < 10) {
    Serial.print("    ");
  }
  else if(v < 100) {
    Serial.print("   ");

  }
  else if(v < 1000) {
    Serial.print("  ");

  }
  else if(v < 10000) {
     Serial.print(" ");
  }
  Serial.print(v);
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
 OptaController.update();

  Serial.println();

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
      
      /* this will update the status of all the digital input 
       * the same can be achieved using digitalRead() function
       * with the second parameter equal true, however in this
       * case multiple i2c transaction are performed without
       * reason */
      mechExp.updateDigitalInputs();

      for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
        /* this will return the pin status of the pin k */
        PinStatus v = mechExp.digitalRead(k);

        if(v == HIGH) {
          Serial.print("HH");
        }
        else {
          Serial.print("LL");
        }
        Serial.print(' ');
      }
      Serial.println();
    }

  
    /* always check for the validity of the expansion */
    if(stsolidExp) {
      /* get and print information about expansion */
        printExpansionInfo(stsolidExp.getIndex(), stsolidExp.getType(),
        stsolidExp.getI2CAddress());

      /* this will update the status of all the digital input 
       * the same can be achieved using digitalRead() function
       * with the second parameter equal true, however in this
       * case multiple i2c transaction are performed without
       * reason */
      stsolidExp.updateDigitalInputs();

      for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
        /* this will return the pin status of the pin k */
        PinStatus v = stsolidExp.digitalRead(k);

        if(v == HIGH) {
          Serial.print("HH");
        }
        else {
          Serial.print("LL");
        }
        Serial.print(' ');
      }
      Serial.println();
    }
  }
  delay(1000);

}
