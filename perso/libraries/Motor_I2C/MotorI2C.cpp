/*
  MotorI2C 
  2014 Copyright (c) www.duinoedu.com.  All right reserved.
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "MotorI2C.h"

#include "Wire.h"
#include <avr/pgmspace.h>


//========== << CONSTRUCTOR >>
	MotorI2C::MotorI2C(){

	}
	
	
	
//========== INITIALISATION
	void MotorI2C::brancher(void){branch();}
	void MotorI2C::branch(void){
	
		//--- On passe en mode débug
			//debug=0;
			//if(debug==1){Serial.println("==================== DEBUT INITIALISATION");}
			//if(debug==1){Serial.println("branch() : Mode debug active");}
			
		//--- On active la gestion I2C
			//if(debug==1){Serial.println("branch() : Demarrage du bus I2C");}
			Wire.begin();

		//--- On fixe la vitesse maxi des moteurs à 100%
			speedMotorA=1000; 
			speedMotorB=1000;
			//if(debug==1){Serial.println("branch() : Transmission vitesse par defaut");}
			setSpeed(speedMotorA,speedMotorB);
		
		//--- On arrête les moteurs qui pouraient déjà tourner
			//if(debug==1){Serial.println("branch() : Arret des moteurs");}
			stop();
			delay(100);
			

}

	
	
	
//========== DEPLACEMENTS
//---------- Avancer
/*EDU FR*/  void MotorI2C::avancer(String direction){
	// Traitement de la chaine en prévision
	// Choix de la direction
			if(direction=="" || direction=="devant" || direction=="tout droit" || direction=="tout_droit")   	{forward("straight");} 
	else 	if(direction=="droite")                    																						{forward("right");}
	else 	if(direction=="gauche")                    																						{forward("left");}
	else {
		//----- Aide utilisateur
		Serial.println("ERREUR : avec la fonction avancer !");
		Serial.println("avancer ne comprend que : tout_droit/devant/gauche/droite ou doit rester vide");
	}
}
/*EDU US*/	void MotorI2C::forward(String dir){
		if(dir=="straight" 	|| dir=="tout droit" || dir=="tout_doit")														{sendI2C(0b00001010);}
		if(dir=="right" 		|| dir=="droite")																							{sendI2C(0b00001000);}
		if(dir=="left"  		|| dir=="gauche")    																						{sendI2C(0b00000010);}
}
//---------- Reculer
/*EDU FR*/  void MotorI2C::reculer(String direction){
	// Traitement de la chaine en prévision
	// Choix de la direction
	     if(direction=="" || direction=="tout droit" || direction=="tout_droit")          				{back("straight");} 
	else if(direction=="droite")                    {back("right");}
	else if(direction=="gauche")                    {back("left");}
	else {
		//----- Aide utilisateur
		Serial.println("ERREUR : avec la fonction reculer !");
		Serial.println("reculer ne comprend que : gauche/droite ou doit rester vide");
	}
}
/*EDU US*/	void MotorI2C::back(String dir){
		if(dir=="straight" || dir=="tout droit" || dir=="tout_droit"){sendI2C(0b00000101);}
		if(dir=="right" || dir=="droie")   	{sendI2C(0b00000100);}
		if(dir=="left"  || dir=="gauche")  	{sendI2C(0b00000001);}
}
//---------- Pivoter
/*EDU FR*/  void MotorI2C::pivoter(String direction){
	// Traitement de la chaine en prévision
	// Choix de la direction

		 if(direction=="droite"||direction=="tout_droit")                    		{rotate("right");}
	else if(direction=="gauche")                    										{rotate("left");}
	else {
		//----- Aide utilisateur
		Serial.println("ERREUR : avec la fonction pivoter !");
		Serial.println("pivoter ne comprend que : gauche/droite et ne peut pas rester vide");
	}
}
/*EDU US*/	void MotorI2C::rotate(String dir){
		if(dir=="")        {Serial.println("Choisir une direction pour la rotation");}
		if(dir=="right")   {sendI2C(0b00000110);}
		if(dir=="left")    {sendI2C(0b00001001);}
}
//---------- Stopper
/*EDU FR*/  void MotorI2C::stopper(void){stop();}
/*EDU US*/  void MotorI2C::stop(){
		sendI2C(0b00000000);
}
			
			
			
//========== REGLAGE VITESSE 
//---------- Réglage vitesse (même vitesse pour les 2 moteurs)



/*EDU FR*/  void MotorI2C::ecrireVitesse(long vitesse){speed(vitesse);}  	// Doublon linguistique
/*EDU FR*/  void MotorI2C::reglerVitesse(long vitesse){speed(vitesse);}  	// Doublon linguistique
/*EDU FR*/  void MotorI2C::vitesse(long vitesse){speed(vitesse);}       		// Doublon linguistique
/*EDU US*/	void MotorI2C::writeSpeed(long vitesse){speed(vitesse,vitesse);}
/*EDU US*/	void MotorI2C::speed(long vitesse){speed(vitesse,vitesse);}
	
