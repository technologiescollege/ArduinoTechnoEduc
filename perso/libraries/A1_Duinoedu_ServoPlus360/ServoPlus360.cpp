/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "ServoPlus360.h"

#include <avr/pgmspace.h>


//========== << CONSTRUCTOR >>

/*EDU FR*/	ServoPlus360::ServoPlus360(){
		//----On commence par construir dynamiquement le servo de base
			servo = new Servo();
			
}


//========== SETUP

/*EDU US*/	uint8_t		ServoPlus360::brancher	(int _pin){return branch(_pin);}
/*EDU US*/	uint8_t		ServoPlus360::branch	(int _pin){return attach(_pin);}
/*EDU US*/	uint8_t		ServoPlus360::attach	(int _pin){
	
	
	//---- Réglage des valeurs par défaut
		offset=0;
		reverseRotationValue=false;
		uint16_t centerPointPulse = 1500;				// Point milieu
		uint16_t speedMax;								// centerPointPulse-speed à centerPointPulse+speed
	
	//---- On attache le véritable servo
		pin = _pin;
		return 	servo->attach(_pin);
		
	
}



//========== METHODES OPERATIVES

//----- MODE : 0 = SENS1, 500 = arrêt, 1000 = SENS2
/*EDU US*/	void 	ServoPlus360::ecrireRotation(float val1000User){writeRotation(val1000User);}
/*EDU US*/	void 	ServoPlus360::writeRotation	(float val1000User){
// METHODE NON EFFECTIVE : on pilote avec une valeur de 0~1000	
	int16_t val1000 = (int)val1000User;
	//---- On limite la plage à 0~1000 (problème du 1023)
		if(val1000<0)    val1000=0;
		if(val1000>1000) val1000=1000;
	
	//---- On calcule une largeur d'impultion à partir d'une valeur 0~1000
	uint16_t pulseWidth = pulseCalculation(val1000);
	
	writeMicroseconds(pulseWidth);
}

//----- MODE : 0 = VITESSE MIN, 1000 = VITESSE MAX
/*EDU US*/	void 	ServoPlus360::ecrireVitesse(float val1000User){writeSpeed(val1000User);}
/*EDU US*/	void 	ServoPlus360::writeSpeed	(float val1000User){
// METHODE NON EFFECTIVE : on pilote avec une valeur de 0~1000	
	int16_t val1000 = (int)val1000User;
	//---- On limite la plage à 0~1000 (problème du 1023)
		if(val1000<0)    val1000=0;
		if(val1000>1000) val1000=1000;
	
	//---- On calcule une largeur d'impultion à partir d'une valeur 0~1000
	uint16_t pulseWidth = pulseCalculation2(val1000);
	
	writeMicroseconds(pulseWidth);
}

//----- METHODE EFFECTIVE
/*EDU US*/	void 	ServoPlus360::writeMicroseconds	(uint16_t _pulseDelay){
// METHODE EFFECTIVE : On écrit directement une largeur d'impultion
	
	writeMicrosecondsValue=_pulseDelay;
	servo->writeMicroseconds(_pulseDelay);
	
}

/*EDU FR*/	void 	ServoPlus360::desactiver(){detach();}
/*EDU US*/	void 	ServoPlus360::detach(){
// METHODE EFFECTIVE : On débranche le servo (désactivation immédiate)
	
	servo->detach();
	
}

/*EDU US*/	void 	ServoPlus360::reactiver(){reattach();}
/*EDU US*/	void 	ServoPlus360::reattach(){
// METHODE EFFECTIVE : On débranche le servo (désactivation immédiate)
	//---- On rebranche sur la broche mémorisée 
		servo->attach(pin);
	//---- On force la largeur d'impultion effective
		servo->writeMicroseconds(writeMicrosecondsValue);
	
}




//========== MODIFICATION D'OPTIONS

/*EDU US*/	void 	ServoPlus360::reglerDecalage(int16_t offsetUser){writeOffset(offsetUser);}
/*EDU US*/	void 	ServoPlus360::writeOffset(int16_t offsetUser){
	offset = offsetUser;
}	

/*EDU US*/	void 	ServoPlus360::inverserRotation    (boolean rotationUser){reverseRotation(rotationUser);}				// Inverse le sens de rotation
/*EDU US*/	void 	ServoPlus360::reverseRotation	  (boolean rotationUser){
	reverseRotationValue = rotationUser;
}




//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//----- MODE : 0 = SENS1, 500 = arrêt, 1000 = SENS2
/*EDU US*/	uint16_t 	ServoPlus360::pulseCalculation	(uint16_t val1000){

	const int centerPointPulse = 1525 + offset;					// Point milieu réel du servo en largeur d'impulsion	
	const int centerPoint1000  = 512;							// Imposé et aligné avec centerPointPulse
	uint16_t pulseWidth;
	
	// Exemple fenêtre : 0-----500-----1000
	
	if(reverseRotationValue==false){
		pulseWidth = centerPointPulse+(val1000-centerPoint1000);
	}else{
		pulseWidth = centerPointPulse-(val1000-centerPoint1000);
	}	
	
	return pulseWidth;
}

//----- MODE : 0 = VITESSE MIN, 1000 = VITESSE MAX
/*EDU US*/	uint16_t 	ServoPlus360::pulseCalculation2	(uint16_t val1000){
	const int centerPointPulse = 1525 + offset;					// Point milieu réel du servo en largeur d'impulsion	
	uint16_t pulseWidth;
	
	if(reverseRotationValue==false){
		pulseWidth = centerPointPulse+(val1000/2);
	}else{
		pulseWidth = centerPointPulse-(val1000/2);
	}
return pulseWidth;
}

