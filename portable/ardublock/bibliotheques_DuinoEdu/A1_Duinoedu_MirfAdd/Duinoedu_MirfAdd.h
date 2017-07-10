#ifndef Duinoedu_MirfAdd_H
#define Duinoedu_MirfAdd_H

#include <SPI.h> 
#include "Mirf.h"
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h> 


#define DEBUG(val)   //Serial.print(val)
#define DEBUGLN(val) //Serial.println(val)

/* TO DO
=> Fonctionne dans les 2 sens mais ne fonctionne plus après qq minutes.
=> Tester RF.
*/


class Duinoedu_MirfAdd : public Nrf24l {

	public:
		Duinoedu_MirfAdd					();
		
		// METODES HAUT NIVEAU
		void branch							(int8_t _cePin, int8_t _csnPin);
		void configDefaultChannelAndPayload	(int8_t _channel, int8_t _payload=4);			//Payload en octets					
		
		void sendFloat						(float _number);
		float getFloat   					();		
		
		
		void sendFloatInVirtualChanel  		(float _number, uint8_t _virtualChannel);
		float getFloatInvirtualChanel	    (uint8_t _virtualChannel);		
		
		
		
		
		void setMyName						(String _name);
				String m_myName;
				#define MY_NAME 0
		void setReceiverName				(String _name);
				String m_receiverName;
				#define RECEIVER_NAME	1
		
		 
		
		
		// METHODES DE PLUS BAS NIVEAU
		void setName						(String _name, int8_t _type);		
				
				
		
		void setChannel						(int8_t _channel);
				int8_t m_chanel;
		void setPayload						(int8_t _payload);
				int8_t m_payload;


		
		int8_t serialInfos;
		#define DATA_WIDTH 8
		//float data[DATA_WIDTH];
};

#endif

//========== EXEMPLE INITIAL
/* EXEMPLE INITIAL : EMETTEUR
#include <SPI.h>      // Pour la communication via le port SPI
#include <Mirf.h>     // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <MirfHardwareSpiDriver.h> // Pour la communication SPI
 
void setup() {
  Serial.begin(9600);
   
  Mirf.cePin = 9; // Broche CE sur D9
  Mirf.csnPin = 10; // Broche CSN sur D10
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Initialise la bibliothèque


  Mirf.channel = 0; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(float); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio


  Mirf.setTADDR((byte *) "nrf02"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf01"); // Adresse de réception
   
  Serial.println("Go !"); 
}
 
void loop() {
  
  // Lit un nombre depuis le port série
  float valeur = random(0,1000);
  
  Mirf.send((byte *) &valeur); // On envoie le message
  while(Mirf.isSending()); // On attend la fin de l'envoi
}
*/




/* EXEMPLE INITIAL : RECEPTEUR
#include <SPI.h>      // Pour la communication via le port SPI
#include <Mirf.h>     // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <MirfHardwareSpiDriver.h> // Pour la communication SPI


void setup() {
  Serial.begin(9600);


  Mirf.cePin = 9; // Broche CE sur D9
  Mirf.csnPin = 10; // Broche CSN sur D10
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Initialise la bibliothèque


  Mirf.channel = 0; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(float); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio


  Mirf.setTADDR((byte *) "nrf01"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf02"); // Adresse de réception


  Serial.println("Go !"); 
}


void loop() {
  float valeur;


  if(Mirf.dataReady()){
    Mirf.getData((byte *) &valeur); // Réception du paquet
    Serial.println(valeur); // Affiche le message
  }
}
*/









