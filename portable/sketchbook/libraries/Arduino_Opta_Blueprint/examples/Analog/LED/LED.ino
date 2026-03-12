/* -------------------------------------------------------------------------- */
/* FILE NAME:   LED.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240108
   DESCRIPTION: This sketch demonstrate how to use LEDs on Opta Analog when 
                controlled by Opta Controller
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#define PERIODIC_UPDATE_TIME 2 //actually not used (it's DELAY_LED that leads the timing)
#define DELAY_AFTER_SETUP 1000
#define DELAY_LED 250

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
  Serial.println("*** Opta Analog LED example ***");
   
  OptaController.begin();

}



/* -------------------------------------------------------------------------- */
void optaAnalogTask() {
/* -------------------------------------------------------------------------- */
  static bool st[OPTA_CONTROLLER_MAX_EXPANSION_NUM] = {true,true,true,true,true};
  static long int start = millis();

  static const char *msg_on = "ON";
  static const char *msg_off = "OFF";
  static char *msg_ptr = (char *)msg_on;

  if(millis() - start > PERIODIC_UPDATE_TIME) {
    start = millis();

    for(int i = 0; i < OptaController.getExpansionNum(); i++) {
      Serial.println("** Expansion " + String(i) + " status = " + String(st[i]));
      AnalogExpansion exp = OptaController.getExpansion(i);
      if(exp) {
        Serial.println("ANALOG Expansion " + String(i));
        /* exp is true only if exp is an actual 
         * AnalogExpansion and false otherwise */ 

        for(int j = 0; j < 8; j++) {
           if(st[i]) {
              msg_ptr = (char*)msg_on;
              exp.switchLedOn((uint8_t)j,false);
           }
           else {
              msg_ptr = (char*)msg_off;
              exp.switchLedOff((uint8_t)j,false);
           }
           exp.updateLeds();
           delay(250);
           Serial.print("switching LED ");
           Serial.print(j);
           Serial.print(" ");
           Serial.println(msg_ptr);

           
        }
        if(st[i]) {
           st[i] = false;
        }
        else {
           st[i] = true;
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







