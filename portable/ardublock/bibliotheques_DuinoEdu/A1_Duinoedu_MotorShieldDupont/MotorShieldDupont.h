/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 28/02/2017
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
#define EDU_L298 	 1
#define EDU_HBRIDGE  2


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
				
	/*EDU FR*/	void brancher(uint8_t constructor=EDU_ARDUMOTO);								// L298P ARDUMOTO
	/*EDU FR*/	void brancher(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm);			// L298P GENERIQUE
	/*EDU FR*/	void brancherHBridge(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm);	// Pont en H GENERIQUE
	
	/*EDU FR*/	void modeToutOuRien();
				boolean toutOuRien;
	
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
		
		// Mode tout ou rien
			uint8_t valA1;
			uint8_t valA2;
			uint8_t valB1;
			uint8_t valB2;
		// Mode PWM
			uint8_t valA2Pwm;
			uint8_t valB2Pwm;
	
		// Mémorise la dernière instruction appelée 
			int16_t lastCmd;	
				// 111 : avancer tout_droit 
				// 110 : avancer droite
				// 101 : avancer gauche
				// 211 : reculer tout_droit
				// 210 : reculer droite
				// 201 : reculer gauche
				// 300 : pivoter tout_droit (aucun sens)
				// 310 : pivoter droite
				// 301 : pivoter gauche
				// 0   : Stopper
	
		// Variables de consignes 
			const String tout_droit;
			const String droite;
			const String gauche;
	
	
	/*EDU US*/	void serialDebug(const char _message1[]);	
			boolean serialInfos;
			
	

	
	
	
	
	
	
	
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
		uint16_t 	m_lastEffectiveSpeedA225;			// Calcul si changement
		uint16_t 	m_lastEffectiveSpeedB225;
	
	//-- Dérive
		uint16_t	m_userDrift;						// 0] - [500] - [1000
	
	/*EDU US*/	void 		setPins(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm);		// Enregistrement des broches
	/*EDU US*/	void		setEffectiveSpeedAB255();												// Calcule les vitesses effectives
	
	/*EDU US*/	void		effectiveWrite();
	/*EDU US*/	//void		effectiveWrite(int8_t _valA1, int8_t _valA2, int8_t _valB1, int8_t _valB2, int8_t _valA2Pwm, int8_t _valB2Pwm);
	
	/*EDU US*/	void		callBack();					// Rappelle la dernière commande de direction

};
#endif
