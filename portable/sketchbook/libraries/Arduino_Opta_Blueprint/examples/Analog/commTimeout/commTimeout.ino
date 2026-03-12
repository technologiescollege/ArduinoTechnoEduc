/* -------------------------------------------------------------------------- */
/* FILE NAME:   commTimeout.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240424
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"

using namespace Opta;

AnalogExpansion aexp;


/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */  
  if(Serial) {
    if(t == EXPANSION_NOT_VALID) {
      Serial.print("Unknown!");
    }
    else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
      Serial.print("DIGITAL [Mechanical]");
    }
    else if(t == EXPANSION_OPTA_DIGITAL_STS) {
      Serial.print("DIGITAL [Solid State]");
    }
    else if(t == EXPANSION_DIGITAL_INVALID) {
      Serial.print("DIGITAL [!!Invalid!!]");
    }
    else if(t == EXPANSION_OPTA_ANALOG) {
      Serial.print("ANALOG");
    }
    else {
      Serial.print("Unknown!");
    }
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo(uint8_t index, ExpansionType_t type, uint8_t i2c_address) {
/* -------------------------------------------------------------------------- */
  if(Serial) {
    Serial.print("Expansion[" + String(index) + "]:");
    Serial.print(" type ");
    printExpansionType(type);
    Serial.print(", I2C address: ");
    Serial.println(i2c_address);
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

/* this callback will be called every time an expansion does not
 * answer to an I2C transaction */
void comm_timeout_cb(int index, int code) {
	Serial.println("FAILED COMMUNICATION with expansion: " + String(index));
	Serial.println("FAILED message code: " + String(code));

	/* handle the problem ....*/
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
 OptaController.update();

  static long int start_m = millis();
  
      /* non blocking delay, this will run every 1000 ms */
  if(millis() - start_m > 1000) {
    start_m = millis();

    for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
      
        aexp = OptaController.getExpansion(i);

      if(aexp) {
        aexp.setFailedCommCb(comm_timeout_cb);

        uint16_t v = aexp.analogRead(0);
        Serial.println("Value " + String(v));

      }

    }
  }
}



