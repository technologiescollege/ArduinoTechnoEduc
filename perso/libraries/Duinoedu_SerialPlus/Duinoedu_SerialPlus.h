/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 25/05/2015
  www.duinoedu.com - Tous droits réservés
  
*/

#ifndef Duinoedu_SerialPlus_H
#define Duinoedu_SerialPlus_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
//#include "Wire.h"
#endif


//#include <___.h>
#include <SoftwareSerial.h>
//#include "EDU_debug.h"
//#include "EDU_define.h"

//#define DEBUG
#define SERIALPLUS_NBR_OF_CAN 99

#define SERIALPLUS_DEFAULT_INTERVAL 50		// Durée minimum entre 2 transmissions par défaut	
#define SERIALPLUS_FIRST_DATE 500;			// 1ères données pas avant 500ms au démarrage 

//#define brancher branch
//#define demarrer begin

#define SERIALPLUS_MEMORISER 				-1
#define IRREMOTE_MEMORISER_TOUCHE			-1
#define IRREMOTE_MEMORISER      			-1
#define DUINOEDU_MEMORISER_TOUCHE			-1
#define DUINOEDU_MEMORISER					-1

#define SERIALPLUS_MEMORISER_200_MS 		200
#define IRREMOTE_MEMORISER_200_MS 		    200
#define DUINOEDU_MEMORISER_200_MS 		    200

#define SERIALPLUS_MEMORISER_2000_MS 		2000
#define IRREMOTE_MEMORISER_2000_MS 		    2000
#define DUINOEDU_MEMORISER_2000_MS 			2000

#define SERIALPLUS_NE_PAS_MEMORISER 		0
#define IRREMOTE_NE_PAS_MEMORISER 		    0
#define DUINOEDU_NE_PAS_MEMORISER 			0

#define variable1Octet byte
#define variable2octet int
#define variableTexte String

// A faire : bas de page

class Duinoedu_SerialPlus {

  public:
	
	/*EDU FR*/	Duinoedu_SerialPlus();
	/*EDU US*/	void branch(HardwareSerial *ser);
	/*EDU US*/	void branch(SoftwareSerial *ser);	
	/*EDU US*/	void branch(uint8_t rxPin, uint8_t txPin);			// Par 2 broches soft
				
	/*EDU US*/	void begin(uint16_t speed);
	
	//---- Fonctions utilisateur
	
	//== MODE MONOCANAL
	// Lecture d'un vrai boulean
	/*EDU US*/	bool  readBoolean(int option=200);					// Avec prise en compte d'un temps de vie du résultat
		uint8_t  lastValBool;
		uint16_t lifeTimeBool;										// Durée de vie enregistrée pour les boolean
		uint32_t lastTimeBool;										// Date du dernier enregistrement
		uint8_t  trueValBool;										// La dernière valeur lue et mémorisée



	/*EDU FR*/  byte recevoir1Octet(int option=200);
	/*EDU US*/	byte readByte(int option=200);	
		uint32_t lastTimeByte;									    // Date de dernier enregistrement de la valeur texte
		byte   valByte;												// Mémorisation 
	
	/*EDU FR*/  int recevoir2Octet(int option=200);
	/*EDU US*/	int readInteger(int option=200);	
		uint32_t lastTimeInteger;									// Date de dernier enregistrement de la valeur texte
		int   valInteger;											// Mémorisation 
	
	/*EDU FR*/	String recevoirTexte(int option=200);
	/*EDU US*/	String readText(int option=200);	
		uint32_t lastTimeText;										// Date de dernier enregistrement de la valeur texte
		String   valText;											// Mémorisation 


		
	/*EDU FR*/	bool  effectiveReadBoolean();
	/*EDU US*/	int   effectiveReadInteger();

	/*EDU FR*/	void viderBufferDeReception();						// Le flush des nouvels Arduino ne vide plus le buffer de réception
	/*EDU US*/	void flushReceptionBuffer();

	
	/*EDU FR*/	void envoyer1Octet	(byte nbr, 		int interval=200);
	/*EDU US*/	void writeByte		(byte nbr, 		int interval=200);
	
	/*EDU FR*/	void envoyer2Octet	(int nbr, 		int interval=200);
	/*EDU US*/	void writeInteger	(int nbr, 		int interval=200);	
	
	/*EDU US*/	void envoyerTexte	(String text, 	int intervalTimerAppInventor=200,	int nbrOctet=0);
	/*EDU US*/	void writeText		(String text, 	int intervalTimerAppInventor=200,	int nbrOctet=0);
	
	
	//== MODE MULTICANAUX
	/*EDU US*/	void print(int16_t nbr, uint8_t cnl, uint16_t interval=SERIALPLUS_DEFAULT_INTERVAL);	// Envoie la donnée avec un interval minimum 
	/*EDU US*/	int  readNbr(uint8_t can);							// Retourne la dernière valeur disponible en multicanaux (lue ou enregistrée)
	
	
	//---- Fonctions bas niveau
	/*EDU US*/	void send(int16_t nbr, uint8_t cnl);				// Inconditionnelle			
	/*EDU US*/	void receiveNbr();									// Met à jour le tableau values[canaux]

	
	
	int16_t values[SERIALPLUS_NBR_OF_CAN];
	uint16_t   minInterval;											// Interval minimal entre 2 transmissions (attention à l'effet de préférence)
	
	//---- Les pointeurs Soft et Hard
		SoftwareSerial *serialSoft;
		HardwareSerial *serialHard;

	
  protected:

	uint32_t m_dates[SERIALPLUS_NBR_OF_CAN];						// Doit stoker la date de la dernière mesure pour tous les canaux
	uint32_t m_dateLastSent;

	
	
	
	void endBranch();
	

};
#endif

// A FAIRE
/*
	Gestion par serialEvent() => https://digistump.com/board/index.php?topic=966.0


// Trame <canal:valeur><canal:valeur> attente <canal:valeur><canal:valeur>

// Exemple programme Arduino
// PARTIE EMETTEUR //////////////
/* Duinoedu_SerialPlus exemple 
   Recevoir les infos de 2 potentiomètres sur 2 LED 
   POT1 => LED1
   POT2 => LED2
   David Souder - 19/04/2015
*/
/*
#include <Duinoedu_SerialPlus.h>
Duinoedu_SerialPlus monSerialPlus;

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
/* Duinoedu_SerialPlus exemple 
   Recevoir les infos de 2 potentiomètres sur 2 LED 
   POT1 => LED1
   POT2 => LED2
   David Souder - 19/04/2015
*/
/*
#include <Duinoedu_SerialPlus.h>
const int pinLed1 = 3; // UNO 3,5,6,9,10,11
const int pinLed2 = 5; // UNO 3,5,6,9,10,11
Duinoedu_SerialPlus monSerialPlus;

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

  

  
  
  
  




