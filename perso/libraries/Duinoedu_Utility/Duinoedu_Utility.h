/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 11/02/2014
  
*/

#ifndef Duinoedu_Utility_H
#define Duinoedu_Utility_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


//#include <___.h>
#include "EDU_debug.h"

//#define DEBUG

class Duinoedu_Utility{

	public:
	
	/*EDU FR*/	Duinoedu_Utility();
	/*EDU US*/	StringToInteger();



	protected:



};
extern Duinoedu_Utility utility;
#endif
