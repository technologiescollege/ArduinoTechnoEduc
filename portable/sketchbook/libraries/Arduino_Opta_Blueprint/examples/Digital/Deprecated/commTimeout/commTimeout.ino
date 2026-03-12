				/* -------------------------------------------------------------------------- */
/* FILE NAME:   commTimeout.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240220
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */  
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
  else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo() {
/* -------------------------------------------------------------------------- */  
  static long int start = millis();
  
  if(millis() - start > 5000) {
    start = millis();
    Serial.print("Number of expansions: ");
    Serial.println(OptaController.getExpansionNum());

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.print("Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(OptaController.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(OptaController.getExpansionI2Caddress(i));
    }
  }
  
}



void communication_error(int device, int code) {
  Serial.print("!!!!!!!  Failed communication on device ");
  Serial.print(device);
  Serial.print(" code ");
  Serial.println(code); 
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();
  OptaController.setFailedCommCb(communication_error);
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
  printExpansionInfo();

  static int k = 0;
  static bool switch_on = true;

  for(int i = 0; i < OptaController.getExpansionNum(); i++) {

    OptaController.update();
    if(switch_on) {
      Serial.print("ON - device ");
      Serial.print(i);
      Serial.print(" pin ");
      Serial.println(k);
      OptaController.digitalWriteOpta(i, k, HIGH, true);
    }
    else {
      OptaController.digitalWriteOpta(i, k, LOW, true);
      Serial.print("OFF - device ");
      Serial.print(i);
      Serial.print(" pin ");
      Serial.println(k);
    }
    delay(200);
  }

   k++;
   if(k == OPTA_DIGITAL_OUT_NUM) {
     k = 0;
     switch_on = !switch_on;
   }
}
