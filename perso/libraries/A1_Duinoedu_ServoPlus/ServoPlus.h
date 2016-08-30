/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 29/12/2015
  
  
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

	/*EDU FR*/	uint8_t 	brancher(int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	branch(int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	attach(int pin, int buteeHaute=165, int buteeBasse=15);
	
	//-------- Gestion des int
	/*EDU FR*/	void 		ecrire(int value, 				int speed=1000); 
	/*EDU FR*/	void 		ecrireAngle(int value, 			int speed=1000);
	
	//-------- Gestion des float	
	/*EDU FR*/	void		ecrire(float value, 			int speed=1000);
	/*EDU FR*/  void		ecrireAngle(float value, 		int speed=1000);


	//--------  Fonctions effectives
	/*EDU US*/	void 		write(int value, 				int speed=1000); 
	/*EDU US*/	void 		writeMicroseconds(int value, 	int speed=1000); 
	
	Servo* 			servo;					// Le servo de base auquel on ajoute des méthodes
		
	
	
	
	protected:
	
	uint16_t		m_curentMicroPos;					// Durée ACTUELLE de l'impultion en microsecondes
	uint16_t		m_finalMicroPos;					// Durée SOUHAITEE de l'impultion en microsecondes
	uint16_t		m_speed;							// Vitesse à laquelle doit s'éfectuer le changement de position du servo
    uint16_t		m_buteeBasse;					
	uint16_t		m_buteeHaute;						// 
	String			m_version;							// Version du module








};
#endif
