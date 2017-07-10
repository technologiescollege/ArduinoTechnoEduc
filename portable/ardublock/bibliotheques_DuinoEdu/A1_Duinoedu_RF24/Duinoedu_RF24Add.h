/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 24/04/2017
  
*/

#ifndef Duinoedu_RF24Add_H
#define Duinoedu_RF24Add_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#include <RF24.h>


class Duinoedu_RF24Add : public RF24 
{

	public:
	
	/*EDU FR*/	Duinoedu_RF24Add(uint8_t _cepin, uint8_t _cspin);
	

	protected:



};
#endif
