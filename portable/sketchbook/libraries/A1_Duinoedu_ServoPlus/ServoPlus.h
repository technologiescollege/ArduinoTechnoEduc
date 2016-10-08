/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 25/09/2016
  
  
*/

#ifndef NewServo_H
#define NewServo_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#include <Servo.h>
#include "EDU_debug.h"

//#define DEBUG

class ServoPlus{

	public:
	
	/*EDU FR*/	ServoPlus();
				bool modeAsync;

	/*EDU FR*/	uint8_t 	brancher(int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	branch  (int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	attach  (int pin, int buteeHaute=165, int buteeBasse=15);
				uint8_t 	servoPin;
				
	
	
	
	//-------- Gestion des int
	/*EDU FR*/	void 		ecrire(int value, 				int speed=1000); 
	/*EDU FR*/	void 		ecrireAngle(int value, 			int speed=1000);

	//-------- Gestion des long
	/*EDU FR*/	void 		ecrire(long value, 				int speed=1000); 
	/*EDU FR*/	void 		ecrireAngle(long value, 		int speed=1000);
	
	//-------- Gestion des float	
	/*EDU FR*/	void		ecrire(float value, 			int speed=1000);
	/*EDU FR*/  void		ecrireAngle(float value, 		int speed=1000);

	//--------	Gestion de l'activation/Désactivation
	/*EDU FR*/	void		desactiver();
	/*EDU US*/	void 		desactivate();
	
	/*EDU FR*/	void		reactiver();
	/*EDU US*/	void 		reactivate();
				uint8_t		servoActivated;
				bool		etaitDesactive;
	
	
	

	//--------  Fonctions effectives
	/*EDU US*/	void 		write								(int value, 	int speed=1000); 
	/*EDU US*/	void 		writeMicroseconds					(int value, 	int speed=1000); 
	/*EDU US*/	void 		writeMicrosecondsAssync				(int value, 	int speed=1000);
				uint32_t 	lastTime;
	
	
	Servo* 			servo;								// Le servo de base auquel on ajoute des méthodes
		
	
	
	
	protected:
	uint16_t		m_curentMicroPos;					// Durée ACTUELLE de l'impultion en microsecondes
	uint16_t		m_finalMicroPos;					// Durée SOUHAITEE de l'impultion en microsecondes
	uint16_t		m_speed;							// Vitesse à laquelle doit s'éfectuer le changement de position du servo
    uint16_t		m_buteeBasse;					
	uint16_t		m_buteeHaute;						// 
	String			m_version;							// Version du module
	


};
#endif
