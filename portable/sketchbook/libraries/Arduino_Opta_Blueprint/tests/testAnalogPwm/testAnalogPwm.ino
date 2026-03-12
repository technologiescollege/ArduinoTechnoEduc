/* -------------------------------------------------------------------------- */
/* FILE NAME:   testStressAnalog.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240605
   DESCRIPTION: This test verify that PWMs work with every other channel configuration
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

AnalogExpansion out_expansion;
AnalogExpansion in_expansion;

#define PWM_EXPANSION_INDEX 1

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   Serial.begin(115200);
   delay(5000);

   OptaController.begin();
}
 
/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
   static uint8_t configuration[5] = {0,0,0,0,0};
   static uint8_t device = 0;

   OptaController.update();
   
   AnalogExpansion pwm = OptaController.getExpansion(device);
   if(pwm) {
      
      
      for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
         if(configuration[device] == 0) {
            Serial.println("\nCONFIGURATION Voltage ADC");
            pwm.beginChannelAsVoltageAdc(ch);
         }
         else if(configuration[device] == 1) {
            Serial.println("\nCONFIGURATION Current ADC");
            pwm.beginChannelAsCurrentAdc(ch);
         }
         else if(configuration[device] == 2) {
            Serial.println("\nCONFIGURATION Current DAC");
            pwm.beginChannelAsCurrentDac(ch);
            pwm.addVoltageAdcOnChannel(ch);
         }
         else if(configuration[device] == 3) {
            Serial.println("\nCONFIGURATION Voltage DAC");
            pwm.beginChannelAsVoltageDac(ch);
            pwm.addCurrentAdcOnChannel(ch);
         }
         else if(configuration[device] == 4) {
            Serial.println("\nCONFIGURATION RTD 3 wires");
            pwm.beginChannelAsRtd(ch, true, 1.2);
            pwm.beginRtdUpdateTime(1200);
         }
         else if(configuration[device] == 5) {
            Serial.println("\nCONFIGURATION RTD 2 wires");
            pwm.beginChannelAsRtd(ch, false, 1.2);
            pwm.beginRtdUpdateTime(1200);
         }
         else if(configuration[device] == 6) {
            Serial.println("\nCONFIGURATION Digital Input");
            pwm.beginChannelAsDigitalInput(ch);
         }
         else if(configuration[device] == 7) {
            Serial.println("\nCONFIGURATION HIGH IMPEDANCE");
            pwm.beginChannelAsHighImpedance(ch);
         }
      } 
      configuration[device]++;
      if(configuration[device] >= 8){
         configuration[device] = 0;
      }
      
      Serial.println("Expansion index " + String(pwm.getIndex()));

      Serial.println("----------------- PWM ON ---------------");
      pwm.setPwm(OA_PWM_CH_0,10000,5000);
      pwm.setPwm(OA_PWM_CH_1,10000,5000);
      pwm.setPwm(OA_PWM_CH_2,10000,5000);
      pwm.setPwm(OA_PWM_CH_3,10000,5000);
      delay(5000);

      Serial.println("----------------- PWM OFF ---------------");
      pwm.setPwm(OA_PWM_CH_0,0,0);
      pwm.setPwm(OA_PWM_CH_1,0,0);
      pwm.setPwm(OA_PWM_CH_2,0,0);
      pwm.setPwm(OA_PWM_CH_3,0,0);
      delay(5000);
	}
   device = (++device)%8;
}
