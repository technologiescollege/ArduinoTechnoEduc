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
	m_constructor 	= EDU_ARDUMOTO;		// Ardumoto par défaut
	m_userSpeedA 	= 1000;				// Max par défaut
	m_userSpeedB	= 1000;	
	m_userDrift		= 500;				// Valeur nulle de dérive par défaut
	
}

/*EDU FR*/	void MotorShieldDupont::brancher(uint8_t constructor){
	if(constructor==EDU_ARDUMOTO){
		setPins(ARDUMOTO_ADIR, ARDUMOTO_APWM, ARDUMOTO_BDIR, ARDUMOTO_BPWM);
		initPins(EDU_ARDUMOTO);
	}

}
/*EDU FR*/	void MotorShieldDupont::brancher(uint8_t ADir, uint8_t APwm, uint8_t BDir, uint8_t BPwm){
		setPins(ADir, APwm, BDir, BPwm);
		if(m_constructor==EDU_ARDUMOTO){
			initPins(EDU_ARDUMOTO);
		}
	
	//--- On configure les moteurs - Une configuration initiale semble obligatoire (le moteur A ne fonctionne pas si le B n'a pas été initialisé)
}

/*EDU FR*/	void MotorShieldDupont::reglerVitesse(int16_t userSpeedA, int16_t userSpeedB){ecrireVitesse(userSpeedA,userSpeedB);}
/*EDU FR*/	void MotorShieldDupont::ecrireVitesse(int16_t userSpeedA, int16_t userSpeedB){
	//---- On sauvegarde les vitesses (sur la base 0-1000) - Le réglage effectif est sur la base 0-255
		/**/ if(userSpeedA>1000){userSpeedA=1000;}
		/**/ if(userSpeedA<0)	{userSpeedA=0;}
		/**/ if(userSpeedB>1000){userSpeedB=1000;}
		/**/ if(userSpeedB<0)	{userSpeedB=0;}
		
		m_userSpeedA=userSpeedA;
		m_userSpeedB=userSpeedB;
		
	//---- On recalcule les vitesses effective 
		setEffectiveSpeedAB255();
		
	//---- Les nouvelles valeurs de vitesses sont disponibles en base 255 dans :
		// m_effectiveSpeedA255
		// m_effectiveSpeedB255
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
}

/*EDU FR*/	void MotorShieldDupont::avancer(String direction){
	// TOUT_DROIT
	if(direction=="" || direction=="devant" || direction=="tout droit" || direction=="tout_droit"){
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_AVANT_A);		    // Moteur A - Direction
		analogWrite(m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  EDU_MSD_ARDU_AVANT_B);		    // Moteur B - Direction
		analogWrite(m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - Vitesse
	}
	// DROITE
	if(direction=="droite"){
		Serial.println("droite");
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_AVANT_A);		    // Moteur A - Direction
		analogWrite(m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  HIGH);							// Moteur B - Direction
		analogWrite(m_pinSpeedB, 0);							// Moteur B - ARRET
	}
	// GAUCHE
	if(direction=="gauche"){
		Serial.println("gauche");
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_AVANT_A);		    // Moteur A - Direction
		analogWrite(m_pinSpeedA, 0);							// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  EDU_MSD_ARDU_AVANT_B);							// Moteur B - Direction
		analogWrite(m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - ARRET
	}
}
/*EDU FR*/	void MotorShieldDupont::reculer(String direction){
	// TOUT_DROIT
	if(direction=="" || direction=="devant" || direction=="tout droit" || direction=="tout_droit"){
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_ARRIERE_A);		// Moteur A - Direction
		analogWrite(m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  EDU_MSD_ARDU_ARRIERE_B);		// Moteur B - Direction
		analogWrite(m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - Vitesse
	}
	// DROITE
	if(direction=="droite"){
		Serial.println("droite");
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_ARRIERE_A);		// Moteur A - Direction
		analogWrite(m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  HIGH);							// Moteur B - Direction
		analogWrite(m_pinSpeedB, 0);							// Moteur B - ARRET
	}
	// GAUCHE
	if(direction=="gauche"){
		Serial.println("gauche");
		digitalWrite(m_pinDirA,  HIGH);		// Moteur A - Direction
		analogWrite(m_pinSpeedA, 0);							// Moteur A - Vitesse
		digitalWrite(m_pinDirB,  EDU_MSD_ARDU_ARRIERE_B);							// Moteur B - Direction
		analogWrite(m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - ARRET
	}
}

/*EDU FR*/	void MotorShieldDupont::pivoter(String direction){
	// TOUT_DROIT (n'a pas de sens)
	if(direction=="tout_droit"){
		digitalWrite(m_pinDirA,  EDU_MSD_ARDU_AVANT_A);		    // Moteur A - Direction
		analogWrite(m_pinSpeedA, 0);							// Moteur A - Arrêt
		digitalWrite(m_pinDirB,  EDU_MSD_ARDU_AVANT_B);		    // Moteur B - Direction
		analogWrite(m_pinSpeedB, 0);							// Moteur B - Arrêt
	}
	// DROITE
	else if(direction=="droite"){
		//Serial.println("droite");
		digitalWrite(m_pinDirA,   EDU_MSD_ARDU_AVANT_A);		    // Moteur A - Direction
		analogWrite (m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,   EDU_MSD_ARDU_ARRIERE_B);			// Moteur B - Direction
		analogWrite (m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - Vitesse
	}
	// GAUCHE
	else if(direction=="gauche"){
		//Serial.println("gauche");
		digitalWrite(m_pinDirA,   EDU_MSD_ARDU_ARRIERE_A);		    // Moteur A - Direction
		analogWrite (m_pinSpeedA, m_effectiveSpeedA255);			// Moteur A - Vitesse
		digitalWrite(m_pinDirB,   EDU_MSD_ARDU_AVANT_B);			// Moteur B - Direction
		analogWrite (m_pinSpeedB, m_effectiveSpeedB255);			// Moteur B - Vitesse
	}
}

/*EDU FR*/	void MotorShieldDupont::stopper(){
		digitalWrite(m_pinDirA,  LOW);		    				// Moteur A - Direction (LED éteinte)
		analogWrite(m_pinSpeedA, 0);							// Moteur A - Arrêt
		digitalWrite(m_pinDirB,  LOW);		    				// Moteur B - Direction (LED éteinte)
		analogWrite(m_pinSpeedB, 0);							// Moteur B - Arrêt
}

//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





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
}

// Config. des broches (pinMode...)
/*EDU US*/	void MotorShieldDupont::initPins(uint8_t constructor){
	if(constructor==EDU_ARDUMOTO){
		//-- Mode
		pinMode(ARDUMOTO_ADIR, OUTPUT);		// Moteur A, direction
		pinMode(ARDUMOTO_APWM, OUTPUT);		// Moteur A, vitesse
		pinMode(ARDUMOTO_BDIR, OUTPUT);		// Moteur B, direction
		pinMode(ARDUMOTO_BPWM, OUTPUT);		// Moteur B, vitesse
		//-- Etat
		digitalWrite(ARDUMOTO_ADIR,0);		// Moteur A, Sens 0
		analogWrite (ARDUMOTO_APWM,0);		// Moteur A, Vitesse nulle
		digitalWrite(ARDUMOTO_ADIR,0);		// Moteur B, Sens 0
		analogWrite (ARDUMOTO_APWM,0);		// Moteur B, Vitesse nulle
	}
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


}

