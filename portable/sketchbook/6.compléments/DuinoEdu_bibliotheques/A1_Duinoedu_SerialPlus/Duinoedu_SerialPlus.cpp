/*
 
  Original Author: David Souder souder.d@gmail.com
  
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_SerialPlus.h"


#include <avr/pgmspace.h>

#define SERIAL_PLUS_BEGIN(arg)      	(serialSoft!=0)?serialSoft->begin(arg):serialHard->begin(arg)
#define SERIAL_PLUS_PRINT(arg)      	(serialSoft!=0)?serialSoft->print(arg):serialHard->print(arg)
#define SERIAL_PLUS_PRINTLN(arg)    	(serialSoft!=0)?serialSoft->println(arg):serialHard->println(arg)
#define SERIAL_PLUS_READ(arg)       	(serialSoft!=0)?serialSoft->read(arg):serialHard->read(arg)
#define SERIAL_PLUS_AVAILABLE()    		(serialSoft!=0)?serialSoft->available():serialHard->available()
#define SERIAL_PLUS_PARSEINT()      	(serialSoft!=0)?serialSoft->parseInt():serialHard->parseInt()
#define SERIAL_PLUS_FLUSH()         	(serialSoft!=0)?serialSoft->flush():serialHard->flush()
#define SERIAL_PLUS_READSTRING()    	(serialSoft!=0)?serialSoft->readString():serialHard->readString()
#define SERIAL_PLUS_READBYTESUNTIL()    (serialSoft!=0)?serialSoft->readBytesUntil():serialHard->readBytesUntil()
#define SERIAL_PLUS_WRITE(arg)    		(serialSoft!=0)?serialSoft->write(arg):serialHard->write(arg)

//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_SerialPlus::Duinoedu_SerialPlus(){
	//---- On initialise les pointeurs à NULL pour tester leur existance plus tard
		serialSoft = NULL;
		serialHard = NULL;
		
	//---- Bluetooth
		verbose = true;
		
	//---- Autres 
		minInterval = 0;  // Interval mini entre 2 envois
	

			
}

//=== SI SOFT SERIAL
//-- Par 2 broches
/*EDU US*/	void Duinoedu_SerialPlus::branch(uint8_t rxPin, uint8_t txPin){
	serialSoft = new SoftwareSerial(rxPin, txPin);
	
	//---- On termine l'initialisation commune à tous les modes d'initialisation
		endBranch();
}
//-- Par pointeur
/*EDU US*/	void Duinoedu_SerialPlus::branch(SoftwareSerial *ser){
	//---- On enregistre l'adresse du port série soft réel 
		serialSoft=ser;
	
	//---- On termine l'initialisation commune à tous les modes d'initialisation
		endBranch();
		
	//---- On active le port série	
		Serial.begin(9600);
		
}//*/


//=== SI HARD SERIAL
/*EDU US*/	void Duinoedu_SerialPlus::branch(HardwareSerial *ser){
	//---- On enregistre l'adresse du port série réel dans le pointeur protected
		serialHard=ser;
	
	//---- On termine l'initialisation commune à tous les modes d'initialisation
		endBranch();
		

}

//=== Fin branchement (bas niveau)
/*EDU US*/	void Duinoedu_SerialPlus::endBranch(){
	//---- On initialise les dates à partir desquelles on peut transmettre
		for(uint8_t i=0; i<SERIALPLUS_NBR_OF_CAN;i++){
			m_dates[i]=SERIALPLUS_FIRST_DATE;
		}
	//---- Valeur par défaut
		
		lastValBool =-10;			// Dernière valeur effectivement retorunée (même si ce n'est pas la valeur réellement lue)	
		lastTimeBool=0;				// Date de la dernière valeur lue
		lifeTimeBool=200;			// La durée de vie par défaut d'une valeur
		
		trueValBool =-1;			// La denière valeur réellement lue (même si ce n'est pas la valeur réellement retournée)
		
		lastTimeText=0;
		valText="0";
		
		lastTimeByte=0;
		valByte=0;
		
		lastTimeInteger=0;
		valInteger=0;		
		
		
		Serial.setTimeout(10);
}



