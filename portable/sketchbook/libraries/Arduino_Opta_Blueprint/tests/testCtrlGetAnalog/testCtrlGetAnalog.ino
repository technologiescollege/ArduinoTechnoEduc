/* -------------------------------------------------------------------------- */
/* FILE NAME:   getAnalog.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240228
   DESCRIPTION: This will test analog value via controller interface (deprecated 
                 but used)
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"

Controller controller; 

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
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("Opta ~~~ ANALOG ~~~");
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
    Serial.println(controller.getExpansionNum());

    for(int i = 0; i < controller.getExpansionNum(); i++) {
      Serial.print("Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(controller.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(controller.getExpansionI2Caddress(i));
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  controller.begin();

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

void wait_for_serial() {
  while(!Serial.available()) {}
  while(Serial.available()) {Serial.read();}
} 


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
 controller.update();
 int test_failed = 0;

  printExpansionInfo();

  for(int i = 0; i < controller.getExpansionNum(); i++) {
    if(controller.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_MEC ||
       controller.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_STS) {
      Serial.print("Ex n. ");
      Serial.print(i);
      Serial.print(" ");
      for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
        /* analogReadOpta() using default value for the third argument (true) will
           read the analog value from Opta Digital (only that single analog value)*/

        Serial.println("Put 24 V on pin " +  String(k) + " and write a char on the serial");
        wait_for_serial();
        Serial.println("Reading pin " + String(k) + " as analog...");
        uint16_t v = controller.analogReadOpta(i, k);
        printUint16(v);

        if(v > 55000) {
          Serial.println(" OK");
        }
        else {
          Serial.println(" FAILED!");
          test_failed++;
        }

        Serial.println("Put 5 V on pin " +  String(k) + " and write a char on the serial");
        wait_for_serial();
        Serial.println("Reading pin " + String(k) + " as analog...");
        v = controller.analogReadOpta(i, k);
        printUint16(v);

        if(v > 10400 && v < 10800) {
          Serial.println(" OK");
        }
        else {
          Serial.println(" FAILED!");
          test_failed++;
        }

        Serial.println("Put 3.3.3.3 V on pin " +  String(k) + " and write a char on the serial");
        wait_for_serial();
        Serial.println("Reading pin " + String(k) + " as analog...");
        v = controller.analogReadOpta(i, k);
        printUint16(v);

        if(v > 7200 && v < 7600) {
          Serial.println(" OK");
        }
        else {
          Serial.println(" FAILED!");
          test_failed++;
        }
      }
    }
  }

  Serial.println("TEST FINISHED!");
  if(test_failed > 0) {
    Serial.println("TEST FAILED!");
  }
  else {
    Serial.println("TEST PASSED");
  }
  

}
