/*
  ServoPlus360 - Pilotage de servomoteurs à rotation continue
  2015 Copyright (c) www.duinoedu.com. 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 31/12/2015
  Utilisation commerciales interdites sans autorisation de son auteur
  
*/

#ifndef ServoPlus360_H
#define ServoPlus360_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#include <Servo.h>
#include "EDU_debug.h"

//#define DEBUG
#define OUI true
#define NON false

class ServoPlus360{

	public:
	
	/*EDU FR*/		ServoPlus360();
	Servo* 			servo;					// Le servo de base auquel on ajoute des méthodes

	//========== SETUP	
	/*EDU FR*/	uint8_t 	brancher	(int pin);
	/*EDU US*/	uint8_t 	branch		(int pin);
	/*EDU US*/	uint8_t 	attach		(int pin);
	
	
	//========== METHODES OPERATIVES
	//----- MODE : 0 = SENS1, 500 = arrêt, 1000 = SENS2
	/*EDU FR*/	void 	ecrireRotation	    (float val1000User);				// [0~1000], 500 = point milieu thèorique
	/*EDU US*/	void 	writeRotation	    (float val1000User);				
	
	//----- MODE : 0 = VITESSE MIN, 1000 = VITESSE MAX
	/*EDU FR*/	void 	ecrireVitesse	    (float val1000User);				// [0~1000], 500 = point milieu thèorique
	/*EDU US*/	void 	writeSpeed	        (float val1000User);				
	
	/*EDU US*/	void 	writeMicroseconds	(uint16_t pulseDelay);			    // Ecriture directe en largeur d'impultions

	
	//========== MODIFICATION D'OPTIONS
	/*EDU US*/	void 	reglerDecalage	    (int16_t offsetUser);				// Ajustement de la position neutre du servo (l'arrêt)
	/*EDU US*/	void 	writeOffset	      	(int16_t offsetUser);			
		int16_t offset;

	/*EDU US*/	void 	inverserRotation    (boolean rotationUser);				// Inverse le sens de rotation
	/*EDU US*/	void 	reverseRotation	    (boolean rotationUser);		
		boolean reverseRotationValue;
		

		
	
	protected:
	
	/*EDU US*/	uint16_t pulseCalculation(uint16_t val1000);

	/*EDU US*/	uint16_t pulseCalculation2(uint16_t val1000);






};
#endif