/*EDU US*/	void Duinoedu_SerialPlus::begin(uint16_t speed){
	//---- On paramètre la vitesse du port série réel
		SERIAL_PLUS_BEGIN(speed);

}



//---- Méthodes utilisateur
//=== MODE MONOCANAL : APP INVENTOR

/*EDU US*/	bool Duinoedu_SerialPlus::readBoolean(int option){

	
	byte bufferVal = false;


	//--- Cas de la lecture en live => gestion classique du port série
		if(option==0){
			if(SERIAL_PLUS_AVAILABLE()){
				uint8_t val;
				val = SERIAL_PLUS_READ();
				SERIAL_PLUS_FLUSH();
				return val;
			}else{
				return false;
			}
		}
	
	//--- Ca où il y a un effet mémoire (gestion statistique)
	//--- On vide le buffer et on regarde quelle valeur ressort le plus (0 ou 1 ?)
		if(SERIAL_PLUS_AVAILABLE()){
			uint8_t iterations = 0;
			uint8_t tempVal = 0;
			//Serial.print("it");
			while(SERIAL_PLUS_AVAILABLE()){
				bufferVal = SERIAL_PLUS_READ();
				(bufferVal==1)?tempVal++:tempVal=tempVal;
				iterations++;	
			}
		//-- Le buffer est vide
		//-- Détermination du vrai 1 ou du vrai 0, stokage et enregistrement de la date de dernière vrai valeur
			//Serial.println(iterations);
		    if(iterations==1){
				//- La vrai valeur est directement accessible
				trueValBool=tempVal;
			}else{ 
				( (tempVal/iterations) >= (iterations/2) ) ? trueValBool=1 : trueValBool=0; 
			}
			//- On enregistre la date de la dernière vrai valeur lue
			lastTimeBool = millis();
		}
		//-- Le buffer est vide
		//-- A ce stade, on sait si on a un vrai 1 ou 1 vrai 0 ou sinon, on l'a mémorisé

	

	//--- On regarde s'il faut envoyer la vrai valeur ou la dernière valeur mémorisée (option = 0)
		//-- Cas sans effet mémoire
			// Géré au début
		
		//-- Cas avec mémorisation de la dernière valeur reçue 
			if(option==-1){
				//Serial.println("opt0");
				if(trueValBool==1){
					return true;
				}else{
					return false;
				}
			}
		//-- Cas où il faut mémoriser un certain temps le 1
	
			if(option>0){
				//Serial.println("opt>0");
				if(trueValBool==1 && millis() < (lastTimeBool+option) ){
					return true;
				}else{
					return false;
				}
			}
}


/*EDU FR*/  byte Duinoedu_SerialPlus::recevoir1Octet(int _option){return readByte(_option);}
/*EDU US*/	byte Duinoedu_SerialPlus::readByte(int _option){
	//--- Lecture d'un octet
		static byte returnVal=0;
		//-- Si des données sont disponibles, on les transmet tout de suite
			if(SERIAL_PLUS_AVAILABLE()){
				valByte = SERIAL_PLUS_READ();
				//Serial.print("valByte:");Serial.println(valByte);
				delayMicroseconds(20);
				lastTimeByte= millis();
				SERIAL_PLUS_FLUSH();
				returnVal = valByte;
				}
		//-- Mais que faire si plus aucune donnée n'est disponible
			
			else{
				
				//- MODE SANS EFFET MEMOIRE : Retourner 0
				if(_option==0){
					return 0;
				}
				
				//- MODE AVEC EFFET MEMOIRE MOMENTANE : Retourner dernière valeur pendant un certain temps, puis 0
				else if(_option>0){
					//Serial.println("op>0");
					if( millis() < lastTimeByte+_option ){
						//Serial.print("rt:");Serial.print(valText);
						return valByte;
					}else{
						return 0;
					}
				}
				
				//- MODE AVEC EFFET MEMOIRE PERMANANT : Retourner la dernière valeur 
				else if(_option==-1){
					return valByte;
				}				
			}
			
	return returnVal;
}

