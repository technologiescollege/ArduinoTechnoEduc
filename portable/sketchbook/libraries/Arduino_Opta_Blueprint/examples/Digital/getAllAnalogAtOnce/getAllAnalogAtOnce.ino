/* -------------------------------------------------------------------------- */
/* FILE NAME:   getAnalog.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231116
   DESCRIPTION: This sketch shows how to get the analog values of Digital
                Expansion analog input pins 
                In this example all analog inputs are read all together
                and the value is used without any further interaction with
                the expansion
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
      
      /* this reads all analog input value from expansion at once */
      mechExp.updateAnalogInputs();

      /* get and print information about analog input status */
      for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
        /* this will return the ad converter bits 
         * no need to get value from expansion since we used 
         * updateAnalogInputs() so use false as the last parameter */
        uint16_t v = mechExp.analogRead(k, false);
        Serial.print("[");
        if(k < 10) {
          Serial.print("0");
        } 
        Serial.print(String(k) + "]:");
        printUint16(v);
        Serial.print(" ");
        /* this will return the voltage at the pin k in Volts
         * we pass false as the last argument since we have
         * updated the values with updateAnalogInputs() */
        float V = mechExp.pinVoltage(k,false);
        Serial.print("(" + String(V) + "V) ");
        if(k == 7) {
          Serial.println();
        }
      }
      Serial.println();
    }

  
    /* always check for the validity of the expansion */
    if(stsolidExp) {
      /* get and print information about expansion */
        printExpansionInfo(stsolidExp.getIndex(), stsolidExp.getType(),
        stsolidExp.getI2CAddress());

      /* this reads all analog input value from expansion at once */
      stsolidExp.updateAnalogInputs();
        /* this will return the ad converter bits 
         * no need to get value from expansion since we used 
         * updateAnalogInputs() so use false as the last parameter */
      for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
        uint16_t v = stsolidExp.analogRead(k,false);
        Serial.print("[" + String(k) + "]:");
        printUint16(v);
        Serial.print(" ");
        /* this will return the voltage at the pin k in Volts
         * we pass false as the last argument since we have
         * updated the values with updateAnalogInputs() */
        float V = stsolidExp.pinVoltage(k,false);
        Serial.print("(" + String(V) + "V) ");
        if(k == 7) {
          Serial.println();
        }
      }
      Serial.println();
    }
  }
  delay(1000);

}
