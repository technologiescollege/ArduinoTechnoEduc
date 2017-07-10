/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "MotorShieldDupont.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	MotorShieldDupont::MotorShieldDupont(){

	m_userSpeedA 	= 1000;				// Max par défaut
	m_userSpeedB	= 1000;	
	m_userDrift		= 500;				// Valeur nulle de dérive par défaut
	toutOuRien 		= false;			// Par défaut on est pas en mode Tout / Rien
	serialInfos 	= false;			// Messages sur le port Série
	
	tout_droit		= "tout_droit";
	droite			= "droite";
	gauche          = "gauche";
	
}

// ADUMOTO (ou autre marque à définir)
/*EDU FR*/	void MotorShieldDupont::brancher(uint8_t constructor){
	if(constructor==EDU_ARDUMOTO){
		m_constructor = EDU_ARDUMOTO;
		setPins(ARDUMOTO_ADIR, ARDUMOTO_APWM, ARDUMOTO_BDIR, ARDUMOTO_BPWM);
	}

}

// L298 GENERIQUE
/*EDU FR*/	void MotorShieldDupont::brancher(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm){
		m_constructor = EDU_L298;
		setPins(ADir, APwm, BDir, BPwm);
}


// HBRISGE 
/*EDU FR*/	void MotorShieldDupont::brancherHBridge(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm){
		m_constructor = EDU_HBRIDGE;
		setPins(ADir, APwm, BDir, BPwm);
}


/*EDU FR*/  void MotorShieldDupont::modeToutOuRien(){
	toutOuRien = true;
}


/*EDU FR*/	void MotorShieldDupont::reglerVitesse(int16_t userSpeedA, int16_t userSpeedB){ecrireVitesse(userSpeedA,userSpeedB);}
/*EDU FR*/	void MotorShieldDupont::ecrireVitesse(int16_t userSpeedA, int16_t userSpeedB){
	//Serial.print("SPA:");Serial.println(userSpeedA);
	//---- On sauvegarde les vitesses (sur la base 0-1000) - Le réglage effectif est sur la base 0-255
		/**/ if(userSpeedA>1000)	{userSpeedA=1000;}
		/**/ else if(userSpeedA<0)	{userSpeedA=0;}
		/**/ if(userSpeedB>1000)	{userSpeedB=1000;}
		/**/ else if(userSpeedB<0)	{userSpeedB=0;}
		
		m_userSpeedA=userSpeedA;
		m_userSpeedB=userSpeedB;
	
	//---- On recalcule les vitesses effective 
		setEffectiveSpeedAB255();
		
	//---- Les nouvelles valeurs de vitesses sont disponibles en base 255 dans :
		// m_effectiveSpeedA255
		// m_effectiveSpeedB255
	
	//---- On rappelle la dernière fonction e direction
		callBack();
	
	
		
		
		
}

/*EDU FR*/	void MotorShieldDupont::ecrireDerive(int16_t userDrift){


	//---- On sauvegarde le valeur de dérive utilisateur
		/**/ if(userDrift>1000){userDrift=1000;}
		/**/ if(userDrift<0)	{userDrift=0;}
		m_userDrift=userDrift;
	
	//---- On recalcule les vitesses effective en base 0-255
		setEffectiveSpeedAB255();
	
	//---- Les nouvelles valeurs de vitesses sont disponibles en base 0-255 dans :
		// m_effectiveSpeedA255
		// m_effectiveSpeedB255
	
	//---- On rappelle la dernière fonction e direction
		callBack();
}



//===== PILOTAGE DU ROBOT
/*EDU FR*/	void MotorShieldDupont::avancer(String direction){
	
	// TOUT DROIT
	if(direction==tout_droit ){
		lastCmd = 111;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:avancer:tout_droit");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_AVANT_A;			// Direction
				valA2 		= HIGH;							// Vitesse
				valB1 		= EDU_MSD_ARDU_AVANT_B;			// Direction
				valB2		= HIGH;							// Vitesse
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;			
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:avancer:tout_droit");
			// Mode tout ou rien
			valA1 		= HIGH;
			valA2 		= LOW;
			valB1 		= HIGH;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= 255-m_effectiveSpeedA255;
			valB2Pwm    = 255-m_effectiveSpeedB255;				
		}


	}
	
	
	// DROITE
	if(direction==droite){
		lastCmd = 110;
		//--- L298 
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:avancer:droite");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_AVANT_A;
				valA2 		= HIGH;
				valB1 		= HIGH;
				valB2		= LOW;
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;
				valB2Pwm    = 0;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:avancer:droite");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= LOW;
			valB1 		= HIGH;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= 0;
			valB2Pwm    = 255-m_effectiveSpeedB255;				
		}

	
	}
	
	
	// GAUCHE
	if(direction==gauche){
		lastCmd = 101;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:avancer:gauche");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_AVANT_A;
				valA2 		= LOW;
				valB1 		= EDU_MSD_ARDU_AVANT_B;
				valB2		= HIGH;
			// Mode PWM
				valA2Pwm	= 0;
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:avancer:gauche");
			// Mode tout ou rien
			valA1 		= HIGH;
			valA2 		= LOW;
			valB1 		= LOW;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= 255-m_effectiveSpeedA255;
			valB2Pwm    = 0;				
		}

	
	}
	//-- Pilotage effectif		
		effectiveWrite();	
}