/*EDU FR*/  int Duinoedu_SerialPlus::recevoir2Octet(int option){return readInteger(option);}
/*EDU US*/	int Duinoedu_SerialPlus::readInteger(int option){
	//--- Lecture d'un octet
		//-- Si des données sont disponibles, on les transmet tout de suite
			if(SERIAL_PLUS_AVAILABLE()>0){
				byte lsb = SERIAL_PLUS_READ(); 
				delayMicroseconds(50);
				//Serial.println(lsb, DEC);
				byte msb = SERIAL_PLUS_READ();
				//delayMicroseconds(20);
				//Serial.println(msb, DEC);
				valInteger = msb*256;
				valInteger += lsb;
				//Serial.print("valInt:");Serial.println(valInteger);
				lastTimeInteger= millis();
				SERIAL_PLUS_FLUSH();
				return valInteger;
				}
		//-- Mais que faire si plus aucune donnée n'est disponible
			else{
				
				//- MODE SANS EFFET MEMOIRE : Retoruner 0
				if(option==0){
					return 0;
				}
				
				//- MODE AVEC EFFET MEMOIRE MOMENTANE : Retourner dernière valeur pendant un certain temps, puis 0
				else if(option>0){
					//Serial.println("op>0");
					if(millis()<lastTimeInteger+option){
						//Serial.print("rt:");Serial.print(valText);
						return valInteger;
					}else{
						return 0;
					}
				}
				
				//- MODE AVEC EFFET MEMOIRE PERMANANT : Retourner la dernière valeur 
				else if(option==-1){
					return valInteger;
				}				
			}
}

/*EDU US*/	String Duinoedu_SerialPlus::recevoirTexte(int option){return readText(option);}
/*EDU US*/	String Duinoedu_SerialPlus::readText(int option){
	
	//--- Lecture d'une chaîne
		//-- Si des données sont disponibles, on les transmet tout de suite
			if(SERIAL_PLUS_AVAILABLE()>1){
				valText = SERIAL_PLUS_READSTRING();
				lastTimeText = millis();
				SERIAL_PLUS_FLUSH();
				return valText;
				}
		//-- Mais que faire si plus aucune donnée n'est disponible
			else{
				
				//- MODE SANS EFFET MEMOIRE : Retoruner 0
				if(option==0){
					return "0";
				}
				
				//- MODE AVEC EFFET MEMOIRE MOMENTANE : Retourner dernière valeur pendant un certain temps, puis 0
				else if(option>0){
					//Serial.println("op>0");
					if(millis()<lastTimeText+option){
						//Serial.print("rt:");Serial.print(valText);
						return valText;
					}else{
						return "0";
					}
				}
				
				//- MODE AVEC EFFET MEMOIRE PERMANANT : Retourner la dernière valeur 
				else if(option==-1){
					return valText;
				}				
			}	
}




/*EDU US*/	bool Duinoedu_SerialPlus::effectiveReadBoolean(){
	bool nbrBool = false;
	byte nbrByte = 0;
	if(SERIAL_PLUS_AVAILABLE()){
		nbrByte = SERIAL_PLUS_READ();
		//Serial.println(nbrByte);
	}
	(nbrByte>0)?nbrBool=true:nbrBool=false;

return nbrBool;

}

/*EDU US*/	int  Duinoedu_SerialPlus::effectiveReadInteger(){
	uint16_t nbr = -1;
	if(SERIAL_PLUS_AVAILABLE()){
		nbr = SERIAL_PLUS_PARSEINT();
	}

	return nbr;
}


/*EDU US*/	void Duinoedu_SerialPlus::envoyer1Octet(byte nbr, int intervalTimerAppInventor){writeByte(nbr,intervalTimerAppInventor);}
/*EDU US*/	void Duinoedu_SerialPlus::writeByte(byte nbr, int intervalTimerAppInventor){
	
	//---- Gestion de l'intervalle des envois
	static uint32_t lastTime = 0;
	uint16_t interval = intervalTimerAppInventor*3; 
	if(millis()>lastTime+interval){	
		SERIAL_PLUS_WRITE(nbr);
		lastTime=millis();
		//Serial.println(lastTime);
	}
}

