/*
	Librairie crée par David Souder - 10/01/2015
	Cette librairie est librement utilisable
	www.duinoedu.com

*/


#ifndef  AnalogicAccelerometer_H
#define AnalogicAccelerometer_H

#include "debug.h"

//----- Nécessaire car utilisation de String...
	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif




//----- Début classe
class AnalogicAccelerometer {

public :

		/*EDU US*/	AnalogicAccelerometer();
		
		/*EDU FR*/ 	void brancher(int pinX, int pinY=-1);
		/*EDU US*/ 	void branch(int pinX, int pinY=-1);
		
		/*EDU FR*/	int lire(char axis='X');
		/*EDU US*/	int read(char axis);
		
		// Réglage utilisateur (par monAccelerometre.correctionEnX=...)
		int correctionEnX;				// Décalages en X 
		int correctionEnY;				// Y
		int buteeHauteX;				// Valeur brute réellement retournée par le capteur 
		int buteeBasseX;				//
		int buteeHauteY;				// 
		int buteeBasseY;
			

			

protected :
	
	//---- Méthodes privées

	

	//---- Attributs
		
		int8_t m_pinX;					// Pin pour accélération X
		int8_t m_pinY;					// Pin pour accélération Y (-1 si non utilisé)
		
		int m_ofsetX;
		float m_coefX;
		
		
			


		
};


#endif


