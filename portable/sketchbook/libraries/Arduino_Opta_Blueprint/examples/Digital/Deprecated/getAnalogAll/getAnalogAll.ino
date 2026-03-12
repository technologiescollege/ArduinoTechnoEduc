/* -------------------------------------------------------------------------- */
/* FILENAME: getAnalogAll.ino
   AUTHOR: Daniele Aimo
   DATE: 20231020
   REVISION: 0.0.1
   DESCRIPTION: get analog status from Opta digital reading all input together
   PRODUCT: Opta
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
/* REVISION LIST:
   
   DATE:
   AUTHOR:
   CHANGES:                                                                   */
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
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("~~~ Opta  ANALOG ~~~");
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
  printExpansionInfo();
  for(int i = 0; i < OptaController.getExpansionNum(); i++) {
     if(OptaController.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_MEC ||
        OptaController.getExpansionType(i) == EXPANSION_OPTA_DIGITAL_STS) {
        /* this function updates all the analog values (16) from opta digital 
           at once */
        OptaController.updateAnalogsIn(i);

        Serial.print("Ex n. ");
        Serial.print(i);
        Serial.print(" ");
            
        for(int k = 0; k < OPTA_DIGITAL_IN_NUM; k++) {
          /* using the analogReadOpta with false as third argument gets the value
             from the local copy without updating the value with an I2C transaction
             (the local values are update all at once with the updateAnalogsIn()
             function */
          uint16_t v = OptaController.analogReadOpta(i, k, false);
          printUint16(v);
          Serial.print(" ");
        }
        Serial.println();
     }
  }
  delay(1000);

}