/*EDU US*/	void Duinoedu_SerialPlus::envoyer2Octet(int nbr, int intervalTimerAppInventor){writeInteger(nbr,intervalTimerAppInventor);}
/*EDU US*/	void Duinoedu_SerialPlus::writeInteger(int nbr, int intervalTimerAppInventor){
	static uint32_t lastTime = 0;
	//Serial.print("nbr:");Serial.println(nbr);
	uint16_t number = nbr;
	byte mso = number/256;
	//Serial.print("m:");Serial.println(mso, DEC);
	byte lso = nbr%256;
	//Serial.print("l:");Serial.println(lso, DEC);
	
	//---- Gestion de l'intervalle des envois
	uint16_t interval = intervalTimerAppInventor*3;
	if(millis()>lastTime+interval){	
		SERIAL_PLUS_WRITE(lso);
		SERIAL_PLUS_WRITE(mso);

		
		lastTime=millis();
		//Serial.println(lastTime);
	}
}

/*EDU US*/	void Duinoedu_SerialPlus::envoyerTexte(String text, int intervalTimerAppInventor,  int nbrOctet){writeText(text, intervalTimerAppInventor, nbrOctet);}
/*EDU US*/	void Duinoedu_SerialPlus::writeText(String text, int intervalTimerAppInventor,  int nbrOctet){
	// App inventor : on veut un nombre d'octets 
	// 10 octets = 8 caractères utiles
	// Il faut envoyer 3 x moins vite que le timer App inv.
	
	//---- Formatage de la chaîne
	if(nbrOctet!=0){
		//---- Si on impose une longueur de chaîne
		uint8_t myLength = nbrOctet/*-2*/;		// Propre à App inventor
		//--- Trop longue ?
		if(text.length()>myLength){
			//-- La chaîne est trop longue. Il faut la raccourcir
			text=text.substring(0,myLength);
			Serial.println(text);Serial.println(text);
		
		}
		//--- Trop courte ?
		else if(text.length()<myLength){
			//-- Il faut compléter avec le caractère NULL
			do{
				char null= ' ';     //NULL
				text=text+null;
			}while(text.length()<myLength);
			Serial.println(text);
		}
	}
		
	
	//---- Gestion de l'intervalle des envois
	static uint32_t lastTime = 0;
	uint16_t interval = intervalTimerAppInventor*5;
	if(millis()>lastTime+interval){	
		SERIAL_PLUS_PRINT(text);
		lastTime=millis();
		//Serial.println(lastTime);
	}

}




//=== MODE MULTICANAUX : ECHANGES MULTICANAUX
/*EDU US*/	void Duinoedu_SerialPlus::print(int16_t nbr, uint8_t cnl, uint16_t interval){
	
	//---- On regarde s'il n'est pas trop top pour envoyer une donnée peu importe le canal
		if(millis()<m_dateLastSent+minInterval){
			return;
		}

	//---- On regarde s'il n'est pas trop top pour envoyer une nouvelle donnée pour le canal en question
		if(millis()>=m_dates[cnl]+interval){
			//-- On envoie une nouvelle donnée
				send(nbr, cnl);
			//-- On enregistre la date à laquelle la donnée à été envoyée
				m_dates[cnl]=millis();
			//-- On enregistrer la dernière date à laquelle qq chose a été enregistré
				m_dateLastSent = m_dates[cnl];
					//Serial.println(m_dates[cnl]);
		}else{
			//-- C'est trop tôt pour envoyer 1 nouvelle donnée
					//Serial.println("t top");
			return;
			
		}
	
	
}

/*EDU US*/	int  Duinoedu_SerialPlus::readNbr(uint8_t can){
	//---- On commence par rafraîchir les valeurs si possible
		receiveNbr();
		delay(2);
	//---- On retourne la valeur pointée par le canal demandé
		return values[can];
}


//---- Méthode plus bas niveau
/*EDU US*/	void Duinoedu_SerialPlus::send(int16_t nbr, uint8_t cnl){
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
		
		SERIAL_PLUS_PRINT(trame);		// Ecriture effective

			
}

