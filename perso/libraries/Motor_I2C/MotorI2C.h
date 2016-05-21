/*
  MotorI2C 
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder
  
*/

/*REMARQUES - TESTS

> L'utilisation sans réglage de vitesse est possible mais sans utiliser Serial.begin(...) (problème de debug trop long ?)






*/
#ifndef MotorI2C_H
#define MotorI2C_H

// Inclusion de Wire.h si...
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif




#define MotorSpeedSet             0x82
#define PWMFrequenceSet           0x84
#define DirectionSet              0xaa
#define MotorSetA                 0xa1
#define MotorSetB                 0xa5
#define Nothing                   0x01

#define I2CMotorDriverAdd         0x0f   // Set the address of the I2CMotorDriver

#include "EDU_debug.h"



class MotorI2C{
	public:
	/*EDU FR*/	MotorI2C();
	
	/*EDU FR*/	void brancher(void);
	/*EDU US*/	void branch(void);
	
	/*EDU FR*/  void ecrireVitesse(long vitesse);
	/*EDU FR*/  void ecrireVitesse(long vitesseA, long vitesseB);	
	
	/*EDU US*/  void writeSpeed(long vitesse);
	/*EDU US*/  void writeSpeed(long vitesseA, long vitesseB);	
	
	
	
	/*EDU FR*/  void ecrireVitesseMoteurA(long vitesseA);
	/*EDU FR*/  void ecrireVitesseMoteurB(long vitesseB);	
	
	/*EDU FR*/  void reglerVitesse(long vitesse);
	/*EDU FR*/  void reglerVitesse(long vitesseA, long vitesseB);
	
	/*EDU FR*/  void vitesse(long vitesse);
	/*EDU FR*/  void vitesse(long vitesseA, long vitesseB);
	
	/*EDU US*/  void speed(long vitesse);					   // For on motor
	/*EDU US*/  void speed(long vitesseA, long vitesseB);      // For 2 motors
	
	/*EDU US*/  void setSpeed(uint8_t speedA, uint8_t speedB); // Set speed with I2C (core of the method)
	
	/*EDU FR*/  void avancer(String direction="");
	/*EDU US*/  void forward(String dir="straight");

	/*EDU FR*/  void reculer(String direction="");	
	/*EDU US*/  void back(String dir="straight");
	
	/*EDU FR*/  void pivoter(String direction="");
	/*EDU US*/  void rotate(String dir="");
	
	/*EDU FR*/  void stopper(void);
	/*EDU US*/  void stop(void);
	
	private:
	//Méthodes 
		/*EDU US*/ 	void sendI2C(uint8_t com=0);
	//Mémoire
		int16_t m_vitesseA;			// Vitesse demandée
		int16_t m_vitesseB;			// Vitesse demandée
		int16_t speedMotorA;		// Vitesse actuelle
		int16_t speedMotorB;		// Vitesse actuelle
		int8_t debug;             	     // 1=Mode débugage 0=normal
		
};
#endif
