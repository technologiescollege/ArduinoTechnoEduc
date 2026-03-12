/* -------------------------------------------------------------------------- */
/* FILE NAME:   testDigitalTimeout.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240424
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */



#include "OptaBlue.h"

using namespace Opta;

unsigned long long start_time = 0;
/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();

  Serial.println("ALL output of Digital Expansions are turned on");

  for(int i = 0; i <OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++ ) {
    DigitalExpansion exp = OptaController.getExpansion(i);
    if(exp) {
      uint8_t M = 0;
      uint8_t m = 0;
      uint8_t r = 0;
      exp.getFwVersion(M, m, r);

      Serial.print("Digital expansion at index " +
      String(exp.getIndex()));

      Serial.print(" FW version ");
      Serial.print(M);
      Serial.print(".");
      Serial.print(m);
      Serial.print(".");
      Serial.println(r);

      exp.digitalWrite(0,HIGH);
      exp.digitalWrite(1,HIGH);
      exp.digitalWrite(2,HIGH);
      exp.digitalWrite(3,HIGH);
      exp.digitalWrite(4,HIGH);
      exp.digitalWrite(5,HIGH);
      exp.digitalWrite(6,HIGH);
      exp.digitalWrite(7,HIGH);
      exp.updateDigitalOutputs();
    }
  }
  start_time = millis();
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
 //OptaController.update();

  static long int start_m = millis();
  static bool st = true;
      /* non blocking delay, this will run every 1000 ms */
  if(millis() - start_m > 1000) {
    start_m = millis();

	 float time_elapsed = (float)(millis() - start_time) / 1000.0;
	 Serial.println("Time elapsed: " + String(time_elapsed) + " seconds");
  }

}

