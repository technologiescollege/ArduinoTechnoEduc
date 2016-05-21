/*
	Librairie crée par David Souder - 10/01/2015 
	Librairie inspirée de l'exemple de programme impcomplet fourini par Seeed Grove
	Cette librairie est librement utilisable 
	OpenSource
	www.duinoedu.com

*/


#ifndef EduMp3_H
#define EduMp3_H

#include <SoftwareSerial.h>
#include "debug.h"

//#include <Stream.h>

//----- Nécessaire car utilisation de String...
	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif




//----- Début classe
class EduMp3{

public :

		/*EDU US*/	EduMp3(int pin1, int pin2);				// Pins 2-3 à la construction
		
		/*EDU FR*/ 	void brancher();		// Pins 2-3 par défaut
		/*EDU US*/ 	void branch();
		
		/*EDU FR*/	boolean choisirModeDeLecture(uint8_t playmode);
		/*ARD US*/	boolean SetPlayMode(uint8_t playmode);
		
		/*EDU FR*/ 	boolean lireMorceau(uint8_t track);
		/*EDU FR*/ 	boolean lire(uint8_t track);
		/*ARD US*/	boolean SetMusicPlay(uint8_t hbyte,uint8_t lbyte);
		
		/*EDU FR*/	boolean volume(int volume);
		/*EDU FR*/	boolean reglerVolume(int volume);
		/*ARD US*/	boolean SetVolume(int volume);
		
		/*EDU FR*/	boolean mettreEnPause(void);
		/*EDU FR*/	boolean pause(void);
		/*ARD US*/	boolean PauseOnOffCurrentMusic(void);

protected :
	
	//---- Méthodes privées

	

	//---- Attributs
		
		int8_t 		m_pin1;					// 1ère broche de port série virtuel  
		int8_t 		m_pin2;					// 2ème broche 
		uint16_t	m_volume;				// Mémoire sur fourchette 0 > 1000
		SoftwareSerial* mySerial;
		
			


		
};


#endif


