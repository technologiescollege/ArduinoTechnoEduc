/* -------------------------------------------------------------------------- */
/* FILE NAME:   defaultValueV2.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240926
   DESCRIPTION: This example shows how to set timeout and default value on Opta
                Analog. PWM values and DAC are set up at the beginning together
                with the default values. A timeout in the communication will
                cause the output to go to their default. When the communication
                starts again the outputs are set to the values they had before
                the timeout.
                The purpose of this example is to show as output values can
                change depending on the presence of the I2C communication (without
                any actual "set" performed by user program)
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:       This version uses "synchronous"version of the function so that 
                timeout and default values can be set only if the expansion
                are already physically attached (timeout and default will be set on the
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
    
    AnalogExpansion exp = OptaController.getExpansion(device);

    if(exp) {
    /* Set the timeout 2000 = 2 seconds */
      exp.setTimeoutForDefaultValues(5000);
    
      for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
        /* Begin all channels as DAC */
        exp.beginChannelAsDac(ch,
                              OA_VOLTAGE_DAC,
                              true,
                              false,
                              OA_SLEW_RATE_0);
        /* Set default DAC outputs */
        exp.setDefaultDac(OptaController, device, ch, ch * 500);
        exp.pinVoltage(ch, (float)(1.0 + ch * 0.5));
      }
       /* Set default PWM outputs */
      for(int ch = OA_PWM_CH_FIRST; ch <= OA_PWM_CH_LAST; ch++) {
         exp.setDefaultPwm(OptaController, device, ch,1000 + ch * 5000, 500 + ch*500);
         exp.setPwm(ch, 1000 + ch * 2500, ch * 250);
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
  for(int device = 0; device < OPTA_CONTROLLER_MAX_EXPANSION_NUM ; device++) {
    AnalogExpansion exp = OptaController.getExpansion(device);
    if(exp) {
      exp.isChDac(0,true); //just keep communication alive
    }
    delay(1000);
  }
 

}
