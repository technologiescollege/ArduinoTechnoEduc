/* -------------------------------------------------------------------------- */
/* FILE NAME:   DAC.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231211
   DESCRIPTION: This example shows how to use the OptaBlue library to control
                the Opta Analog expansion. The example shows how to set the
                DAC value for each channel. Odd channels are configured as
                current DACs and even channels are configured as voltage DACs.
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"
#define PERIODIC_UPDATE_TIME 5000
#define DELAY_AFTER_SETUP 200

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

  for(int device = 0; device < OptaController.getExpansionNum(); device++) {
  
    for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {

      if(ch%2 == 0) {
        /* even channels are configured as current DAC */
        AnalogExpansion::beginChannelAsDac(OptaController,
                                           device,
                                           ch,
                                           OA_CURRENT_DAC,
                                           true,
                                           false,
                                           OA_SLEW_RATE_0);
      }
      else {
        /* odd channel are configured as voltage DAC */
        AnalogExpansion::beginChannelAsDac(OptaController,
                                           device,
                                           ch,
                                           OA_VOLTAGE_DAC,
                                           true,
                                           false,
                                           OA_SLEW_RATE_0);
      }
    }
  }
}


/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
	
  static long int start = millis();

  /* using this the code inside the if will run every PERIODIC_UPDATE_TIME ms
     assuming the function is called repeteadly in the loop() function */

  if(millis() - start > PERIODIC_UPDATE_TIME) {
    start = millis();

    /* this implement a 2 states state machine 
       in the rising state the values of all the dac are increased
       of 1000 "bits"
       in the falling state the values of all dac channel are decreased
       of 1000 "bits" 
       we change state from rising to falling when bit are greater than 6000
       and we go back to rising state when bit are lower than 1000*/
    
    static uint16_t dac_value = 0;
    static bool rising  = 1;
    if(rising) {
       /* RISIGN STATE*/
       dac_value += 1000;
       if(dac_value > 6000) {
			    /* go in falling state */
          rising = 0;
       }
    } else {
      /* FALLING STATE */
       dac_value -= 1000;
      if(dac_value <= 1000) {
         /* go in rising state */
         rising = 1;
      }
    }
    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
        AnalogExpansion exp = OptaController.getExpansion(i);
        if(exp) {
          Serial.println("Setting dac value " + String(dac_value) + " on expansion n. " + String(exp.getIndex()));
          for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
              exp.setDac(ch, dac_value);
          }
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
  printExpansionInfo();
  optaAnalogTask();

}


