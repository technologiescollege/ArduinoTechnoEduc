/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 25/05/2015
  www.duinoedu.com - Tous droits réservés
  
*/

#ifndef SerialPlus_H
#define SerialPlus_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


//#include <___.h>
#include <SerialPlus.h>
#include "EDU_debug.h"
#include "EDU_define.h"

//#define DEBUG
#define SERIALPLUS_NBR_OF_CAN 99

#define SERIALPLUS_DEFAULT_INTERVAL 50		// Durée minimum entre 2 transmissions par défaut	
#define SERIALPLUS_FIRST_DATE 500;			// 1ères données pas avant 500ms au démarrage 


class SerialPlus {

	public:
	
	/*EDU FR*/	SerialPlus();
	/*EDU US*/	void branch(HardwareSerial *ser);
	/*EDU US*/	void begin(uint16_t speed);
	
	//---- Fonctions utilisateur
	/*EDU US*/	void print(int16_t nbr, uint8_t cnl, uint16_t interval=SERIALPLUS_DEFAULT_INTERVAL);	// Envoie la donnée avec un interval minimum 
	/*EDU US*/	int  readNbr(uint8_t can);							// Retourne la dernière valeur disponible (lue ou enregistrée)
	
	
	//---- Fonctions bas niveau
	/*EDU US*/	void send(int16_t nbr, uint8_t cnl);				// Inconditionnelle			
	/*EDU US*/	void receiveNbr();									// Met à jour le tableau values[canaux]

	int16_t values[SERIALPLUS_NBR_OF_CAN];

	
	protected:
	HardwareSerial *m_serial;
	uint32_t m_dates[SERIALPLUS_NBR_OF_CAN];						// Doit stoker la date de la dernière mesure pour tous les canaux


};
#endif

// Trame <canal:valeur><canal:valeur>

// Exemple programme Arduino
// PARTIE EMETTEUR //////////////
/* SerialPlus exemple 
   Recevoir les infos de 2 potentiomètres sur 2 LED 
   POT1 => LED1
   POT2 => LED2
   David Souder - 19/04/2015
*/
/*
#include <SerialPlus.h>
SerialPlus monSerialPlus;

void setup(){
  monSerialPlus.branch(&Serial);
  monSerialPlus.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}

void loop(){
  monSerialPlus.print(analogRead(A0), CANAL0);
  monSerialPlus.print(analogRead(A1), CANAL1);
  Serial.print(88888888);
}
*/

// PARTIE RECEPTEUR //////////////
/* SerialPlus exemple 
   Recevoir les infos de 2 potentiomètres sur 2 LED 
   POT1 => LED1
   POT2 => LED2
   David Souder - 19/04/2015
*/
/*
#include <SerialPlus.h>
const int pinLed1 = 3; // UNO 3,5,6,9,10,11
const int pinLed2 = 5; // UNO 3,5,6,9,10,11
SerialPlus monSerialPlus;

void setup(){
  monSerialPlus.branch(&Serial);
  monSerialPlus.begin(9600);
  pinMode(pinLed1,OUTPUT);
  pinMode(pinLed2,OUTPUT);
}

void loop(){
    analogWrite(pinLed1,monSerialPlus.readNbr(CANAL0)/4); 
    analogWrite(pinLed2,monSerialPlus.readNbr(CANAL1)/4);
}
*/

  

  
  
  
  