/*EDU FR*/	void MotorShieldDupont::reculer(String direction){
	// TOUT_DROIT
	if(direction==tout_droit){
		lastCmd = 211;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:reculer:tout_droit");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_ARRIERE_A;
				valA2 		= HIGH;
				valB1 		= EDU_MSD_ARDU_ARRIERE_B;
				valB2		= HIGH;
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:reculer:tout_droit");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= HIGH;
			valB1 		= LOW;
			valB2		= HIGH;
			// Mode PWM
			valA2Pwm	= m_effectiveSpeedA255;
			valB2Pwm    = m_effectiveSpeedB255;				
		}
	
	}
	
	// DROITE
	if(direction==droite){
		lastCmd = 210;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:reculer:droite");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_ARRIERE_A;
				valA2 		= HIGH;
				valB1 		= EDU_MSD_ARDU_ARRIERE_B;
				valB2		= LOW;
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;
				valB2Pwm    = 0;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:reculer:droite");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= LOW;
			valB1 		= LOW;
			valB2		= HIGH;
			// Mode PWM
			valA2Pwm	= 0;
			valB2Pwm    = m_effectiveSpeedB255;				
		}
		

	}
	
	
	// GAUCHE
	if(direction==gauche){
		lastCmd = 201;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:reculer:gauche");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_ARRIERE_A;
				valA2 		= LOW;
				valB1 		= EDU_MSD_ARDU_ARRIERE_B;
				valB2		= HIGH;
			// Mode PWM
				valA2Pwm	= 0;
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:reculer:droite");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= HIGH;
			valB1 		= LOW;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= m_effectiveSpeedA255;
			valB2Pwm    = 0;				
		}
	
	}
	
	//-- Pilotage effectif		
		effectiveWrite();
}

/*EDU FR*/	void MotorShieldDupont::pivoter(String direction){
	// TOUT_DROIT (n'a pas de sens)
	if(direction==tout_droit){
		lastCmd = 300;
			//serialDebug("L298:pivoter:tout_droit");
			digitalWrite(m_pinDirA,  LOW);							// Moteur A - Direction
			digitalWrite(m_pinSpeedA, LOW);							// Moteur A - Vitesse
			digitalWrite(m_pinDirB,  LOW);							// Moteur B - Direction
			digitalWrite(m_pinSpeedB, LOW);							// Moteur B - Vitesse		
	}
	// DROITE
	else if(direction==droite){
		lastCmd = 310;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:pivoter:droite");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_AVANT_A;
				valA2 		= HIGH;
				valB1 		= EDU_MSD_ARDU_ARRIERE_B;
				valB2		= HIGH;
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:pivoter:droite");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= HIGH;
			valB1 		= HIGH;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= m_effectiveSpeedA255;
			valB2Pwm    = 255-m_effectiveSpeedB255;				
		}

		
	}
	
	// GAUCHE
	else if(direction==gauche){
		lastCmd = 301;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:pivoter:gauche");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_ARRIERE_A;
				valA2 		= HIGH;
				valB1 		= EDU_MSD_ARDU_AVANT_B;
				valB2		= HIGH;
			// Mode PWM
				valA2Pwm	= m_effectiveSpeedA255;
				valB2Pwm    = m_effectiveSpeedB255;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:pivoter:guauche");
			// Mode tout ou rien
			valA1 		= HIGH;
			valA2 		= LOW;
			valB1 		= LOW;
			valB2		= HIGH;
			// Mode PWM
			valA2Pwm	= 255-m_effectiveSpeedA255;
			valB2Pwm    = m_effectiveSpeedB255;				
		}		

	}
	//-- Pilotage effectif		
		effectiveWrite();
}

/*EDU FR*/	void MotorShieldDupont::stopper(){
		lastCmd = 0;
		//--- L298
		if( m_constructor==EDU_ARDUMOTO || m_constructor==EDU_L298 ){
			//serialDebug("L298:stopper");
			// Mode tout ou rien
				valA1 		= EDU_MSD_ARDU_AVANT_A;
				valA2 		= LOW;
				valB1 		= EDU_MSD_ARDU_AVANT_B;
				valB2		= LOW;
			// Mode PWM
				valA2Pwm	= 0;
				valB2Pwm    = 0;
		}
		//--- HBridge
		else if(m_constructor==EDU_HBRIDGE){
			//serialDebug("HBridge:stopper");
			// Mode tout ou rien
			valA1 		= LOW;
			valA2 		= LOW;
			valB1 		= LOW;
			valB2		= LOW;
			// Mode PWM
			valA2Pwm	= 0;
			valB2Pwm    = 0;				
		}

	//-- Pilotage effectif		
		effectiveWrite();
}

