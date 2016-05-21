/*
	Librairie créée par David Souder - 01/08/2014
	Cette librairie est librement utilisable
	www.duinoedu.com

*/

// Voir infos en bas de page


#ifndef PIN_H
#define PIN_H

#include <SoftwareSerial.h>


#include "EDU_debug.h"

//----- Nécessaire si utilisation de String

	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif

//----- Langage EDU
#define faire void
#define initialisation setup()
#define boucle loop()

#define portSerie Serial.begin(9600)

#define lectureEtat digitalRead

#define utiliser 

#define chaine String
#define nombre int
#define grandNombre long

#define si if
#define sinon else


//----- Début classe
class Pin {

public:

		/*EDU US*/	Pin();

	//---- Initialisation
		/*EDU US*/	void branch();

	//---- Méthodes appératives
		/*EDU US*/	void methode();
		
	//---- Modification options
		
		
		
	//---- Variables publiques
		int8_t	pin;
		int8_t	cadran;
		int8_t 	graduation;	
			

		

protected:
	//---- N° de la broche choisie


	
		

		
};

//extern xx xx;

#endif







