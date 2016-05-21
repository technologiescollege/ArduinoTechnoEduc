/*
 
  Original Author: David Souder - souder.d@gmail.com pour DuinoEDU.com
  Date de dernière modification : 18/10/2015
  
*/



#ifndef Duinoedu_MathPlus_H
#define Duinoedu_MathPlus_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include <WProgram.h>
	#include "Wire.h"
#endif


#include "EDU_debug.h"
//#define DEBUG

static float convertirEnDegres(uint16_t valCan, uint16_t coefTermitance=3975, uint16_t maxConv = 1023 );
static float groveCanToTemp(uint16_t valCan, uint16_t coefTermitance=3975, uint16_t maxConv = 1023 );

static float convertirEnDegres(uint16_t valCan, uint16_t coefTermitance, uint16_t maxConv ){
	return groveCanToTemp(valCan);
}
static float groveCanToTemp(uint16_t valCan, uint16_t coefTermitance, uint16_t maxConv){
// Calcul une température à partir d'une valeur brute de conversion
				
	float temperature;
	float resistance;
	// Calcul de résistance
		resistance=(float)(1023-valCan)*10000/valCan;	
	// Calcul de température 								
		temperature=1/(log(resistance/10000)/coefTermitance+1/298.15)-273.15;
	
	return temperature;		

}

static float lm35CanToTemp(uint16_t valCan){
// Calcul une température à partir d'une valeur brute de conversion
// https://www.dfrobot.com/wiki/index.php?title=DFRobot_LM35_Linear_Temperature_Sensor_(SKU:DFR0023)

	float temp;
	temp = valCan;
	temp *=500;
	temp /=1024;
	
	return temp;		

}



#endif


// To do....
/* GESTION D UNE THERMISTANCE 10K
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
// What pin to connect the sensor to
#define THERMISTORPIN A0 
 
void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  float reading;
 
  reading = analogRead(THERMISTORPIN);
 
  Serial.print("Analog reading "); 
  Serial.println(reading);
 
  // convert the value to resistance
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;
  Serial.print("Thermistor resistance "); 
  Serial.println(reading);
 
  delay(1000);
}
*/