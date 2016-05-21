/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 11/02/2014
  Tous droits réservés www.duinoedu.com
  
*/

#ifndef Duinoedu_NeoPixelAdd_H
#define Duinoedu_NeoPixelAdd_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#include "Adafruit_NeoPixel.h"

#include "EDU_debug.h"
//#define DEBUG

class Duinoedu_NeoPixelAdd : public Adafruit_NeoPixel {

	public:
	
	/*EDU US*/	Duinoedu_NeoPixelAdd(uint16_t taille, uint8_t p, uint8_t t);
			uint16_t m_taille;	
	
	/*EDU FR*/	void preparerPixel(	uint16_t 	startLed, 
									uint16_t 	r,  uint16_t g,  uint16_t b, 
									uint16_t 	widthGroupe=1, 
									uint8_t 	step=1, 
									int16_t 	endR=-1,  int16_t endG=-1,  int16_t endB=-1, 
									int16_t 	dep=0
								   );

			uint16_t 	m_startIndex			;   // Position effective du premier pixel (après calcul) 
			uint16_t 	m_r, m_g, m_b			;
			uint16_t	m_widthGroupe			;
			uint8_t		m_step					;
			uint16_t	m_endR, m_endG,m_endB	;

			
			
			
			
								   
								   
	/*EDU US*/	void afficher(void);
	/*EDU US*/	void show(void);

	
	byte *memory;			// Contient l'état actuel du ruban


	/*EDU US*/	void saveBit(bool value, uint16_t place, byte *tab=0);
	/*EDU US*/	bool readBit(uint16_t place, byte *tab=0);
	
	protected:
		

	
	
		//long count;

	



};
#endif
