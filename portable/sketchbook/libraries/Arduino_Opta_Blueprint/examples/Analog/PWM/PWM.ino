/* -------------------------------------------------------------------------- */
/* FILE NAME:   Pwm.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231205
   DESCRIPTION: This sketch shows basic PWM usage with the OptaBlue library.
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"


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

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(2000);

  OptaController.begin();
}

/* the optaAnalogTask function runs every 2000 ms it set the pwm for all the
 * channels from with a period equal to 10 ms and a variable duty cycle from 10%
 * to 70% */

/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
	
  static long int start = millis();
  static bool stop_pwm = false; 
  if(millis() - start > 2000) {
    if(Serial.available()) {
	while(Serial.available()) {
	    Serial.read();
	}
	stop_pwm = !stop_pwm;
    } 
    start = millis();
    static uint16_t period = 10000;
    static uint16_t pulse = 0;
    static bool rising  = 1;
    if(rising) {
       pulse += 1000;
       if(pulse > 7000) {
	 rising = 0;
       }
    } else {
       pulse -= 1000;
      if(pulse <= 1000) {
         rising = 1;
      }
    }

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {

      AnalogExpansion aexp = OptaController.getExpansion(i);

      if(aexp) {
        if(stop_pwm) {
	  Serial.println("PWM stopped");
	  aexp.setPwm(OA_FIRST_PWM_CH, 0, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 1, 0, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 2, 0, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 3, 0, pulse);
	}
	else {
	  Serial.println("PWM started");
	  aexp.setPwm(OA_FIRST_PWM_CH, period, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 1, period, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 2, period, pulse);
	  aexp.setPwm(OA_FIRST_PWM_CH + 3, period, pulse);
	}
      }
    }
    for(int i = 0; i < OptaController.getExpansionNum(); i++) {

      AnalogExpansion aexp = OptaController.getExpansion(i);

      if(aexp) {
	Serial.println("PWM ch 0 period " +
	String(aexp.getPwmPeriod(OA_FIRST_PWM_CH))+ 
	  " pulse " + aexp.getPwmPulse(OA_FIRST_PWM_CH));
	Serial.println("PWM ch 1 period " +
	String(aexp.getPwmPeriod(OA_FIRST_PWM_CH + 1)) + 
	" pulse " + aexp.getPwmPulse(OA_FIRST_PWM_CH + 1));
	Serial.println("PWM ch 2 period " +
	String(aexp.getPwmPeriod(OA_FIRST_PWM_CH + 2)) + 
	  " pulse " + aexp.getPwmPulse(OA_FIRST_PWM_CH + 2));
	Serial.println("PWM ch 3 period " +
	String(aexp.getPwmPeriod(OA_FIRST_PWM_CH + 3))+ 
	    " pulse " + aexp.getPwmPulse(OA_FIRST_PWM_CH + 2));
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

