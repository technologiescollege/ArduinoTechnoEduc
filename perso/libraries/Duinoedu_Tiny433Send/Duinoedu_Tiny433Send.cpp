/*
 
  Original Author: David Souder
  www.duinoedu.com 
  Tous droits réservés
   
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_Tiny433Send.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_Tiny433Send::Duinoedu_Tiny433Send(){
	
							
			
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PUBLIQUES
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//========== SETUP
/*EDU US*/	void 	Duinoedu_Tiny433Send::branch(uint8_t txPin, uint16_t speed){
	
	//---- On branche sur une pin en particulier
		vw_set_tx_pin(txPin);
	
	//---- On initialise la vitesse de VirtualWire
		vw_setup(speed);
		
	//---- Initialisation de diverses variables
		m_lastTime = 0;
	
}

//========== METHODES OPERATIVES



/*EDU US*/	void	Duinoedu_Tiny433Send::sendNbrInChanel(int nbr, uint8_t cnl, uint16_t interval){

	//---- On teste si ce n'est pas trop top pour renvoyer une valeur
		/*
		uint32_t now = millis();
		if(now < m_lastTime+interval){
			// On sort car c'est trop top pour envoyer des données
				return;
		}
		*/
	
	//---- On prépare le message
		String message = "";
		message += '<';
		message += "00";
		message += ':';
		message += "3210";
		message += '>';
		


	//---- On convertit le message en chaîne pointée (problème de compatibilité avec VirtualWire)
		char *messagePointe = &message[0];			// On a une adresse sur un char

		uint8_t len = message.length();				// On a une taille de chaîne
	
	//---- On lance l'envoi effectif à partir de la fonction VirtualWire en précisant la taille de la chaine
		vw_send( (uint8_t *)messagePointe, len);
		
	
	DEBUG2("f:", millis());

		
}
	


//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVEES
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





