/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 07/01/2016
  
*/

#ifndef Duinoedu_EasyInterrupt_H
#define Duinoedu_EasyInterrupt_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


//#include <Duinoedu_EasyInterrupt.h>
#include "EDU_debug.h"

//#define DEBUG

class Duinoedu_EasyInterrupt{

	public:
	
	/*EDU FR*/	Duinoedu_EasyInterrupt();
	void attachInterruptPin(int8_t _pin);


	protected:



};
#endif
