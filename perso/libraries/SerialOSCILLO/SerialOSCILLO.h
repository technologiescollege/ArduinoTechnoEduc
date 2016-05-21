/*
	Librairie créée par David Souder - 01/08/2014
	Cette librairie est librement utilisable
	www.duinoedu.com

*/

// Voir infos en bas de page

//---- Options utilisateur
#define EDU_NBR_MAX_PINS 100


#ifndef SerialOSCILLO_H
#define SerialOSCILLO_H

#include <SoftwareSerial.h>
#include "Pin.h"


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
class SerialOSCILLO {
#define VALEURS_BRUTES		'|'
#define EN_VOLTS 					'~'
#define EN_CM 						'§'
#define EN_DEGRES 					'°'

#define SUR_CADRAN0			0
#define SUR_CADRAN1			1
#define SUR_CADRAN2			2
#define SUR_CADRAN3			3
#define SUR_CADRAN4			4
#define SUR_CADRAN5			5
#define SUR_CADRAN6			6
#define SUR_CADRAN7			7
#define SUR_CADRAN8			8
#define SUR_CADRAN9			9




public:

		/*EDU US*/	SerialOSCILLO();

	//---- Initialisation
		/*EDU FR*/	void brancher(int32_t speed=115200);
		/*EDU FR*/	void begin(int32_t speed=115200);
		/*EDU US*/	void branch(int32_t speed=115200);

		
	//---- Commandes d'écriture 
		/*EDU FR*/	void		nombreCadrans(int8_t nbrCadrans);
		/*EDU US*/	void 		analogWrite(int16_t pin, int8_t cadran=0, char graduation='|');
			

		
	//---- Modification options
		
		
		
	//---- Variables publiques
		
			

		

protected:
		//Pin				m_pinsBranchees[EDU_NBR_MAX_PINS];						// Contient la configuration actulle
		//uint8_t		m_pinsSouhaitees[EDU_NBR_MAX_PINS];						// Contient la nouvelle configuration (la différence entre les 2 traduit le changement à réaliser)
	    //uint8_t		m_nbrInstances;													 	    // Contient le nombre de Pins utilisées 
		uint8_t			m_nbrCadrans;		
		
		
		

		
};

extern SerialOSCILLO SerialOscillo;

#endif







