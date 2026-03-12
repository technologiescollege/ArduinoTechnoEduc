/* -------------------------------------------------------------------------- */
/* FILENAME:    getExpansions.ino
   AUTHOR:      Daniele Aimo
   DATE:        20231020
   REVISION:    0.0.1
   DESCRIPTION: Retrieve list of expansions chained to Opta Controller and their
                type
   PRODUCT:     Arduino Opta
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */
  if(t == EXPANSION_NOT_VALID) {
    Serial.print("Unknown! (Might be an Unregistered custom expansion?)");
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
  delay(2000);

  OptaController.begin();
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  /* always call update as fast as possible */
  OptaController.update();
  
  Serial.print("Number of expansions: ");
  /* get the number of expansion */
  Serial.println(OptaController.getExpansionNum());
  /* for each expansion get and print the type and the I2C address of each 
   * expansion 
   *
   * NOTE: remember that getExpansionNum() return the number of expansion 
   * discovered when I2C addresses are assigned and will not be
   * update if you disconnect an expansion until a new assign address is
   * performed */
  for(int i = 0; i < OptaController.getExpansionNum(); i++) {
    Serial.print("Expansion n. ");
    Serial.print(i);
    Serial.print(" type ");
    printExpansionType(OptaController.getExpansionType(i));
    Serial.print(" I2C address ");
    Serial.println(OptaController.getExpansionI2Caddress(i));
  }
  delay(2000);
}
