	/* -------------------------------------------------------------------------- */
/* FILE NAME:   genericDigital.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240219
   DESCRIPTION: This example is used on Opta OptaController to show a possible
	             use of one ore more Opta Digital
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:       This example uses 2 opta digital one with mechanical rele
	             at position 0, one with state solid rele at position 1 */
/* -------------------------------------------------------------------------- */



#include "OptaBlue.h"

using namespace Opta;


DigitalMechExpansion mechExp;
DigitalStSolidExpansion stsolidExp;

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  /* always call OptaController.begin() */
  OptaController.begin();

  mechExp = OptaController.getExpansion(0); /* Expansion at index 0 is Mechanical */
  stsolidExp = OptaController.getExpansion(1); /* Expansion at index 1 is State
  Solid */

}

/* task for expansion at position 0 */
void taskExpansion0 () {

   /* this way all digital are updated at the same time */
   mechExp.updateDigitalInputs();

   /* if digital input 0 or 1 are HIGH */
	if(mechExp.digitalRead(0) == HIGH || mechExp.digitalRead(1) ==
	HIGH) {
	   /* set high Digital Output 0 */
		mechExp.digitalWrite(0, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(0, LOW);
	}
	
	Serial.println(mechExp.pinVoltage(2));
	Serial.println(mechExp.pinVoltage(3));

	if( (mechExp.pinVoltage(2) > 4.00 && mechExp.pinVoltage(2) < 6.00) || 
	     mechExp.pinVoltage(3) > 13.00) {
	   /* set high Digital Output 1 */
		mechExp.digitalWrite(1, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(1, LOW);
	}

	if(mechExp.digitalRead(4) == HIGH || mechExp.digitalRead(5) ==
	HIGH) {
	   /* set high Digital Output 0 */
		mechExp.digitalWrite(2, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(2, LOW);
	}

	if( (mechExp.pinVoltage(6) > 4.00 && mechExp.pinVoltage(6) < 6.00) || 
	     mechExp.pinVoltage(7) > 15.00) {
	   /* set high Digital Output 1 */
		mechExp.digitalWrite(3, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(3, LOW);
	}

	if(mechExp.digitalRead(8) == HIGH || mechExp.digitalRead(9) ==
	HIGH) {
	   /* set high Digital Output 0 */
		mechExp.digitalWrite(4, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(4, LOW);
	}

	if( (mechExp.pinVoltage(10) > 1.8 && mechExp.pinVoltage(10) < 3.5) || 
	     mechExp.pinVoltage(11) > 15.00) {
	   /* set high Digital Output 1 */
		mechExp.digitalWrite(5, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
			
		mechExp.digitalWrite(5, LOW);
	}

	if(mechExp.digitalRead(12) == HIGH || mechExp.digitalRead(13) ==
	HIGH) {
	   /* set high Digital Output 0 */
		mechExp.digitalWrite(6, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(6, LOW);
	}


	if( (mechExp.pinVoltage(14) > 1.8 && mechExp.pinVoltage(14) < 3.5) ||
	     mechExp.pinVoltage(15) > 4) {
	   /* set high Digital Output 1 */
		mechExp.digitalWrite(7, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		mechExp.digitalWrite(7, LOW);
	}
   
	/* all digital outputs are updated at the same time */
	mechExp.updateDigitalOutputs();

}


void taskExpansion1 () {
   /* this way all digital are updated at the same time */
   stsolidExp.updateDigitalInputs();
   /* if digital input 0 or 1 are HIGH */
	if(stsolidExp.digitalRead(0) == HIGH || stsolidExp.digitalRead(1) ==
	HIGH) {
	   /* set high Digital Output 0 */
		stsolidExp.digitalWrite(7, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(7, LOW);
	}
			
	if( (stsolidExp.pinVoltage(2) > 4.00 && stsolidExp.pinVoltage(2) < 6.00) || 
	     stsolidExp.pinVoltage(3) > 13.00) {
	   /* set high Digital Output 1 */
		stsolidExp.digitalWrite(6, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(6, LOW);
	}

	if(stsolidExp.digitalRead(4) == HIGH || stsolidExp.digitalRead(5) ==
	HIGH) {
	   /* set high Digital Output 0 */
		stsolidExp.digitalWrite(5, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(5, LOW);
	}

	if( (stsolidExp.pinVoltage(6) > 4.00 && stsolidExp.pinVoltage(6) < 6.00) || 
	     stsolidExp.pinVoltage(7) > 15.00) {
	   /* set high Digital Output 1 */
		stsolidExp.digitalWrite(4, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(4, LOW);
	}

	if(stsolidExp.digitalRead(8) == HIGH || stsolidExp.digitalRead(9) ==
	HIGH) {
	   /* set high Digital Output 0 */
		stsolidExp.digitalWrite(3, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(3, LOW);
	}

	if( (stsolidExp.pinVoltage(10) > 1.8 && stsolidExp.pinVoltage(10) < 3.5) || 
	     stsolidExp.pinVoltage(11) > 15.00) {
	   /* set high Digital Output 1 */
		stsolidExp.digitalWrite(2, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(2, LOW);
	}

	if(stsolidExp.digitalRead(12) == HIGH || stsolidExp.digitalRead(13) ==
	HIGH) {
	   /* set high Digital Output 0 */
		stsolidExp.digitalWrite(1, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(1, LOW);
	}

	if( (stsolidExp.pinVoltage(14) > 1.8 && stsolidExp.pinVoltage(14) < 3.5) || 
	     stsolidExp.pinVoltage(15) > 4.00) {
	   /* set high Digital Output 1 */
		stsolidExp.digitalWrite(0, HIGH);
	}
	else {
	   /* otherwise set Digital Ouput */
		stsolidExp.digitalWrite(0, LOW);
	}

	/* all digital outputs are updated at the same time */
	stsolidExp.updateDigitalOutputs();
}
/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */    
  OptaController.update();

  mechExp = OptaController.getExpansion(0);

  stsolidExp = OptaController.getExpansion(1);


  /* check for validity of mechExp */
  if(mechExp) {
      /* you can check the index of the expansion */
		if(mechExp.getIndex() == 0) {
			taskExpansion0 ();
		}
  }

  if(stsolidExp) {
      /* you can check the index of the expansion */
		if(stsolidExp.getIndex() == 1) { 
			taskExpansion1 ();
		}
  }




}

