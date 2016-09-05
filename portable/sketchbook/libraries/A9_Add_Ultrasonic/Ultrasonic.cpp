/*
  Ultrasonic.h - Ultrasonic driver Library
  2011 Copyright (c) Seeed Technology Inc.  All right reserved.
  For Ultrasonic v1.0 firmware.

  Original Author: LG
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Ultrasonic.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//<<CONSTRUCTOR>>
Ultrasonic::Ultrasonic(int pin, int pinTrig){
	_pinTrig = pinTrig;
	_pin = pin;
}

// MESURES EN CM
/*EDU FR*/long Ultrasonic::mesurerEnCm(int nbrOfEch){
  if(_pinTrig==0){
    // Gestion type Grove
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInCentimeters = duration/29/2;	
	return RangeInCentimeters;
	}
	else{
	// Gestion Dupont=SR04
	pinMode(_pinTrig, OUTPUT);
	digitalWrite(_pinTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pinTrig,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInCentimeters = duration/29/2;
	return RangeInCentimeters;
	}
}
/*ARD US*/ void Ultrasonic::MeasureInCentimeters(int nbrOfEch){
  if(_pinTrig==0){
    //----- Gestion type Grove (1 broche)
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInCentimeters = duration/29/2;	
	}
	else{
	//----- Gestion Dupont=SR04 (2 broches)
	pinMode(_pinTrig, OUTPUT);
	digitalWrite(_pinTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pinTrig,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInCentimeters = duration/29/2;
	}
}

/*EDU FR*/ long Ultrasonic::mesurer(int nbrOfEch){return MultiMeasureInCentimeters(nbrOfEch);}
/*EDU US*/ long Ultrasonic::MultiMeasureInCentimeters(int nbrOfEch){
	long val=0;
	//-- On récupère le nombre d'échantillons courant
		if(nbrOfEch<ULTRASONIC_NBR_OF_ECH_MAX){_nbrOfEch=nbrOfEch;}
		else{_nbrOfEch=ULTRASONIC_NBR_OF_ECH_MAX;}
		
	//-- On réalise les mesures et on remplit le tableau
		for(uint8_t i=0;i<nbrOfEch;i++){
			MeasureInCentimeters();
			_ech[i]=RangeInCentimeters;
			delay(10);
		}
	//-- On calcule la valeur moyenne des échantillons
		for(uint8_t j=0;j<nbrOfEch;j++){
			val+=_ech[j];
		}
		val/=nbrOfEch;
	
	return val;
		

}

// MESURES EN MM 
/*EDU FR*/ long Ultrasonic::mesurerEnMm(void){
	return MeasureInMillimeters();
}
/*EDU US*/ long Ultrasonic::MeasureInMillimeters(void){
  if(_pinTrig==0){
    // Gestion type Grove
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInMillimeters = 10*duration/29/2;	
	return RangeInMillimeters;
	}
	else{
	// Gestion Dupont=SR04
	pinMode(_pinTrig, OUTPUT);
	digitalWrite(_pinTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pinTrig,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInMillimeters = 10*duration/29/2;
	return RangeInMillimeters;
	}
}

// MESURES EN POUCES
/*ARD US*/ void Ultrasonic::MeasureInInches(void){
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH);
	RangeInInches = duration/74/2;
}


