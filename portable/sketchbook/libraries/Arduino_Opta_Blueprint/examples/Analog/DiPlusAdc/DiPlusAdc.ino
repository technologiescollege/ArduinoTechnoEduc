/* -------------------------------------------------------------------------- */
/* FILE NAME:   DiPlusAdc.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240308
   DESCRIPTION: This sketch uses an Opta Controller to configure
	        Each channel of an Analog Expansion as digital input
                and add on each channel a voltage ADC
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       This sketch only uses the first OA found                      */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

#define PERIODIC_UPDATE_TIME_ANALOG 2000
 
#define DAC_VALUE_NUM 10

#define INC_DAC_INDEX(x) (x = ((++x) % DAC_VALUE_NUM))

void setupAnalogExpansion(AnalogExpansion &aexp) {
   for(int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
      aexp.beginChannelAsDigitalInput(i, // channel
                                      true, // filter
                                      false, // invert output
                                      true, // simple debounce
                                      OA_DI_SINK_1, // sink current
                                      OA_DI_DEB_TIME_10, // debouncing time 
                                      false, // scale with vcc
                                      3.8, // threshold
                                      16); // vcc (ignored if scale if false)
      aexp.addAdcOnChannel(i, //channel
                          OA_VOLTAGE_ADC,
                          true, // use pull down
                          true, // use rejection
                          false, // use diagnostic
                          0); // no moving average
   }
}

void updateAnalogExpansion() {
  
   Serial.println("\n*** OPTA ANALOG expansion ");
   
   AnalogExpansion aexp = OptaController.getExpansion(0);
   
   if(!aexp) {
     return;
   }

   for(int i = 0; i < 8; i++) {
         int d = aexp.digitalRead(i,true);
         Serial.print("   - Channel ");
         Serial.print(i); 
         Serial.print(" digital status: " + String(d));
         float va = aexp.pinVoltage(i,true);
         Serial.println(" ADC " + String(va) + " Volts");
            
   }
}




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
  
  if(millis() - start > 30000) {
    start = millis();
    Serial.print("\n*** Number of expansions: ");
    Serial.println(OptaController.getExpansionNum());

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.print("    Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(OptaController.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(OptaController.getExpansionI2Caddress(i));
    }
  }  
}

int8_t oa_index_1 = -1;

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);

  Serial.println("*** Opta example Digital Input plus ADC ***");

  /* always call begin before any other OptaController function */
  OptaController.begin();
  
  /* wait until some expansion is found */
  while(OptaController.getExpansionNum() == 0) {
    Serial.println("Looking for expansions...");
    OptaController.update(); // call update to look for new expansion
    delay(1000);
  }

  /* analog expansion must be in position 0 */
  AnalogExpansion aexp = OptaController.getExpansion(0);

  if(aexp) {
     setupAnalogExpansion(aexp);
  }
  else {
    while(1) {
      Serial.println("NO Opta Analog found at position 0... looping forever...");
      delay(2000);
    }
  }
  
}
 

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
   
  static long int start = millis();
  if(millis() - start > PERIODIC_UPDATE_TIME_ANALOG) {
    start = millis();
    updateAnalogExpansion();
  }
}




/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
  printExpansionInfo();
  optaAnalogTask();
}
