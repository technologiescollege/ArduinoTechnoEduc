/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 08/03/2016
  
*/



	
#ifndef Duinoedu_Utility_H
#define Duinoedu_Utility_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#define JUMP "\r\n"
#define _SEPARATOR    '='
#define _END    '|'

class Duinoedu_Utility{

	public:
	Duinoedu_Utility();

};

// IR : utiliser un émetteur IR Grove pour une barrière
	void irGenerator(uint8_t _pin, uint8_t on, unsigned int frequency);
	
// STRING : récupération d'une valeur à partir d'une clé [EXEMPLE 01]
	String	findStringValue(String _bigString, String _key, char _separator, char _end=0);
	int findIntValue(String _bigString, String _key, char _separator, char _end=0);
	double findDoubleValue(String _bigString, String _key, char _separator, char _end=0);
	//Integer = findIntValue(String(Serial.read()), "key",'t','b') ;
	
// Send : envoyer une valeur à partir d'une clé avec un séparateur et une fin
	String SendStringSerial(String _key, String _Value, char _separator='=', char _end=-1);

	
	
// CHRONOS
class Chrono{
	public:
		Chrono();
		void 			start();
		uint32_t 		duration();
						uint32_t lastTime;
		void 			reset();
};

// ONLY EVERY
#define ONLY_EVERY(val) \
  static uint32_t lastTime = 0;\
  uint32_t thisTime = millis();\
  if( thisTime > (lastTime+val) ){\
  lastTime=thisTime;	
#define END_ONLY_EVERY }

#define NOT_BEFORE(val) \
  if(millis()>val){
#define END_NOT_BEFORE }


// ANALOG READ WIDTH

#if defined(ESP8266)
	// Convertisseur sur 0..3V avec un ESP8266
	#define VOLTAGE_CONVERTER(val) float(val*3 / 1023.0)
#else
	// Convertisseur sur 0..5V avec un Atmega328 & 2560
	#define VOLTAGE_CONVERTER(val) float(val*5 / 1023.0)
#endif







#endif






















/* [EXEMPLE 01]
#include <Duinoedu_Utility.h>

String maChaine ="#LED1=ON&LED2=OFF";


void setup(){
  Serial.begin(9600);
  
}


void loop(){
  String var1 = findStringValue(maChaine,"LED1",'=','&');
  String var2 = findStringValue(maChaine,"LED2",'=','&');  

  Serial.println(var1);
  Serial.println(var2);
  delay(1000);
}

*/







