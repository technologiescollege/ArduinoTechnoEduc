/* -------------------------------------------------------------------------- */
/* FILE NAME:   voltage_measurement.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240419
   DESCRIPTION: 
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */



#include "OptaBlue.h"

using namespace Opta;

#define UPDATE_TIME 1000


/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
  Serial.begin(115200);
  delay(2000);

  Serial.println("+ SETUP");

  OptaController.begin();
}

/* -------------------------------------------------------------------------- */
void printUint16(uint16_t v) {
/* -------------------------------------------------------------------------- */  
  if(v < 10) {
    Serial.print("    ");
  }
  else if(v < 100) {
    Serial.print("   ");

  }
  else if(v < 1000) {
    Serial.print("  ");

  }
  else if(v < 10000) {
     Serial.print(" ");
  }
  Serial.print(v);
}

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */
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

int getIntegerFromSerial() {
/* -------------------------------------------------------------------------- */
  /* basic function that get a integer from serial 
   * (can be improved....)*/
  
  /* this function returns -1 if the user just hit return without entering 
   * any number */
  int rv = -1;
  /* wait for user to write something on the serial line */
  while(!Serial.available()) {
  }
  
  /* get the number (everything is not a number is ignored) */
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
/* -------------------------------------------------------------------------- */
int getIntegerNonBlocking() {
/* -------------------------------------------------------------------------- */
   /* basic function that get a integer from serial 
   * (can be improved....)
   * it does not wait for input from serial and return -1 if*/
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
	 else if(num == 'm' || num == 'M') {
	   rv = 0x7FFFFFFF;
	 }
  }
  return rv;
}

int number_of_expansion = 0;
int last_number_of_expansion = 0;
bool print_menu_flag = true;

/* -------------------------------------------------------------------------- */
void print_menu() {
/* -------------------------------------------------------------------------- */
  if(print_menu_flag) {
	  Serial.println("### Climatic chamber tests: Input channels measurement ###");
	  Serial.println();
	  Serial.println("Number of expansions: " + String(number_of_expansion));
	  if(number_of_expansion > 0) {
		  Serial.println("List of Expansions: ");
		  for(int i = 0; i < number_of_expansion; i++) {
			  Serial.print(i);
			  Serial.print(") ");
			  printExpansionType(OptaController.getExpansionType(i));
			  Serial.println();
		  }
	  }
	  Serial.println("*** MAIN MENU: (at any moment type 'm' to see the menu)");
	  Serial.println("1. Select the ANALOG expansion to read input from"); 
	  Serial.println("2. Toggle read continuously"); 
	  Serial.println("3. Read once"); 
	  Serial.println(":> ");
	  print_menu_flag = false;
  }
}

/* -------------------------------------------------------------------------- */
void print_input_status(int exp_index) {
/* -------------------------------------------------------------------------- */
  static unsigned long start = millis() + UPDATE_TIME + 1;
  AnalogExpansion ae = OptaController.getExpansion(exp_index);
  if(millis() - start > UPDATE_TIME) {
    start = millis();
    if(ae) {
        
        for(int k = 0; k < 8; k++) {
          /* this will return the ad converter bits 
          * no need to get value from expansion since we used 
          * updateAnalogInputs() so use false as the last parameter */
          uint16_t v = ae.analogRead(k);
          Serial.print("[");
          if(k < 10) {
            Serial.print("0");
          } 
          Serial.print(String(k) + "]:");
          Serial.print(v);
          Serial.print(" ");
          /* this will return the voltage at the pin k in Volts
          * we pass false as the last argument since we have
          * updated the values with updateAnalogInputs() */
          float V = ae.pinVoltage(k);
          Serial.print("(" + String(V) + "V) ");
          if(k == 7) {
            Serial.println();
          }
        }
        Serial.println();
    }
    else {
      Serial.println("Expansion is not digital");
    }
  }
}

uint8_t selected_expansion = 255;
bool print_status_flag = false;
bool print_status_once = false;

bool configured[5] = {false,false,false,false,false};


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  OptaController.update();
 
  for(uint8_t i = 0; i < 5; i++) {
		if(configured[i] == false) {
			AnalogExpansion ae = OptaController.getExpansion(i);
			if(ae) {
				 configured[i] = true;
				 for(int i = 0; i < 8; i++) {
					ae.beginChannelAsVoltageAdc(i);
				}
			}
		}
  }




  if(number_of_expansion != OptaController.getExpansionNum()) {
     print_menu_flag = true;
  }
  number_of_expansion = OptaController.getExpansionNum();
  print_menu();
  int result = getIntegerNonBlocking();

  if(result > -1) {
		if(result == 0x7FFFFFFF) {
			print_menu_flag = true;
		}
		else if(result == 1) {
			Serial.println(" - Select expansion:> ");
			selected_expansion = getIntegerFromSerial();
			Serial.println(" -> Selected expansion " + String(selected_expansion));
			print_menu_flag = true;
		}
		else if(result == 2) {
			print_status_flag = !print_status_flag;
		}
		else if(result == 3) {
			print_status_once = true;
		}
  }

  if(selected_expansion < 5 && (print_status_flag || print_status_once)) {
	  print_status_once = false;
     print_input_status(selected_expansion);
  }
}

