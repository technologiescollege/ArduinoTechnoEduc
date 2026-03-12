/* -------------------------------------------------------------------------- */
/* FILE NAME:   DAC_interactive.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240326
   DESCRIPTION: This example shows how to use the OptaBlue library to control
                the Opta Analog expansion. The example shows how to set the
                DAC value for each channel. Output values can be choosen
                by the user via Serial line
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



int getIntegerFromSerial() {
  /* basic function that get a integer from serial 
   * (can be improved....)*/

  int rv = -1;
  /* wait for user to write something on the serial line */
  while(!Serial.available()) {
  }
  
  /* */
  while(Serial.available()) {
    
    int num = Serial.read();
    
    if( (num >= 0x30 && num <= 0x39) ) {
      if(rv == -1) {
        rv = 0;
      }
      rv *= 10;
      rv += num - 0x30;
    }
  }
  return rv;
}

int getIntegerNonBlocking() {
   /* basic function that get a integer from serial 
   * (can be improved....)*/
  int rv = -1;
  if(Serial.available()) {
    rv = 0;
  }
  
  while(Serial.available()) {
    int num = Serial.read();
    
    if( (num >= 0x30 && num <= 0x39) ) {
      rv *= 10;
      rv += num - 0x30;
    }
  }
  return rv;
}


float getFloatFromSerial() {
  /* basic function that get a float from serial 
   * (can be improved....)*/

  float rv = 0;
  float decimal = 0;
  float dec_mult = 0.1;
  /* wait for user to write something on the serial line */
  while(!Serial.available()) {
  }
  
  bool integer_part = true;

  /* */
  while(Serial.available()) {
    
    int num = Serial.read();

    if(integer_part) {
      if( (num >= 0x30 && num <= 0x39) ) {
        rv *= 10;
        rv += (float)(num-0x30);
      }
      else if(num == '.') {
        integer_part = false;
      }
    }
    else {
      if( (num >= 0x30 && num <= 0x39) ) {
        float a = dec_mult * (float)(num-0x30);
        decimal += a;
        dec_mult /= 10;
      }
    }
  } 
  rv += decimal;
  return rv;
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  Serial.begin(115200);
  delay(3000);

  OptaController.begin();

  Serial.println("*** ANALOG expansion DAC interactive example ***");
  Serial.println();
  Serial.println("Make sure that your Opta Analog are connected and powered up!");

  int go = 1000;

  do {

    Serial.println("Select the Analog expansion to be configured [0-4] or just hit return to finish: ");
    go = getIntegerFromSerial();
    Serial.println("Selected expansion at index " + String(go));

    if(go >= 0 && go < 5) {

     AnalogExpansion e = OptaController.getExpansion(go);

     if(e) {
        Serial.println("ANALOG expansion at index " + String(go) + " found!");
        Serial.println("Please Configure DAC channel...");
        for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
            Serial.print("Select DAC type for CHANNEL " + String(ch));
            Serial.println(": 1 ->VOLTAGE    2 ->CURRENT");
            
            int sel = getIntegerFromSerial();
            Serial.print("Set CHANNEL " + String(ch));
            if(sel == 1) {
              Serial.println(" as DAC VOLTAGE");
              e.beginChannelAsVoltageDac(ch); // use default parameters
            }
            else if(sel == 2) {
              Serial.println(" as DAC CURRENT");
              e.beginChannelAsCurrentDac(ch); // use default parameters
            }
            else {
              Serial.println("ERROR: invalid choice, channel " + String(ch) + " will not be configured");
            }
        }
     }
     else {
      Serial.println("ERROR: could not find an Analog Expansion at index " + String(go));
     }
   }
  } while(go != -1);
}




/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();
  Serial.println("* Select an analog expansion: ");
  int go = getIntegerNonBlocking();
  if(go >= 0 && go < 5) {
     Serial.println(" Selected expansion at index " + String(go));
     AnalogExpansion e = OptaController.getExpansion(go);
     if(e) {
        Serial.println("* Select the channel you want to change: ");
        int ch = getIntegerFromSerial();
        Serial.println("  -> Selected channel " + String(ch));

        if(ch >= 0 && ch < OA_AN_CHANNELS_NUM) {
          if(e.isChVoltageDac(ch)) {
            Serial.println("  Enter the new value in Volts [0-11]: ");
            float v = getFloatFromSerial();
            Serial.println("  -> Entered voltage: " + String(v) + " V");

            if(v > 0 && v < 11.0) {
              Serial.println("     setting new value...");
              e.pinVoltage(ch,v);
            }
            else {
              Serial.println("ERROR: wrong value");
            }
          }
          else if(e.isChCurrentDac(ch)) {
            Serial.println("  Enter the new value in milli-Ampere [0-25]: ");
            float v = getFloatFromSerial();
            Serial.println("  -> Entered current: " + String(v) + " mA");
            if(v > 0 && v < 25.0) {
              Serial.println("      setting new value...");
              e.pinCurrent(ch,v);
            }
            else {
              Serial.println("ERROR: wrong value");
            }
          }
          else {
            Serial.println("ERROR: channel is not a DAC!");
          }
        }
     }
     else {
      Serial.println("ERROR: could not find an Analog Expansion at index " + String(go));
     }
  }

  delay(2000);
}


