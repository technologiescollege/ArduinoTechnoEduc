/* -------------------------------------------------------------------------- */
/* FILE NAME:   testRecognizeExpansions.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240605
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

/* Test recognize expansions

	Wire the maximum number of expansion
	Then reset the controller
	Every time all the expansion must be found
 */


#include "OptaBlue.h"

#define NUMBER_OF_EXPANSION 5

AnalogExpansion a;
DigitalExpansion d;

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   Serial.begin(115200);
   delay(2000);

   Serial.println("\n --------- TEST -------------");

   Serial.print(NUMBER_OF_EXPANSION);
   Serial.println(" must be connected to perform this test");

   OptaController.begin();

   for(int i = 0; i < NUMBER_OF_EXPANSION; i++) {
   	a = OptaController.getExpansion(i);
   	d = OptaController.getExpansion(i);
   	if(a || d) {
   		Serial.print("Found expansion at index " + String(i));
         if(a) {
            Serial.println(" ANALOG");
         }
         else if(d) {
            Serial.println(" DIGITAL");
         }
   	}
   	else {
   		Serial.println("!!!!!!! FAILED !!!!! ----> expansion not Found at index " + String(i));
   	}
   }

   Serial.println("Number of expansions: " + String(OptaController.getExpansionNum()));
   if(OptaController.getExpansionNum() == NUMBER_OF_EXPANSION) {
      Serial.println("       TEST PASSED");
   }
   else {
      Serial.println("       TEST failed");
   }

   Serial.println("Reset to perform the test again"); 
   
}   
   


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
   OptaController.update();
   delay(1000);
   Serial.println("Number of expansions: " + String(OptaController.getExpansionNum()));
   if(OptaController.getExpansionNum() == NUMBER_OF_EXPANSION) {
      Serial.println("       TEST PASSED");
   }
   else {
      Serial.println("       TEST failed");
   }
}

