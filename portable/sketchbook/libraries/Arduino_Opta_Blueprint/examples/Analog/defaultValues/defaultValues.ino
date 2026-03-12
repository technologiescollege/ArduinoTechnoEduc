/* -------------------------------------------------------------------------- */
/* FILE NAME:   defaultValue.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240923
   DESCRIPTION: This example shows how to set timeout and default value on Opta
                Analog. If the expansion does not receive any message from 
                the controller within the timeout, it set its output PWM and DAC
                to the default values (programmable) for safety reason.
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       This version uses static version of the function so that 
                timeout and default values can be set also if the expansion
                is not physically attached (timeout and default will be set on the
                expansion as soon as the expansion is attached to the chain)  */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"
#define PERIODIC_UPDATE_TIME 1000
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
  delay(3000);

  OptaController.begin();

  for(int device = 0; device < OPTA_CONTROLLER_MAX_EXPANSION_NUM ; device++) {
    
    /* Set the timeout 2000 = 2 seconds */
    AnalogExpansion::setTimeoutForDefaultValues(OptaController,device,2000);
    
    for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      /* Begin all channels as DAC */
      AnalogExpansion::beginChannelAsDac(OptaController,
                                           device,
                                           ch,
                                           OA_VOLTAGE_DAC,
                                           true,
                                           false,
                                           OA_SLEW_RATE_0);
      /* Set default DAC outputs */
      AnalogExpansion::setDefaultDac(OptaController, device, ch, ch * 500);
    }
     /* Set default PWM outputs */
    for(int ch = OA_PWM_CH_FIRST; ch <= OA_PWM_CH_LAST; ch++) {
      AnalogExpansion::setDefaultPwm(OptaController, device, ch,1000 + ch * 5000, 500 + ch*500);
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

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
        AnalogExpansion exp = OptaController.getExpansion(i);
        if(exp) {
          
          for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
              exp.setDac(ch, 200 + ch * 1000);
          }

          for(int ch = OA_PWM_CH_FIRST; ch <= OA_PWM_CH_LAST; ch++) {
            exp.setPwm(ch, 1000 + ch * 2500, ch * 250);
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
  //printExpansionInfo();
  optaAnalogTask();

}
