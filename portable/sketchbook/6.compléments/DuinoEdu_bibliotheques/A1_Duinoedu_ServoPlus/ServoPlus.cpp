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
		
		//---- Mode assynchrone désactivé par défaut
			modeAsync = 0;
			
			lastTime=0;
			etaitDesactive=1;
			


			
}


//========== SETUP

/*EDU US*/	uint8_t		ServoPlus::brancher(int pin, int buteeBasse, int buteeHaute){return branch(pin);}
/*EDU US*/	uint8_t		ServoPlus::branch  (int pin, int buteeBasse, int buteeHaute){return attach(pin);}
/*EDU US*/	uint8_t		ServoPlus::attach  (int pin, int buteeBasse, int buteeHaute){
	

	
	//---- On enregistre les butees
		m_buteeHaute = buteeHaute;
		m_buteeBasse = buteeBasse;
	
	//---- On enregistrer la broche
		servoPin = pin;
		servoActivated = 1;
		
	
	//---- On attache le véritable servo
	return 	servo->attach(servoPin);
}


//========== METHODES OPERATIVES

//--------  Gestion des float
/*EDU FR*/	void		ServoPlus::ecrire(float value, int speed){
	int intAngle = (int)value;
	ecrireAngle(intAngle,speed);	
}
/*EDU FR*/  void		ServoPlus::ecrireAngle(float value, int speed){
	int intAngle = (int)value;
	ecrireAngle(intAngle,speed);
}

//--------  Gestion des long
/*EDU FR*/	void		ServoPlus::ecrire(long value, int speed){
	int intAngle = (int)value;
	ecrireAngle(intAngle,speed);	
}
/*EDU FR*/  void		ServoPlus::ecrireAngle(long value, int speed){
	int intAngle = (int)value;
	ecrireAngle(intAngle,speed);
}


//--------  Gestion des int
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
			if(modeAsync){
				writeMicrosecondsAssync(valueMicro,speed);
			}
			else
			{
				writeMicroseconds(valueMicro,speed);
			}
			
			
	}
		
}


/*EDU US*/	void		ServoPlus::writeMicroseconds(int value, int speed){

	//---- On enregistre la largeur d'impiltion souhaitée
		m_finalMicroPos = value;
	
	//---- On lit la position réelle du servo
		m_curentMicroPos = servo->readMicroseconds();
	
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

/*EDU US*/	void		ServoPlus::writeMicrosecondsAssync(int value, int speed){

	// Si on était désactivé avant, il faut remettre l'horlogue à l'heure
		if(etaitDesactive){
			lastTime = millis();
			etaitDesactive=0;
			Serial.println(etaitDesactive);
		}
	
	//---- On enregistre la largeur d'impultion souhaitée
		m_finalMicroPos = value;
	
	//---- On lit la position réelle du servo
		m_curentMicroPos = servo->readMicroseconds();
	
	//---- On détermine le décalage entre largeur actuelle et largeur souhaitée
		int16_t	deltaImpuls	= (m_finalMicroPos - m_curentMicroPos);	
	
	//---- On calcule l'incrément pour 10ms
		uint16_t 	inc10= m_speed/40;
	
	//---- On gère le mode désactivé
		if (!servoActivated){
			//--- On met à jour la date pour éviter les réajustements brutaux de position
				lastTime = millis();
		}
	
	//---- On regarde s'il n'est pas trop top pour faire qq chose 
		uint32_t now = millis();
		if( now < (lastTime + 10) )
		{
			return;
		}
	
	//---- On calcule le nbr de tranches de 10ms écoulées
		uint16_t 	deltaT 		= now - lastTime;
		uint16_t 	nbrSamples 	= deltaT/10;
		
	//---- On calcule l'incrément maxi pour le temps écoulé.
		uint16_t incMax = inc10*nbrSamples;
		
	
	

	//---- On ajuste la position
		if(deltaImpuls>0)
		{
			//--- Il faut incrémenter la largeur d'impultion 
				//-- On écrit sur le véritable servo
					if(deltaImpuls > incMax )
					{
						servo->writeMicroseconds(m_curentMicroPos+incMax);
					}
					else
					{
						servo->writeMicroseconds(m_curentMicroPos+deltaImpuls);
					}
		}
		else 
		{
			//--- Il faut décrémenter la largeur d'impultion
				//---- On écrit sur le véritable servo
					if(deltaImpuls > incMax )
					{
						
						servo->writeMicroseconds(m_curentMicroPos-incMax);
					}
					else
					{
						
						servo->writeMicroseconds(m_curentMicroPos-deltaImpuls);
					}

		}
	
	//--- On enregsitre la date
		lastTime = now;
				

		
}


//--------	Gestion de l'activation/Désactivation		
/*EDU FR*/	void 		ServoPlus::desactiver(){desactivate();}
/*EDU US*/	void 		ServoPlus::desactivate(){
	servoActivated = false;
	etaitDesactive = 1;
}

/*EDU US*/	void 		ServoPlus::reactiver(){reactivate();}
/*EDU US*/	void 		ServoPlus::reactivate(){

		servoActivated = true;	
}



//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/*#include <Servo.h>
#include <ServoPlus.h>

ServoPlus monServoPlus2;

void setup(){
  Serial.begin(9600);
  monServoPlus2.brancher(2,15,165);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  monServoPlus2.modeAsync = true;
}

void loop(){
  if(digitalRead(3)== HIGH)
  {
    monServoPlus2.ecrireAngle(15,100);
  }
  else
  {
    monServoPlus2.ecrireAngle(165,100);
  } 
  
  if(digitalRead(4) == HIGH)
  {
    monServoPlus2.desactiver();
  }else{
    monServoPlus2.reactiver();
  }
  Serial.println(monServoPlus2.servoActivated);
}

*/
