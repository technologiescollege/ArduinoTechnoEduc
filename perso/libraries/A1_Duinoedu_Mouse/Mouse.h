/*
	Librairie crée par David Souder - 08/07/2014
	Elle a été inspirée par le programme disponible ici :
	http://playground.arduino.cc/ComponentLib/Ps2mouse
	Cette librairie est librement utilisable
	www.duinoedu.com

*/


#ifndef Mouse_H
#define Mouse_H

#include "debug.h"

//----- Nécessaire car utilisation de String
	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif




//----- Début classe
class Mouse {

public:

		/*EDU US*/	Mouse();
		
		/*EDU FR*/	void brancher(int8_t pinData, int8_t pinClk=0);
		/*EDU US*/	void branch(int8_t pinData, int8_t pinClk=0);
		/*ARD US*/	void mouse_init();
		/*ARD US*/	void mouse_write(char data);
		/*ARD US*/	char mouse_read(void);
		
		/*EDU FR*/	int lire(String quoi="x");
		/*EDU US*/	int read(String what="x");
				
		/*EDU FR*/	int lireX();
		/*EDU US*/	int readX();
		
		/*EDU FR*/	int lireY();
		/*EDU US*/	int readY();
			

protected:
	
	//---- Méthodes
		/*EDU US*/	void effectiveRead(int8_t* mstat,int8_t m[],int32_t theDate[]);
		/*ARD US*/	void gohi(int8_t pin);
		/*ARD US*/	void golo(int8_t pin);
	

	//---- Attributs
		/*EDU US*/	int8_t	m_DATA;
		/*EDU US*/ 	int8_t	m_CLK;
	
	//---- Calcul intérgral (0=X / 1=Y)
		//--- Pour X
		/*EDU US*/	int32_t	m_t1[2];		// Date début échantillon 
		/*EDU US*/	int32_t	m_t2[2];		// Date fin échantillon 
		/*EDU US*/	int8_t	m_s1[2];		// Vitesse instantannée à t1
		/*EDU US*/	int8_t	m_s2[2];		// Vitesse instantannée à t2
		
	
	//---- Positions
		/*EDU US*/	int32_t	m_pos1000[2];		// Position de la souris en X (0 à 1000000) 1000 fois trop grande
		

		/*EDU US*/	int16_t	m_pos[2];			// Position de la souris en X (0 à 1000)
				
		/*EDU US*/	int16_t	m_sensitivity;

		
};


#endif