/*EDU US*/	void Duinoedu_SerialPlus::receiveNbr(){
	char tempC='0';
	String tempS="";
	uint8_t i=0;
	//const uint8_t readMax = 50;
	//uint8_t j=50;
	uint8_t curentCan=0;
	uint16_t curentVal=0;
	
	
	//01 RECHERCHE MOTIF DE DEPART (1 trame contient tjrs 9 caractères)
	receiveNbr_reloop:
	
	if(SERIAL_PLUS_AVAILABLE()>=5){				// Le + petite trame est à au moins 5 caractères - Ex : <0:1>
		tempC = SERIAL_PLUS_READ();
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
			if(SERIAL_PLUS_AVAILABLE()>0){ 
				tempC = SERIAL_PLUS_READ();
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
	

//== BLUETOOTH
/*EDU US*/	void Duinoedu_SerialPlus::btVerbose(bool _verbose){
	verbose = _verbose;
	(verbose)?Serial.println("btVerbose:on"):Serial.println("btVerbose:off");
}


/*EDU US*/	void Duinoedu_SerialPlus::bluetoothSetNameHc06(String _btName){
	Serial.begin(9600);
	delay(500);
	Serial.println("MODIFICATION OPTIONS BLUETOOTH");
	//Serial.println("SUIVRE PROCEDURE SUR: https://lc.cx/oTcT ");
	SERIAL_PLUS_BEGIN(9600);									//Les HC06 sont à 9600 par défaut
	delay(500);
	//---- Envoid de la commande AT
		Serial.println("Envoi du nouveau nom: "+_btName);
		SERIAL_PLUS_PRINT("AT+NAME");
		SERIAL_PLUS_PRINT(_btName);
		//SERIAL_PLUS_PRINT("\r\n");							// Pas \r\n sur les HC06
		delay(1000);
	//---- Affichage de la réponse
	char   recvChar;
	String recvString = "";
	while(SERIAL_PLUS_AVAILABLE()){  
		recvChar = SERIAL_PLUS_READ();
		recvString += recvChar;			
	} 
	Serial.print(recvString);
	if(recvString=="OKsetname"){
		Serial.print("\r\n");
		Serial.println("RENOMMAGE TERMINE");
	}
 	return;
}	

/*EDU US*/	void Duinoedu_SerialPlus::bluetoothSetNameHc05(String _btName){
	Serial.begin(9600);
	delay(500);
	Serial.println("MODIFICATION OPTIONS BLUETOOTH");
	Serial.println("Le module doit clignoter lentement");
	Serial.println("SUIVRE PROCEDURE SUR: https://lc.cx/oTcT ");
	SERIAL_PLUS_BEGIN(38400);									//Les HC05 sont à 38400 par défaut
	delay(500);
	//---- Envoid de la commande AT
		Serial.println("Envoi du nouveau nom: "+_btName);
		SERIAL_PLUS_PRINT("AT+NAME=");
		SERIAL_PLUS_PRINT(_btName);
		SERIAL_PLUS_PRINT("\r\n");
		delay(1000);
	//---- Affichage de la réponse
	char   recvChar;
	String recvString = "";
	while(SERIAL_PLUS_AVAILABLE()){  
		recvChar = SERIAL_PLUS_READ();
		recvString += recvChar;			
	} 
	Serial.print(recvString);
	if(recvString=="OK\r\n"){
		Serial.println("RENOMMAGE TERMINE");
		Serial.println("REBRANCHEZ LE MODULE BLUETOOTH AVEC UNIQUEMENT LE CABLE GROVE");
	}
 	return;
}	

	

	
	
	
	
//== FONCTIONS SERIAL DE BASE

/*EDU US*/	int Duinoedu_SerialPlus::print(String text){
	return SERIAL_PLUS_PRINT(text);
}

/*EDU US*/	char Duinoedu_SerialPlus::read(){
	return SERIAL_PLUS_READ();
}		

/*EDU US*/	int Duinoedu_SerialPlus::available(){
	return SERIAL_PLUS_AVAILABLE();
}	

/*EDU FR*/	void Duinoedu_SerialPlus::viderBufferDeReception(){flushReceptionBuffer();}					
/*EDU US*/	void Duinoedu_SerialPlus::flushReceptionBuffer(){
	while (SERIAL_PLUS_AVAILABLE()){
		SERIAL_PLUS_READ();
	}
}


	
	
	
//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





