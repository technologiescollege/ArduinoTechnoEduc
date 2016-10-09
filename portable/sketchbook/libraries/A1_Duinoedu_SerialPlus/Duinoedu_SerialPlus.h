/*
 
  Original Author: David Souder - souder.d@gmail.com
  Date de dernière modification : 03/11/2016
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

#define DUINOEDU_MEMORISER_500_MS 			500
#define DUINOEDU_MEMORISER_1000_MS 			1000

#define SERIALPLUS_MEMORISER_2000_MS 		2000
#define IRREMOTE_MEMORISER_2000_MS 		    2000
#define DUINOEDU_MEMORISER_2000_MS 			2000

#define DUINOEDU_MEMORISER_5000_MS 			5000
#define DUINOEDU_MEMORISER_10000_MS 		10000

#define SERIALPLUS_NE_PAS_MEMORISER 		0
#define IRREMOTE_NE_PAS_MEMORISER 		    0
#define DUINOEDU_NE_PAS_MEMORISER 			0

#define EDU_HC05 5
#define EDU_HC06 6


#define variable1Octet byte
#define variable2octet int
#define variableTexte  String


#define CANAL0	0
#define CANAL1	1
#define CANAL2	2
#define CANAL3	3
#define CANAL4	4
#define CANAL5	5
#define CANAL6	6
#define CANAL7	7
#define CANAL8	8
#define CANAL9	9
#define CANAL10	10
#define CANAL11	11
#define CANAL12	12
#define CANAL13	13
#define CANAL14	14
#define CANAL15	15
#define CANAL16	16
#define CANAL17	17
#define CANAL18	18
#define CANAL19	19
#define CANAL20	20
#define CANAL21	21
#define CANAL22	22
#define CANAL23	23
#define CANAL24	24
#define CANAL25	25
#define CANAL26	26
#define CANAL27	27
#define CANAL28	28
#define CANAL29	29
#define CANAL30	30
#define CANAL31	31
#define CANAL32	32
#define CANAL33	33
#define CANAL34	34
#define CANAL35	35
#define CANAL36	36
#define CANAL37	37
#define CANAL38	38
#define CANAL39	39
#define CANAL40	40
#define CANAL41	41
#define CANAL42	42
#define CANAL43	43
#define CANAL44	44
#define CANAL45	45
#define CANAL46	46
#define CANAL47	47
#define CANAL48	48
#define CANAL49	49
#define CANAL50	50
#define CANAL51	51
#define CANAL52	52
#define CANAL53	53
#define CANAL54	54
#define CANAL55	55
#define CANAL56	56
#define CANAL57	57
#define CANAL58	58
#define CANAL59	59
#define CANAL60	60
#define CANAL61	61
#define CANAL62	62
#define CANAL63	63
#define CANAL64	64
#define CANAL65	65
#define CANAL66	66
#define CANAL67	67
#define CANAL68	68
#define CANAL69	69
#define CANAL70	70
#define CANAL71	71
#define CANAL72	72
#define CANAL73	73
#define CANAL74	74
#define CANAL75	75
#define CANAL76	76
#define CANAL77	77
#define CANAL78	78
#define CANAL79	79
#define CANAL80	80
#define CANAL81	81
#define CANAL82	82
#define CANAL83	83
#define CANAL84	84
#define CANAL85	85
#define CANAL86	86
#define CANAL87	87
#define CANAL88	88
#define CANAL89	89
#define CANAL90	90
#define CANAL91	91
#define CANAL92	92
#define CANAL93	93
#define CANAL94	94
#define CANAL95	95
#define CANAL96	96
#define CANAL97	97
#define CANAL98	98
#define CANAL99	99



// A faire : bas de page

class Duinoedu_SerialPlus {

  public:
	
	//========== INITIALISATION
		/*CONSTRUCTOR*/			Duinoedu_SerialPlus();
		
		/*EDU US*/	void 		branch					(HardwareSerial *ser);				// Par un pointeur sur Serial
		/*EDU US*/	void 		branch					(SoftwareSerial *ser);				// Par un pointeur sur Softserial
		/*EDU US*/	void 		branch					(uint8_t rxPin, uint8_t txPin);		// Par 2 broches soft
				
		/*EDU US*/	void 		begin					(uint16_t speed);
	
	
	
	//==========  FONCTIONS UTILISATEUR
	
		//===== [ MODE MONOCANAL : APP INVENTOR : ]
			// Lecture d'un vrai boolean
					bool  		readBoolean				(int option=200);					// Avec prise en compte d'un temps de vie du résultat
					uint8_t  	lastValBool;
					uint16_t 	lifeTimeBool;												// Durée de vie enregistrée pour les boolean
					uint32_t 	lastTimeBool;												// Date du dernier enregistrement
					uint8_t  	trueValBool;												// La dernière valeur lue et mémorisée
	
	
			// App Inventor : byte
					byte 		recevoir1Octet			(int _option=200);
					byte 		readByte				(int _option=200);	
					uint32_t 	lastTimeByte;									    		// Date de dernier enregistrement de la valeur texte
					byte   		valByte;													// Mémorisation 
	
			// App Inventor : Integer
					int 		recevoir2Octet			(int option=200);
					int 		readInteger				(int option=200);	
					uint32_t 	lastTimeInteger;											// Date de dernier enregistrement de la valeur texte
					int   		valInteger;													// Mémorisation 
	
			// App Inventor : String
					String 		recevoirTexte			(int option=200);
					String 		readText				(int option=200);	
					uint32_t 	lastTimeText;												// Date de dernier enregistrement de la valeur texte
					String   	valText;													// Mémorisation 


			//-- Fonctions plus bas niveau
					bool  		effectiveReadBoolean	();
					int   		effectiveReadInteger	();

			// App Inventor : byte
					void 		envoyer1Octet			(byte nbr, 		int interval=200);
					void 		writeByte				(byte nbr, 		int interval=200);
		
			// App Inventor : integer
					void 		envoyer2Octet			(int nbr, 		int interval=200);
					void 		writeInteger			(int nbr, 		int interval=200);	
		
			// App Inventor : String
					void 		envoyerTexte			(String text, 	int intervalTimerAppInventor=200,	int nbrOctet=0);
					void 		writeText				(String text, 	int intervalTimerAppInventor=200,	int nbrOctet=0);
	
	
		
		//===== [ MODE MULTICANAUX : ECHANGES MULTICANAUX ]
					void 		print					(int16_t nbr, uint8_t cnl, uint16_t interval=SERIALPLUS_DEFAULT_INTERVAL);	// Envoie la donnée avec un interval minimum 
					int  		readNbr					(uint8_t can);						// Retourne la dernière valeur disponible en multicanaux (lue ou enregistrée)
	
			//-- Fonctions bas niveau
					void 		send					(int16_t nbr, uint8_t cnl);			// Inconditionnelle			
					void 		receiveNbr();												// Met à jour le tableau values[canaux]

			//-- Mémoire
					int16_t 	values[SERIALPLUS_NBR_OF_CAN];
					uint16_t   	minInterval;												// Interval minimal entre 2 transmissions (attention à l'effet de préférence)
	

		

					
	
		//===== [ FONCTION BLUETOOTH ]	
					void 		btVerbose(bool _verbose);
								bool verbose;
					
					void 		bluetoothSetNameHc06(String _btName );
					void 		bluetoothSetNameHc05(String _btName );
					
					void		btGetName();
					
		
		//===== [ FONCTIONS SERIAL DE BASE ]
					int			print(String text);
					char		read();
					int			available();
					void 		viderBufferDeReception();									// Le flush des nouvels Arduino ne vide plus le buffer de réception
					void 		flushReceptionBuffer();					
					

	//========== LES POINTEURS HARD & SOFT
			SoftwareSerial *serialSoft;
			HardwareSerial *serialHard;

	
  protected:

		uint32_t 	m_dates[SERIALPLUS_NBR_OF_CAN];											// Doit stoker la date de la dernière mesure pour tous les canaux
		uint32_t 	m_dateLastSent;
	
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

  
/* BT
#include <SoftwareSerial.h>
#include <Duinoedu_SerialPlus.h>

Duinoedu_SerialPlus  monSerialPlus;

void setup(){

  monSerialPlus.branch(8,9); 
  monSerialPlus.begin(9600);
  monSerialPlus.bluetoothSetName("DuinoEDU",EDU_HC05);
}

void loop(){

  
}


/*



//===== ENVOYER COMMANDE AT SUR HC05
#include <SoftwareSerial.h>   
SoftwareSerial BTSerie(8,9);              //Rx Tx

void setup(){  
  Serial.begin(9600);    
  delay(500);  
  Serial.println("Brancher module HC05 avec EN sur 3.3V \r\net bouton maintenu");
  Serial.println("La LED doit clignoter lentement");
  Serial.println("Attente d'une commande AT..."); 

  //pinMode(RxD, INPUT);  
  //pinMode(TxD, OUTPUT);  
  BTSerie.begin(38400);                   //57600 38400
  delay(500);  
}  

void loop(){  
  char recvChar;  

  if (BTSerie.available()){  
    recvChar = BTSerie.read();  
    Serial.print(recvChar);  
  }  
  
  if (Serial.available()) {  
    recvChar = Serial.read();  
    BTSerie.write(recvChar);  
  } 
   
}  

*/

//===== RENOMMER UN HC05
/*
#include <SoftwareSerial.h>
#include <Duinoedu_SerialPlus.h>

// PROCEDURE: https://lc.cx/oTX7 
Duinoedu_SerialPlus  monSerialPlus;

void setup()
{
  monSerialPlus.branch(8,9); 
  monSerialPlus.begin(9600);
  monSerialPlus.bluetoothSetNameHc05("DuinoEDU");
}

void loop()
{
}
*/

//===== RENOMMER UN HC06
/*
#include <SoftwareSerial.h>
#include <Duinoedu_SerialPlus.h>

// PROCEDURE: https://lc.cx/oTX7 
Duinoedu_SerialPlus  monSerialPlus;

void setup()
{
  monSerialPlus.branch(8,9); 
  monSerialPlus.begin(9600);
  monSerialPlus.bluetoothSetNameHc06("DuinoEDU");
}

void loop()
{
}
*/

// Réception de données brutes 
/*
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,9);

void setup()
{
  mySerial.begin(9600);

  Serial.begin(9600);
}

void loop()
{
  if (mySerial.available())
  {
    Serial.print(mySerial.read());
    Serial.print(" ");
    Serial.println();
  }
  else
  {
  }
}
*/

  
  
  
  




