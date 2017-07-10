/*
 
  Original Author: David Souder
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_LedControlAdd.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_LedControlExtend::Duinoedu_LedControlExtend(){
	
		m_sequenceIndex = 0;

		for (int i = 0; i < EDU_LEDCONTROL_MAXDISP; i++) {
			setDelay(i, EDU_LEDCONTROL_DEFAULT_DELAY);	
		}
	}

		/* Short */	
/*EDU FR*/	void Duinoedu_LedControlExtend::brancher(int din, int cs, int clk) 
			{branch(din,cs,clk, 1);}
/*EDU US*/	void Duinoedu_LedControlExtend::branch(int din, int cs, int clk)
			{branch(din,cs,clk, 1);}
		/* Extended */
/*EDU FR*/	void Duinoedu_LedControlExtend::brancher(int din, int cs, int clk, int num_disp)
			{branch(din,cs,clk,num_disp);}
/*EDU US*/	void Duinoedu_LedControlExtend::branch(int din, int cs, int clk, int num_disp) {
			if (din==0) din = EDU_LEDCONTROL_DIN;
			if (cs==0) cs = EDU_LEDCONTROL_CS;
			if (clk == 0) clk=EDU_LEDCONTROL_CLK;
			if (num_disp==0) num_disp=EDU_LEDCONTROL_DISP;
	
		// On initialise l'objet
		m_numdisp = num_disp;
		matrix = new LedControl(din,clk,cs,num_disp);
	
		// On initialise l'affichage
		for (int i = 0; i < m_numdisp; i++) {
			matrix->clearDisplay(i);
			matrix->shutdown(i, false);
			matrix->setIntensity(i,5);
		}
}

		/* ------------------ */
/*EDU FR*/	void Duinoedu_LedControlExtend::afficherImage(byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 )
			{__displayImage(0, L0,L1,L2,L3,L4,L5,L6,L7);}
/*EDU US*/	void Duinoedu_LedControlExtend::displayImage(byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 )
			{__displayImage(0, L0,L1,L2,L3,L4,L5,L6,L7);}
/*EDU FR*/	void Duinoedu_LedControlExtend::afficherImage(int idisp, byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 )
			{__displayImage(idisp, L0,L1,L2,L3,L4,L5,L6,L7);}
/*EDU US*/	void Duinoedu_LedControlExtend::displayImage(int idisp, byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 )
			{__displayImage(idisp, L0,L1,L2,L3,L4,L5,L6,L7);}

/*EDU US*/	void Duinoedu_LedControlExtend::__displayImage(int idisp, byte L0, byte L1, byte L2, byte L3, byte L4, byte L5, byte L6, byte L7 ){
			// Utilité : permet d'afficher une image décrite à la volée par l'utilisateur
			// On remplit un tableau à la volée avec les données utilisateur 
			byte temp[8];
			temp[0]=L0; temp[1]=L1; temp[2]=L2; temp[3]=L3; temp[4]=L4; temp[5]=L5; temp[6]=L6; temp[7]=L7;
		
			// On affiche 	
			__displayImageX(idisp, temp);
		}

		/* ------------------ */
/*EDU FR*/	void Duinoedu_LedControlExtend::afficherImage(const byte* icon)
			{__displayImageX(0,icon);}
/*EDU US*/	void Duinoedu_LedControlExtend::displayImage(const byte* icon)
			{__displayImageX(0,icon);}
/*EDU FR*/	void Duinoedu_LedControlExtend::afficherImage(int idisp, const byte* icon)
			{__displayImageX(idisp,icon);}
/*EDU US*/	void Duinoedu_LedControlExtend::displayImage(int idisp, const byte* icon)
			{__displayImageX(idisp,icon);}

		void Duinoedu_LedControlExtend::__displayImageX(int idisp, const byte* icon){
			// Balayage des lignes et colonnes de l'icone. icon = une seule case 
			// du tableau séquence pointée par IMAGES[x]
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					matrix->setLed(idisp, i, j, bitRead(icon[i], 7 - j));
				}
			}
		}

/*EDU FR*/	void Duinoedu_LedControlExtend::afficherSequence(const byte seq[][8], const int nbrOfIcons)
			{__displaySequence(0,seq,nbrOfIcons);}
/*EDU US*/	void Duinoedu_LedControlExtend::displaySequence(const byte seq[][8], const int nbrOfIcons)
			{__displaySequence(0,seq,nbrOfIcons);}
/*EDU FR*/	void Duinoedu_LedControlExtend::afficherSequence(int idisp, const byte seq[][8], const int nbrOfIcons)
			{__displaySequence(idisp,seq,nbrOfIcons);}
/*EDU US*/	void Duinoedu_LedControlExtend::displaySequence(int idisp, const byte seq[][8], const int nbrOfIcons)
			{__displaySequence(idisp,seq,nbrOfIcons);}

		void Duinoedu_LedControlExtend::__displaySequence(int idisp, const byte seq[][8], const int nbrOfIcons){
		// Utilité : affichage d'une séquence d'icônes 
			for(uint8_t i=0;i<nbrOfIcons;i++){
			//-- Afichage d'une icône
				__displayImageX(idisp, seq[m_sequenceIndex]);
			delay(delayDuration[idisp]);
			m_sequenceIndex++;
			}
		m_sequenceIndex=0;
		}


/*EDU US*/	void Duinoedu_LedControlExtend::incrementerSequence(const byte seq[][8], const int nbrOfIcons)
			{__stepSequence(0, seq,nbrOfIcons);}
/*EDU US*/	void Duinoedu_LedControlExtend::stepSequence(const byte seq[][8], const int nbrOfIcons)
			{__stepSequence(0, seq,nbrOfIcons);}
		void Duinoedu_LedControlExtend::__stepSequence(int idisp, const byte seq[][8], const int nbrOfIcons){
		// Utilité : affichage d'une séquence d'icônes 
		//-- Afichage d'une icône
			__displayImageX(idisp, seq[m_sequenceIndex]);
			delay(delayDuration[idisp]);
			if(m_sequenceIndex<nbrOfIcons-1){
				m_sequenceIndex++;
			}else{
				m_sequenceIndex=0;
			};
}

// PARAMETRAGE UTILISATEUR
/*EDU FR*/	void Duinoedu_LedControlExtend::reglerDelais(uint16_t pause){__setDelay(0, pause);}
/*EDU US*/	void Duinoedu_LedControlExtend::setDelay(uint16_t pause){ __setDelay(0, pause);}
/*EDU FR*/	void Duinoedu_LedControlExtend::reglerDelais(int idisp, uint16_t pause){ __setDelay(idisp, pause);}
/*EDU US*/	void Duinoedu_LedControlExtend::setDelay(int idisp, uint16_t pause){ __setDelay(idisp, pause);}

		void Duinoedu_LedControlExtend::__setDelay(int idisp, uint16_t pause){
	delayDuration[idisp] = pause;	
}


//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





