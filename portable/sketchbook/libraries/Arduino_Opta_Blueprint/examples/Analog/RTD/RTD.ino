/* -------------------------------------------------------------------------- */
/* FILE NAME:   RTD.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231222
   DESCRIPTION: This example shows how to use RTD on Opta Analog expansion
                driven by Opta OptaController.
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       In case of more than one Opta Analog this sketch works on the
                first one (the one closest to the OptaController)                 */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#define PERIODIC_UPDATE_TIME 2000
#define DELAY_AFTER_SETUP 1000

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

  int8_t oa_index = -1;
/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);
  Serial.println("*** Opta Analog RTD example ***");
	
  OptaController.begin();

  for(int i = 0; i < OptaController.getExpansionNum(); i++) {

    for(int k = 0; k < OA_AN_CHANNELS_NUM;k++){
      /* all channels are initialized in the same way as RTD */
      AnalogExpansion::beginChannelAsRtd(OptaController, i, // the device
                            k, // the output channel you are using
			    true, // use 3 wire RTD
			    1.2); // current used on RTD in mA
      /* set the sampling time for RTD to 1000 ms (btw this is the default) */
      AnalogExpansion::beginRtdUpdateTime(OptaController, i,
                                 1000);
    }
  }
}

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
	
  static long int start = millis();
  if(millis() - start > PERIODIC_UPDATE_TIME) {
    start = millis();

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      AnalogExpansion aexp = OptaController.getExpansion(i);
      if(aexp) {
	Serial.println("Expansion n. " + String(aexp.getIndex()));
	for(int j = 0; j < 8; j++) {
	   float value = aexp.getRtd((uint8_t)j);
	   Serial.print("ch ");
	   Serial.print(j);
	   Serial.print(" -> ");
	   Serial.print(value);
	   Serial.print(" ");
	}
	Serial.println();
      }
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
  //printExpansionInfo();
  optaAnalogTask();

}