//---------- Réglage vitesse (vitesse différente sur les 2 moteurs)
/*EDU FR*/ void MotorI2C::ecrireVitesseMoteurA(long vitesseA){speed(vitesseA, speedMotorB);}
/*EDU FR*/ void MotorI2C::ecrireVitesseMoteurB(long vitesseB){speed(speedMotorA, vitesseB);}
/*EDU FR*/ void MotorI2C::ecrireVitesse(long vitesseA, long vitesseB){speed(vitesseA,vitesseB);}
/*EDU FR*/ void MotorI2C::reglerVitesse(long vitesseA, long vitesseB){speed(vitesseA,vitesseB);}
/*EDU FR*/ void MotorI2C::vitesse(long vitesseA, long vitesseB){speed(vitesseA,vitesseB);}
/*EDU FR*/ void MotorI2C::writeSpeed(long vitesseA, long vitesseB){speed(vitesseA,vitesseB);}
/*EDU US*/ void MotorI2C::speed(long vitesseA, long vitesseB){
		DEBUGLN("Entree ds speed");
		DEBUGLN("Valeurs transmises :::::");
		DEBUG2("vitesseA", vitesseA);
		DEBUG2("vitesseB", vitesseB);
		
		
		//-- Valeur par défaut en fonction des dernières valeurs en mémoire
		int16_t speedMotorA255=speedMotorA/4;
		int16_t speedMotorB255=speedMotorB/4;
		boolean actualiser=0;
		
		//--- on teste si la nouvelle vitesse A est différente de la précédente
			if(vitesseA!=speedMotorA){
			DEBUG2("speedMotorA", speedMotorA);
			//--- On signale qu'il faut actualiser
				actualiser=1;
				//if(debug==1){Serial.println("speed() : La vitesse A doit etre ajustee");}
			//--- on mémorise d'abord la nouvelle vitesse A ds la mémoire
				speedMotorA = vitesseA;
				DEBUG2("On memorise la vitesse dans speedMotorA :", speedMotorA);
				//if(debug==1){Serial.println("speed() : Vitesse A en %");}
				//if(debug==1){Serial.println(speedMotorA);}
			//--- On adapte la valeur d'actualisation
				//-- On contraint la vitesse max à 1000
					int16_t speedMotorALimited;
					if(speedMotorA>1000){speedMotorALimited=1000;}
					else{speedMotorALimited=speedMotorA;}
				//-- On actualise
				speedMotorA255 = speedMotorALimited/4;
				DEBUGLN("CALCUL VITESSE 255");
				DEBUG2("speedMotorA255", speedMotorA255);
				//if(debug==1){Serial.println("speed() : Vitesse recalculee A ");}
				//if(debug==1){Serial.println(speedMotorA255);}
			}
			
		//--- on teste si la nouvelle vitesse B est différente de la précédente
			if(vitesseB!=speedMotorB){
			DEBUG2("speedMotorB", speedMotorB);
			//--- On signale qu'il faut actualiser
				actualiser=1;
				//if(debug==1){Serial.println("speed() : La vitesse A doit etre ajustee");}
			//--- on mémorise d'abord la nouvelle vitesse A ds la mémoire
				speedMotorB = vitesseB;
				DEBUG2("On memorise la vitesse dans speedMotorB :", speedMotorB);
				//if(debug==1){Serial.println("speed() : Vitesse A en %");}
				//if(debug==1){Serial.println(speedMotorA);}
			//--- On adapte la valeur d'actualisation
				//-- On contraint la vitesse max à 1000
					int16_t speedMotorBLimited;
					if(speedMotorB>1000){speedMotorBLimited=1000;}
					else{speedMotorBLimited=speedMotorB;}
				//-- On actualise
				speedMotorB255 = speedMotorBLimited/4;
				DEBUGLN("CALCUL VITESSE 255");
				DEBUG2("speedMotorB255", speedMotorB255);
				//if(debug==1){Serial.println("speed() : Vitesse recalculee A ");}
				//if(debug==1){Serial.println(speedMotorA255);}
			}
		
		//--- Actualisation vitesse I2C 
		if(actualiser==1){
			//--- on fait appel à la fonction d'actualisation de la nouvelle vitesse
			//if(debug==1){Serial.println("Actualisation de la vitesse");}
			setSpeed(speedMotorA255, speedMotorB255);
			DEBUGLN("ACTUALISATION EFFECTIVE");
			DEBUG2("speedMotorA255",speedMotorA255);
			DEBUG2("speedMotorB255",speedMotorB255);
			//--- On signale qu'il n'est plus nécessaire d'actualiser
			actualiser=0;
			//--- On attend 100ms (préconisé par Seeed
			delay(100);
		}
		return;
	}

//---------- Actualisation vitesse par I2C 
	void MotorI2C::setSpeed(uint8_t speedA, uint8_t speedB){
		

		//if(debug==1){Serial.println("setSpeed() : Vitesse moteur A : ");}
		//if(debug==1){Serial.println(speedA);}
		//if(debug==1){Serial.println("setSpeed() : Vitesse moteur B : ");}
		//if(debug==1){Serial.println(speedB);}
		//--- On démarre la transmission vers l'esclave I2C (adresse dans I2CMotorDriveAdd)
			Wire.beginTransmission(I2CMotorDriverAdd);      // (define)
		//--- On transmet la fréquence du PWM 
			Wire.write(MotorSpeedSet);                      // (define)
			//delay(50);
		//--- On transmet la vitesse (0-255) du moteur A  
			Wire.write(speedA); 
			//delay(50);  			
		//--- On transmet la vitesse (0-255) du moteur B
			Wire.write(speedB);  
			//delay(50); 
		//--- On termine la transmission I2C
			Wire.endTransmission(); 
			
		
	}
	

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//========== GESTION I2C
/*EDU US*/ 	void MotorI2C::sendI2C(uint8_t com){
	//--- Séquence I2C
		Wire.beginTransmission(I2CMotorDriverAdd);    // Début avec adresse matériel
		Wire.write(DirectionSet);                     // Adresse registre de direction  
		Wire.write(com);                              // Code pour déplacement avant      
		Wire.write(Nothing);                          // Indispensable
		Wire.endTransmission();                       // Fin séquence I2C
}	
	


