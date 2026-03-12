/* -------------------------------------------------------------------------- */
/* FILE NAME:   testEMC.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231122
   DESCRIPTION: To be used for EMC test
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */


#include "OptaBlue.h"

Controller controller; 

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
  else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
void printExpansionInfo() {
/* -------------------------------------------------------------------------- */  
  static long int start = millis();
  
  if(millis() - start > 10000) {
    start = millis();
    Serial.print("Number of expansions: ");
    Serial.println(controller.getExpansionNum());

    for(int i = 0; i < controller.getExpansionNum(); i++) {
      Serial.print("Expansion n. ");
      Serial.print(i);
      Serial.print(" type ");
      printExpansionType(controller.getExpansionType(i));
      Serial.print(" I2C address ");
      Serial.println(controller.getExpansionI2Caddress(i));
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

  controller.begin();

}


void task_single_expansion(uint8_t j) {

  controller.updateDigitalsIn(j);

  if((controller.digitalReadOpta(j,12) == LOW) && (controller.digitalReadOpta(j,
  13) == LOW) && (controller.digitalReadOpta(j,14) == LOW) &&
		(controller.digitalReadOpta(j,15) == LOW) ) {


	  if(controller.digitalReadOpta(j,0) == HIGH) {
			controller.digitalWriteOpta(j,0,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,0,LOW);

	  }

	  if(controller.digitalReadOpta(j,1) == HIGH) {
			controller.digitalWriteOpta(j,1,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,1,LOW);
	  }

	  if(controller.digitalReadOpta(j,2) == HIGH) {
			controller.digitalWriteOpta(j,2,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,2,LOW);
	  }

	  if(controller.digitalReadOpta(j,3) == HIGH) {
			controller.digitalWriteOpta(j,3,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,3,LOW);
	  }

	  if(controller.digitalReadOpta(j,4) == HIGH) {
			controller.digitalWriteOpta(j,4,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,4,LOW);
	  }

	  if(controller.digitalReadOpta(j,5) == HIGH) {
			controller.digitalWriteOpta(j,5,HIGH);
			goto done;
	  }
	  else {

			controller.digitalWriteOpta(j,5,LOW);
	  }

	  if(controller.digitalReadOpta(j,6) == HIGH) {
			controller.digitalWriteOpta(j,6,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,6,LOW);
	  }

	  if(controller.digitalReadOpta(j,7) == HIGH) {
			controller.digitalWriteOpta(j,7,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,7,LOW);
	  }

	  if(controller.digitalReadOpta(j,8) == HIGH) {
			controller.digitalWriteOpta(j,0,HIGH);
			controller.digitalWriteOpta(j,5,HIGH);
			controller.digitalWriteOpta(j,7,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,0,LOW);
			controller.digitalWriteOpta(j,5,LOW);
			controller.digitalWriteOpta(j,7,LOW);
	  }

	  if(controller.digitalReadOpta(j,9) == HIGH) {
			controller.digitalWriteOpta(j,2,HIGH);
			controller.digitalWriteOpta(j,3,HIGH);
			controller.digitalWriteOpta(j,6,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,2,LOW);
			controller.digitalWriteOpta(j,3,LOW);
			controller.digitalWriteOpta(j,6,LOW);
	  }

	  if(controller.digitalReadOpta(j,10) == HIGH) {
			controller.digitalWriteOpta(j,0,HIGH);
			goto done;

	  }
	  else {
			controller.digitalWriteOpta(j,0,LOW);
	  }

	  if(controller.digitalReadOpta(j,11) == HIGH) {
			controller.digitalWriteOpta(j,1,HIGH);
			goto done;
	  }
	  else {
			controller.digitalWriteOpta(j,1,LOW);
	  }
  }
  if(controller.digitalReadOpta(j,12) == HIGH) {
	
		static unsigned long start12 = millis();
		static bool st12 = true;
		if(millis() - start12 >= 2000) {
			start12 = millis();
			if(st12) {
		      controller.digitalWriteOpta(j,0,HIGH);
				controller.digitalWriteOpta(j,1,LOW);
				st12 = false;
			}
			else {
		      controller.digitalWriteOpta(j,0,LOW);
				controller.digitalWriteOpta(j,1,HIGH);
				st12 = true;
			}
		}
		goto done;
  }
  else {
		//	Serial.println("12-C");
		controller.digitalWriteOpta(j,2,LOW);
		controller.digitalWriteOpta(j,1,LOW);
		controller.digitalWriteOpta(j,0,LOW);
  }


  if(controller.digitalReadOpta(j,13) == HIGH) {
		static unsigned long start13 = millis();
		static bool st13 = true;
		if(millis() - start13 >= 2000) {
			start13 = millis();
			if(st13) {
			   st13 = false;
		      controller.digitalWriteOpta(j,4,HIGH);
				controller.digitalWriteOpta(j,5,LOW);
			}
			else {
		      controller.digitalWriteOpta(j,4,LOW);
				controller.digitalWriteOpta(j,5,HIGH);
				st13 = true;
			}
		}
		goto done;
  }
  else {
      if(controller.digitalReadOpta(j,15) == LOW) {
			controller.digitalWriteOpta(j,5,LOW);
			controller.digitalWriteOpta(j,4,LOW);
		}
  }

  if(controller.digitalReadOpta(j,14) == HIGH) {
		static unsigned long start14 = millis();
		static bool st14 = true;
		if(millis() - start14 >= 2000) {
			start14 = millis();
			if(st14) {
			   st14 = false;
		      controller.digitalWriteOpta(j,6,HIGH);
				controller.digitalWriteOpta(j,7,LOW);
			}
			else {
			   st14 = true;
		      controller.digitalWriteOpta(j,6,LOW);
				controller.digitalWriteOpta(j,7,HIGH);
			}
		}
		goto done;
  }
  else {
      if(controller.digitalReadOpta(j,15) == LOW) {
			controller.digitalWriteOpta(j,6,LOW);
			controller.digitalWriteOpta(j,7,LOW);
		}
  }

  if(controller.digitalReadOpta(j,15) == HIGH) {
		static unsigned long start15 = millis();
		static bool st15 = true;
		if(millis() - start15 >= 2000) {
			start15 = millis();
			if(st15) {
			   st15 = false;
		      controller.digitalWriteOpta(j,5,HIGH);
				controller.digitalWriteOpta(j,7,LOW);
			}
			else {
			   st15 = true;
		      controller.digitalWriteOpta(j,5,LOW);
				controller.digitalWriteOpta(j,7,HIGH);
			}
		}
		goto done;
  }
  else {
      if(controller.digitalReadOpta(j,14) == LOW && controller.digitalReadOpta(j,13) == LOW) {
		   controller.digitalWriteOpta(j,5,LOW);
		   controller.digitalWriteOpta(j,7,LOW);
		}
  }
  done: 
  controller.updateDigitalsOut(j);
}


/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  controller.update();

  for(int i = 0; i < controller.getExpansionNum(); i++) {
    task_single_expansion(i);
  }

}


