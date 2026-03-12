/* -------------------------------------------------------------------------- */
/* FILE NAME:   ADC.ino
   AUTHOR:      Daniele Aimo
   DATE:        20231211
   DESCRIPTION: This sketch shows how to use Opta Analog ADC channel
		This will work only on Opta Analog Expansions
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#define PERIODIC_UPDATE_TIME 500
#define DELAY_AFTER_SETUP 5000

using namespace Opta;


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

  OptaController.begin();
  

  for(int i = 0; i < OptaController.getExpansionNum(); i++) {

    for(int k = 0; k < OA_AN_CHANNELS_NUM;k++){
      /* all channels are initialized in the same way as VOLTAGE ADC */
      AnalogExpansion::beginChannelAsAdc(OptaController, i, // the device
                            k, // the output channel you are using
			    OA_VOLTAGE_ADC, // adc type
			    true, // enable pull down
			    false, // disable rejection
			    false, // disable diagnostic
			    0); // disable averaging
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
      AnalogExpansion exp = OptaController.getExpansion(i);
      
      if(exp) {  
	      Serial.println("\nAnalog Expansion n. " +  String(exp.getIndex()));
        
        
        for(int j = 0; j < OA_AN_CHANNELS_NUM; j++) {
          Serial.print(" - ch " + String(j));
	        int value =	exp.analogRead((uint8_t)j);
	        Serial.println(" -> ADC " + String(value));
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




