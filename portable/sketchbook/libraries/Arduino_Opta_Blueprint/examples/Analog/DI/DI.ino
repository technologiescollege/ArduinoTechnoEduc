/* -------------------------------------------------------------------------- */
/* FILE NAME:   DI.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231222
   DESCRIPTION: This example shows how to use the OptaBlue library to read
		the digital inputs of an Opta expansion board from the Opta
		OptaController. 
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
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
  Serial.println("*** Opta Analog Digital Input example ***");
	
  OptaController.begin();

  for(int i = 0; i < OptaController.getExpansionNum(); i++) {

    for(int k = 0; k < OA_AN_CHANNELS_NUM;k++){
      /* all channels are initialized in the same way as VOLTAGE ADC */
      AnalogExpansion::beginChannelAsDigitalInput(OptaController, i, // the device
                            k, // the output channel you are using
			    true, // filter comparator
                            false,            // invert comparator
                            true,             // use simple debounce algorithm
                            OA_DI_SINK_1,     // sink 120 uA
                            OA_DI_DEB_TIME_5, // ~ 42 ms
                            false,            // use fix threshold
                            8.0,              // threshold at 8 V
                            24.0);            // unused in this c    }
    } 
  }

}

/* the optaAnalogTask function runs every 2000 ms it set the pwm for all the
 * channels from with a period equal to 10 ms and a variable duty cycle from 10%
 * to 70% */

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
	
  static long int start = millis();
  static bool stop_pwm = false; 
  if(millis() - start > PERIODIC_UPDATE_TIME) {
    start = millis();

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      AnalogExpansion exp = OptaController.getExpansion(i);
      if(exp) {
	for(int j = 0; j < 8; j++) {
	   int value =  exp.digitalRead((uint8_t)j,true);
	   Serial.print("DI channel ");
	   Serial.print(j);
	   Serial.print(" value ");
	   Serial.println(value);
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
  optaAnalogTask();

}




