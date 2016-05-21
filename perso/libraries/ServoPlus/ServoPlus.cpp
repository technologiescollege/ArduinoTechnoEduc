/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "ServoPlus.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	ServoPlus::ServoPlus(){
		//----On commence par construir dynamiquement le servo de base
			servo = new Servo();
		//---- On initialise certains attributs
			m_speed = 0;								// 0 à 1000 (1023)
			
}


//========== SETUP

/*EDU US*/	uint8_t		ServoPlus::brancher(int pin, int buteeBasse, int buteeHaute){return branch(pin);}
/*EDU US*/	uint8_t		ServoPlus::branch(int pin, int buteeBasse, int buteeHaute){return attach(pin);}
/*EDU US*/	uint8_t		ServoPlus::attach(int pin, int buteeBasse, int buteeHaute){
	
	//---- On attache le véritable servo
		return 	servo->attach(pin);
	
	//---- On enregistre les butees
		m_buteeHaute = buteeHaute;
		m_buteeBasse = buteeBasse;
}



//========== METHODES OPERATIVES
/*EDU FR*/	void		ServoPlus::ecrire(int value, int speed){ecrireAngle(value,speed);}
/*EDU FR*/	void		ServoPlus::ecrireAngle(int value, int speed){write(value,speed);}
/*EDU US*/	void		ServoPlus::write(int value, int speed){
	
	//---- On limite les valeurs aux butées
		if(value<m_buteeBasse){value==m_buteeBasse;}
		if(value>m_buteeHaute){value==m_buteeHaute;}
	//---- Si utilisation normale (sans réglage de vitesse)
	if(speed==1000){	
		//--- On écrit directement sur le véritable servo
			servo->write(value);
	}
	
	else {
		//--- On convertit angle en largeur d'impultion
			int valueMicro = map(value,0,180,600,2400);
		
		//--- On enregistre la nouvelle vitesse
			m_speed=speed;
		
		//--- On utilise writeMicroseconds (plus précise)
			writeMicroseconds(valueMicro,speed);
	}
		
}


/*EDU US*/	void		ServoPlus::writeMicroseconds(int value, int speed){

	//---- On enregistre la largeur d'impiltion souhaitée
		m_finalMicroPos = value;
		DEBUG2("final", m_finalMicroPos);
	
	//---- On lit la position réelle du servo
		m_curentMicroPos = servo->readMicroseconds();
		DEBUG2("curent", m_curentMicroPos);
	
	//---- On détermine le décalage entre largeur actuelle et largeur souhaitée
		int16_t	delta	= (m_finalMicroPos - m_curentMicroPos);	
	
	//---- Calcul de l'incrément
		uint16_t 	inc= m_speed/18;
	
	//---- On exécute une boucle fonction du signe de delta
		if(delta>0){
			//--- Il faut incrémenter la largeur d'impultion 
			for(int16_t i=0; i<delta;i+=inc){
				//---- On écrit sur le véritable servo
					servo->writeMicroseconds(m_curentMicroPos+i);
					delay(20);
			}
		} else {
			//--- Il faut décrémenter la largeur d'impultion
			for(int16_t i=0; i>delta;i-=inc){
				//---- On écrit sur le véritable servo
					servo->writeMicroseconds(m_curentMicroPos+i);
					delay(20);
			}
		}

		
}
		




//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





