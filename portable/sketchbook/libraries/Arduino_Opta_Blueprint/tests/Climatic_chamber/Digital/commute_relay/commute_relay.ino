/* -------------------------------------------------------------------------- */
/* FILE NAME:   commute_relay.ino
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
uint8_t selected_expansion = 255;
bool commute_flag = false;
bool commute_once = false;

/* -------------------------------------------------------------------------- */
void print_menu() {
/* -------------------------------------------------------------------------- */
  if(print_menu_flag) {
	  Serial.println("### Climatic chamber tests: Commute relays ###");
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
	  Serial.println("1. Select the DIGITAL expansion"); 
	  Serial.println("2. Toggle commute output continuously"); 
	  Serial.println("3. Commute once"); 
	  Serial.println(":> ");
	  print_menu_flag = false;
  }
}

/* -------------------------------------------------------------------------- */
void commute(int exp_index) {
/* -------------------------------------------------------------------------- */
	static unsigned long start = millis() + UPDATE_TIME + 1;
  static bool st = true; 
	DigitalExpansion de = OptaController.getExpansion(exp_index);
   de.digitalRead(0,true);
   if(millis() - start > UPDATE_TIME) {
    commute_once = false;
    start = millis();
    if(de) {
      if(st) {
            de.digitalWrite(0,HIGH);  
            de.digitalWrite(1,LOW);
            de.digitalWrite(2,HIGH); 
            de.digitalWrite(3,LOW);
            de.digitalWrite(4,HIGH);
            de.digitalWrite(5,LOW);
            de.digitalWrite(6,HIGH); 
            de.digitalWrite(7,LOW);
            st = false;
      }
      else {
            de.digitalWrite(0,LOW);  
            de.digitalWrite(1,HIGH);
            de.digitalWrite(2,LOW); 
            de.digitalWrite(3,HIGH);
            de.digitalWrite(4,LOW);
            de.digitalWrite(5,HIGH);
            de.digitalWrite(6,LOW); 
            de.digitalWrite(7,HIGH);
            st = true;
      }
      
      de.updateDigitalOutputs();
      
    }
    else {
      Serial.println("Expansion is not digital");
    }
  }
}



/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  OptaController.update();
 
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
         delay(1000);
			
		}
		else if(result == 2) {
			commute_flag = !commute_flag;
		}
		else if(result == 3) {
			commute_once = true;
		}
  }

  if(selected_expansion < 5 && (commute_flag || commute_once)) {
     commute(selected_expansion);
  }
}


