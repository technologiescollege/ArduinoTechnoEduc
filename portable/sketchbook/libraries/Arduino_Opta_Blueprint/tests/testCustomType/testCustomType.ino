/* -------------------------------------------------------------------------- */
/* FILENAME:    testCustomType.ino
   AUTHOR:      Daniele Aimo
   DATE:        20240905
   REVISION:    0.0.1
   DESCRIPTION: Test custom expansion type assignment 
   PRODUCT:     Arduino Opta
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#include "R4DisplayExpansion.h"


/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */
  if(t == EXPANSION_NOT_VALID) {
    Serial.print("TYPE NOT VALID (perhaps custom expansion has not been registered?)");
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
  else if(t == OptaController.getExpansionType(R4DisplayExpansion::getProduct()))
  {
     Serial.print("*** UnoR4 Display ***");
  }
  else if(t == OptaController.getExpansionType("ARDUINO UNO R4 WIFI"))
  {
     Serial.print("+++ UnoR4 WIFI ++++");
  }
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("Opta ~~~ ANALOG ~~~ ");
  }
  else {
    Serial.print("Unknown!");
  }
}

std::string debug_getProduct() {
  std::string rv("ARDUINO UNO R4 WIFI");
  return rv;
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);
  
  int type = OptaController.registerCustomExpansion(R4DisplayExpansion::getProduct(),
                                         R4DisplayExpansion::makeExpansion,
                                         R4DisplayExpansion::startUp);

  Serial.println("Custom expansion type for R4 display " + String(type));
  type = OptaController.registerCustomExpansion(debug_getProduct(),
                                         R4DisplayExpansion::makeExpansion,
                                         R4DisplayExpansion::startUp);
  Serial.println("Custom expansion type for R4 WIFI " + String(type));

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
