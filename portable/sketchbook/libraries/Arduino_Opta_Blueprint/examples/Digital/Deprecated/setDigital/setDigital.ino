/* -------------------------------------------------------------------------- */
/* FILENAME: setDigital.ino
   AUTHOR: Daniele Aimo
   DATE: 20231022
   REVISION: 0.0.1
   DESCRIPTION: set digital output to Opta digital
   PRODUCT: Opta
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
  printExpansionInfo();
  for(int i = 0; i < OptaController.getExpansionNum(); i++) {
    if(OptaController.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_MEC
       || OptaController.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_STS) {
      if(Serial) {
        Serial.print("Ex n. ");
        Serial.print(i);
        Serial.print(" | Switching  on pin: ");
      }
       
      for(int k = 0; k < DIGITAL_OUT_NUM; k++) {
        if(Serial) {
          Serial.print(k);
          Serial.print(" ");
        }
        OptaController.digitalWriteOpta(i, k, HIGH);
        OptaController.updateDigitalsOut(i);
        delay(500);
      }

      if(Serial) {
        Serial.println();
        Serial.print("Ex n. ");
        Serial.print(i);
        Serial.print(" | Switching off pin: ");
      }

      for(int k = 0; k < DIGITAL_OUT_NUM; k++) {
        if(Serial) {
          Serial.print(k);
          Serial.print(" ");
        }
        OptaController.digitalWriteOpta(i, k, LOW);
        OptaController.updateDigitalsOut(i);
        delay(500);
      }
      if(Serial) {
        Serial.println();
      }

    }
  }
}
