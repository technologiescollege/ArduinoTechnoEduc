/* FILE NAME:   readFlash.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240124
   DESCRIPTION: This sketch production information from the flash of 
                the expansions.
                On Digital itite again
   LICENSE:     Copyright (c) 2024 Arduino SA 
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

using namespace Opta;

void printExpansionType(ExpansionType_t t) {
  if(t == EXPANSION_NOT_VALID) {
    Serial.print("Unknown!");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
    Serial.print("Opta --- DIGITAL [Mechanical]  ---");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_STS) {
    Serial.print("Opta --- DIGITAL [Solid State] ---");
  }
  else if(t == EXPANSION_DIGITAL_INVALID) {
    Serial.print("Opta --- DIGITAL [!!Invalid!!] ---");
  }
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("Opta ~~~ ANALOG ~~~ ");
  }
  else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(5000);

  OptaController.begin();
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  OptaController.update();
  
  for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
   
    DigitalExpansion dexp = OptaController.getExpansion(i);

    if(dexp) {
      Serial.print("\nExpansion n. ");
      Serial.print(dexp.getIndex());
      Serial.print(" type ");
      ExpansionType_t ex_type = dexp.getType();
      printExpansionType(ex_type);

      /*
       * READING INFORMATION FROM EXPANSION FLASH 
       */
      Serial.println();
      uint8_t buffer[32];
      uint8_t dbuf = 32;
      uint16_t add = PRODUCTION_DATA_FLASH_ADDRESS;
      Serial.print("Expansion Serial Number: ");
      dexp.getFlashData((uint8_t *)buffer,dbuf,add);
      Serial.print("Serial number: ");
      for(int i = 0;  i < 32; i++) {
        if(std::isprint(buffer[i])) {
          Serial.print((char)buffer[i]);
        }
      }
      Serial.println();
      if(ex_type == EXPANSION_OPTA_DIGITAL_MEC ||
         ex_type == EXPANSION_OPTA_DIGITAL_STS ||
         ex_type == EXPANSION_DIGITAL_INVALID) {
        add = EXPANSION_TYPE_ADDITIONA_DATA;
        dexp.getFlashData((uint8_t *)buffer,dbuf,add);
        Serial.print("Digital type: ");
        if(buffer[0] == 0) {
          Serial.println("MECHANICAL RELE");
        }
        else if(buffer[0] == 1) {
          Serial.println("STATE SOLID");
        }
        else {
          Serial.println("NOT SET");
        }
      }
    }

    AnalogExpansion aexp = OptaController.getExpansion(i);
    if(aexp) {
      Serial.print("\nExpansion n. ");
      Serial.print(aexp.getIndex());
      Serial.print(" type ");
      ExpansionType_t ex_type = aexp.getType();
      printExpansionType(ex_type);

      /*
       * READING INFORMATION FROM EXPANSION FLASH 
       */
      Serial.println("\n---- READING DATA FROM FLASH ----");
      uint8_t buffer[32];
      uint8_t dbuf = 32;
      uint16_t add = PRODUCTION_DATA_FLASH_ADDRESS;
      aexp.getFlashData((uint8_t *)buffer,dbuf,add);
      Serial.print("Serial number: ");
      for(int i = 0;  i < 32; i++) {
        if(std::isprint(buffer[i])) {
          Serial.print((char)buffer[i]);
        }
      }
      Serial.println();

    }
  }
  delay(3000);
}


