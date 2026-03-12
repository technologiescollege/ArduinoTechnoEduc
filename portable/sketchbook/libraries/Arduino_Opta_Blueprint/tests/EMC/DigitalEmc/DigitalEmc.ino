/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalEmc.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20231122
   DESCRIPTION: This is a special FW to be used on Opta-Digital for EMC test.
                The only difference respect to a "non EMC" FW is that this one 
                has a special initial sequence that makes clear if the device 
                has reset. 
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */



#include "OptaBlue.h"

#define DELAY_INITIAL_SEQ  2000

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */    
   pinMode(0,OUTPUT);
	digitalWrite(0,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(0,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(1,OUTPUT);
	digitalWrite(1,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(1,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(2,OUTPUT);
	digitalWrite(2,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(2,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(3,OUTPUT);
	digitalWrite(3,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(3,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(4,OUTPUT);
	digitalWrite(4,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(4,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(5,OUTPUT);
	digitalWrite(5,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(5,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(6,OUTPUT);
	digitalWrite(6,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(6,LOW);
	delay(DELAY_INITIAL_SEQ);

   pinMode(7,OUTPUT);
	digitalWrite(7,HIGH);
	delay(DELAY_INITIAL_SEQ);
	digitalWrite(7,LOW);
	delay(DELAY_INITIAL_SEQ);


	OptaExpansion = new OptaDigital();

   if(OptaExpansion != nullptr) {
      OptaExpansion->begin();
   }
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  if(OptaExpansion != nullptr) {
      OptaExpansion->update();
   }
   
}

