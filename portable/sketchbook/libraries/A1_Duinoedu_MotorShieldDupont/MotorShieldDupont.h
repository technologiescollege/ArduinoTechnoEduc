/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 18/08/2016
  www.duinoedu.com
  
*/

#ifndef MotorShieldDupont_H
#define MotorShieldDupont_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

//-- Les différentes constructeurs
#define EDU_ARDUMOTO 0

//-- Les broches constructeurs
#define ARDUMOTO_ADIR 12		// Ardumoto
#define ARDUMOTO_APWM 3			// Ardumoto
#define ARDUMOTO_BDIR 13		// Ardumoto
#define ARDUMOTO_BPWM 11		// Ardumoto

//#include <___.h>
#include "EDU_debug.h"

//#define DEBUG

class MotorShieldDupont{

	public:
	
	/*EDU FR*/	MotorShieldDupont();
	
	/*EDU FR*/	void brancher(uint8_t constructor=EDU_ARDUMOTO);
	/*EDU FR*/	void brancher(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm);		// Config broche par broche

	/*EDU FR*/	void reglerVitesse(int16_t userSpeedA, int16_t userSpeedB);
	/*EDU FR*/	void ecrireVitesse(int16_t userSpeedA, int16_t userSpeedB);
	
	/*EDU FR*/	void ecrireDerive(int16_t userDrift);
	
	
	/*EDU FR*/	void avancer(String direction="");
	/*EDU FR*/	void reculer(String direction="");
	/*EDU FR*/	void pivoter(String direction="");
	/*EDU FR*/	void stopper();
		#define EDU_MSD_ARDU_AVANT_A	LOW				// Invertion dans le L298P => Allume LED verte
		#define EDU_MSD_ARDU_AVANT_B	HIGH			// Invertion dans le L298P => Allume LED verte
		#define EDU_MSD_ARDU_ARRIERE_A	HIGH			// Invertion dans le L298P => Allume LED rouge
		#define EDU_MSD_ARDU_ARRIERE_B	LOW				// Invertion dans le L298P => Allume LED rouge
	


	protected:
	//-- Constructeur
		byte 		m_constructor;
	//-- Broches
		byte 		m_pinDirA;
		byte 		m_pinSpeedA;
		byte 		m_pinDirB;
		byte 		m_pinSpeedB;
	
	//-- Vitesses
		uint16_t	m_userSpeedA;
		uint16_t	m_userSpeedB;
		uint32_t	m_effectiveSpeedA255;				// Débordement lors des calculs (donc uint32_t)
		uint32_t	m_effectiveSpeedB255;
	
	//-- Dérive
		uint16_t	m_userDrift;						// 0] - [500] - [1000
	
	/*EDU US*/	void 		setPins(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm);		// Enregistrement des broches
	/*EDU US*/	void 		initPins(uint8_t constructor=EDU_ARDUMOTO);								// Init. des broches + état
	/*EDU US*/	void		setEffectiveSpeedAB255();												// Calcule les vitesses effectives


};
#endif