/*EDU US*/	void MotorShieldDupont::serialDebug(const char _message1[]){
	if(serialInfos){
		Serial.println   (_message1);
	}
}






//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Choix des broches (enregistrement)
/*EDU US*/	void MotorShieldDupont::setPins(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm){
	//--- On enregistre le choix des broches (effet mémoire)
		m_pinDirA=ADir;
		m_pinSpeedA=APwm;
		m_pinDirB=BDir;
		m_pinSpeedB=BPwm;
	//--- On configure les broches en sortie
		pinMode(m_pinDirA,   OUTPUT);
		pinMode(m_pinSpeedA, OUTPUT);
		pinMode(m_pinDirB,   OUTPUT);
		pinMode(m_pinSpeedB, OUTPUT);
	//--- On neutralise les moteurs
		digitalWrite(m_pinDirA,   0);	
		digitalWrite(m_pinSpeedA, 0);
		digitalWrite(m_pinDirB,   0);
		digitalWrite(m_pinSpeedB, 0);
		
}


// Calcule la vitesse effectives à partir de la dérive et des vitesses utilisateur - résultat dans m_effectiveSpeedX
/*EDU US*/	void MotorShieldDupont::setEffectiveSpeedAB255(){
		//-- On calcule les vitesses effectives brutes
			m_effectiveSpeedA255=m_userSpeedA/4;
			m_effectiveSpeedB255=m_userSpeedB/4;
			
		//Si m_userDrift>500 => on ralenti A
		//Si m_userDrift<500 => on ralenti B
	
		if(m_userDrift>500){
			//-- Les valeurs de drift ne peuvent aller que :
			// de 501 => ralentissement minim 
			// à 1000 => ralentissement total	
			
			//-- On calcule un coef qui sera appliqué à la future vitesse
			int16_t driftCoef = m_userDrift;
			
			//-- Mapping
			driftCoef = map(driftCoef,500,1000,1000/*100%*/,0/*0%*/);
				
			//-- On calcule et applique la nouvelle vitesse 
			m_effectiveSpeedA255*=driftCoef;		
			m_effectiveSpeedA255/=1000;

		}
		if(m_userDrift<500){ 
			//-- Les valeurs de drift ne peuvent aller que :
			// de 499 => ralentissement minim 
			// à 0    => ralentissement total	
			
			//-- On calcule un coef qui sera appliqué à la future vitesse
			int16_t driftCoef = m_userDrift;
			
			//-- Mapping
			driftCoef = map(driftCoef,0,500,0/*0%*/,1000/*100%*/);

			//-- On calcule et applique la nouvelle vitesse 
			m_effectiveSpeedB255*=driftCoef;		
			m_effectiveSpeedB255/=1000;
		}
		
		//-- On regarde si l'on peut rester en mode tout ou rien (vitesse = 250)
			
			if(m_effectiveSpeedA255==250 && m_effectiveSpeedB255==250 ){
				toutOuRien = true;
				serialDebug("T/R");
			}else{
				toutOuRien = false;
				serialDebug("ANA");
			}


}

// Modifie des sorties
/*EDU US*/  void MotorShieldDupont::effectiveWrite(){
/*EDU US*/  //void MotorShieldDupont::effectiveWrite(int8_t _valA1, int8_t _valA2, int8_t _valB1, int8_t _valB2, int8_t _valA2Pwm, int8_t _valB2Pwm){
		
		if(toutOuRien){
			digitalWrite(m_pinDirA,   valA1);		// Moteur A - 298:Direction    - HBridge:Point haut
			digitalWrite(m_pinSpeedA, valA2);		// Moteur A - 298:Vitesse maxi - HBridge:Point bas
			digitalWrite(m_pinDirB,   valB1);	    // Moteur B - 298:Direction	   - HBridge:Point haut
			digitalWrite(m_pinSpeedB, valB2);		// Moteur B - 299:Vitesse maxi - HBridge:Point bas
		}else{		
			digitalWrite(m_pinDirA,   valA1);		
			analogWrite (m_pinSpeedA, valA2Pwm);	
			digitalWrite(m_pinDirB,   valB1);		
			analogWrite (m_pinSpeedB, valB2Pwm);	
		}
}

// Rappelle la dernière fonction de direction
/*EDU US*/	void MotorShieldDupont::callBack(){
	//---- On met à jour les PWM (pour avoir un recalcul en cas de modification externe de la vitesse)
		//Serial.println(lastCmd);
		switch(lastCmd){
			case 111:
				avancer(tout_droit);
			break;
			case 110: 
				avancer(droite);
			break;
			case 101:
				avancer(gauche);
			break;
			case 211: 
				reculer(tout_droit);
			break;
			case 210: 
				reculer(droite);
			break;
			case 201:
				reculer(gauche);
			break;
			case 300: 
				
			break;
			case 310:
				pivoter(droite);
			break;
			case 301: 
				pivoter(gauche);
			break;
			case 0:
				stopper();
			break;
			
		}
}




	