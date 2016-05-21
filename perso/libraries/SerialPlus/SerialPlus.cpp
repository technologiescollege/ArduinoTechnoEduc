/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "SerialPlus.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	SerialPlus::SerialPlus(){
			
}

/*EDU US*/	void SerialPlus::branch(HardwareSerial *ser){
	//---- On enregistre l'adresse du port série réel dans le pointeur protected
		m_serial=ser;
	//---- On initialise les dates à partir desquelles on peut transmettre
		for(uint8_t i=0; i<SERIALPLUS_NBR_OF_CAN;i++){
			m_dates[i]=SERIALPLUS_FIRST_DATE;
		}
}

/*EDU US*/	void SerialPlus::begin(uint16_t speed){
	//---- On paramètre la vitesse du port série réel
		m_serial->begin(9600);
}

//---- Méthodes utilisateur
/*EDU US*/	void SerialPlus::print(int16_t nbr, uint8_t cnl, uint16_t interval){
	//---- On regarde s'il n'est pas trop top pour envoyer une nouvelle donnée
		if(millis()>=m_dates[cnl]+interval){
			//-- On envoie une nouvelle donnée
				send(nbr, cnl);
			//-- On enregistre la date à laquelle la donnée à été envoyée
				m_dates[cnl]=millis();
					//Serial.println(m_dates[cnl]);
		}else{
			//-- C'est trop tôt pour envoyer 1 nouvelle donnée
					//Serial.println("t top");
			return;
			
		}
	
	
}
/*EDU US*/	int SerialPlus::readNbr(uint8_t can){
	//---- On commence par rafraîchir les valeurs si possible
		receiveNbr();
		delay(2);
	//---- On retourne la valeur pointée par le canal demandé
		return values[can];
}


//---- Méthode plus bas niveau
/*EDU US*/	void SerialPlus::send(int16_t nbr, uint8_t cnl){
		uint8_t tempCan=cnl;
		int16_t tempNbr=nbr;
	//---- Ecriture d'une trame [ <c=v><0=512><10=1023> ]
		String trame ="";
		trame+='<';						// Motif de départ (ASCII 62)
		trame+=tempCan;					// Canal => 0 (ASCII 48)
		trame+='=';						// Séparateur (:)
		trame+=tempNbr;					// 100x => 49 48 48 (ASCII 48~57)
		trame+='>';						// Motif de fin    (ASCII 58)
										// Caractère de fin de chaîne ('\0')
		
		m_serial->print(trame);		// Ecriture effective
			
}

/*EDU US*/	void SerialPlus::receiveNbr(){
	char tempC='0';
	String tempS="";
	uint8_t i=0;
	//const uint8_t readMax = 50;
	//uint8_t j=50;
	uint8_t curentCan=0;
	uint16_t curentVal=0;
	
	
	//01 RECHERCHE MOTIF DE DEPART (1 trame contient tjrs 9 caractères)
	receiveNbr_reloop:
	
	if(m_serial->available()>=5){				// Le + petite trame est à au moins 5 caractères - Ex : <0:1>
		tempC = m_serial->read();
		if(tempC!='<'){
			goto receiveNbr_reloop;				// Caractère de départ non trouvé
		}else{
			tempS+=tempC;
		}
		
	}else{
			return;						        // Buffer trop vide pour contenir 1 trame
	}

	// A ce stade, le motif de départ a été trouvé et le buffer est assez chargé pour contenir 1 trame
	// => tempS == "<"
	
	//02 STOCKAGE DE LA CHAINE
		uint32_t watchDogTimeOut = millis()+ 11;
		while(tempC!='>' && i<9){
			if(m_serial->available()>0){ 
				tempC = m_serial->read();
				tempS+= tempC;
				i++;
			}
			if (millis()>watchDogTimeOut) break;
		}
	// A ce stade, une trame est chargée
	// => temps == "<0=152">" 
	

		//03 VALIDATION DE LA CHAINE
			
			// Test de longueur - Ex min <0=0>; max <99=9999>
				/**/ if (tempS.length()<5 || tempS.length()>9) 	{goto receiveNbr_reloop;}
			// Test de présence du chevron fermé
				/**/ if (tempS.indexOf('>')==-1) 				{goto receiveNbr_reloop;}
			// Test de présence du séparateur ('=')
				/**/ if (tempS.indexOf('=')==-1)				{goto receiveNbr_reloop;}
		
	
		//04 EXTRACTION CANAL & VALEUR
			// Valeurs brutes
				String canS = tempS.substring(tempS.indexOf('<')+1, tempS.indexOf('='));  // [0_]
				String valS = tempS.substring(tempS.indexOf('=')+1, tempS.indexOf('>'));  // [12xx]
		
	
		//05 VALIDATION DES DONNEES
			// Le canal est-il 1 nbr ?
				for(byte i=0;i<2-1;i++){
					char c = canS[i];
					if(c<48 || c>57){goto receiveNbr_reloop;}				// Le canal n'est pas 1 nbr
				}
		// La valeur est-elle 1 nbr ?
			for(byte i=0;i<valS.length();i++){
				char c = valS[i];
				if(c<48 || c>57){goto receiveNbr_reloop;}					// La valeur n'est pas 1 nbr
			}
	
	//06 STOCKAGE DES DONNEES
			uint8_t canI  =  canS.toInt();
			uint16_t valI =  valS.toInt();
			values[canS.toInt()] = valS.toInt();
		
		
	//Serial.println(tempS);
	//Serial.println(canS);		
	//Serial.println(valS);	
	//Serial.println(canI);		
	//Serial.println(valI);	


}
	
//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





