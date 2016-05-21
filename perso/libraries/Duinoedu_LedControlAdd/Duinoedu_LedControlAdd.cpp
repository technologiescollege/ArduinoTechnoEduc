/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_LedControlAdd.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_LedControlAdd::Duinoedu_LedControlAdd(){
	
		m_sequenceIndex = 0;
		setDelay(EDU_LEDCONTROL_DEFAULT_DELAY);
							
			
}

/*EDU US*/	void Duinoedu_LedControlAdd::brancher(int din, int cs, int clk){branch(din,cs,clk);}
/*EDU US*/	void Duinoedu_LedControlAdd::branch(int din, int cs, int clk){
	
	// On initialise l'objet
		matrix = new LedControl(din,clk,cs);
	
	// On initialise l'affichage
		matrix->clearDisplay(0);
		matrix->shutdown(0, false);
		matrix->setIntensity(0,5);
			
		
}


/*EDU US*/	void Duinoedu_LedControlAdd::afficherImage(byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 ){displayImage(L0,L1,L2,L3,L4,L5,L6,L7);}
/*EDU US*/	void Duinoedu_LedControlAdd::displayImage(byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 ){
// Utilité : permet d'afficher une image décrite à la volée par l'utilisateur
	
	// On remplit un tableau à la volée avec les données utilisateur 
		byte temp[8];
		temp[0]=L0; temp[1]=L1; temp[2]=L2; temp[3]=L3; temp[4]=L4; temp[5]=L5; temp[6]=L6; temp[7]=L7;
		
	// On affiche 	
		displayImage(temp);
}


/*EDU US*/	void Duinoedu_LedControlAdd::afficherImage(const byte* icon){displayImage(icon);}
/*EDU US*/	void Duinoedu_LedControlAdd::displayImage(const byte* icon){
	// Balayage des lignes et colonnes de l'icone. icon = une seule case du tableau séquence pointée par IMAGES[x]
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			matrix->setLed(0, i, j, bitRead(icon[i], 7 - j));
		}
	}	
}

/*EDU US*/	void Duinoedu_LedControlAdd::afficherSequence(const byte seq[][8], const int nbrOfIcons){displaySequence(seq,nbrOfIcons);}
/*EDU US*/	void Duinoedu_LedControlAdd::displaySequence(const byte seq[][8], const int nbrOfIcons){
// Utilité : affichage d'une séquence d'icônes 
	for(uint8_t i=0;i<nbrOfIcons;i++){
		//-- Afichage d'une icône
			displayImage(seq[m_sequenceIndex]);
		delay(delayDuration);
		m_sequenceIndex++;
	}
	m_sequenceIndex=0;
}

/*EDU US*/	void Duinoedu_LedControlAdd::incrementerSequence(const byte seq[][8], const int nbrOfIcons){displaySequence(seq,nbrOfIcons);}
/*EDU US*/	void Duinoedu_LedControlAdd::stepSequence(const byte seq[][8], const int nbrOfIcons){
// Utilité : affichage d'une séquence d'icônes 
		//-- Afichage d'une icône
			displayImage(seq[m_sequenceIndex]);
			//delay(delayDuration);
			if(m_sequenceIndex<nbrOfIcons-1){
				m_sequenceIndex++;
			}else{
				m_sequenceIndex=0;
			};
		
}

// PARAMETRAGE UTILISATEUR
/*EDU FR*/	void Duinoedu_LedControlAdd::reglerDelais(int pause){setDelay(pause);}
/*EDU US*/	void Duinoedu_LedControlAdd::setDelay(uint16_t pause){
	delayDuration = pause;	
}


//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





