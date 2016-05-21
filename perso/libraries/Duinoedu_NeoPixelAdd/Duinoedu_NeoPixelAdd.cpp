/*
 
  Original Author: David Souder - www.duinoedu.com
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Duinoedu_NeoPixelAdd.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	Duinoedu_NeoPixelAdd::Duinoedu_NeoPixelAdd(uint16_t taille, uint8_t p, uint8_t t) 
								  :Adafruit_NeoPixel(taille,p,t)		// Constructeur parent
{
	// Construction des tableaux mémoire à la bonne taille
	
	/////////////////// ON PEUT DIVISER PAR 8 !!!!
	//////////////////////////////////////////////
	
	
		memory 		= (byte*)malloc(taille*sizeof(uint8_t));

	
	// On initialise les tableaux à 0 (=aucune LED allumée)
		for(uint8_t i=0;i<taille;i++){
			memory[i]	 =B00000000;
		}
	// On sauvegarde la taille du ruban
		m_taille = taille;
	

}

void Duinoedu_NeoPixelAdd::preparerPixel(uint16_t startLed, uint16_t r, uint16_t g, uint16_t b, uint16_t widthGroupe, uint8_t step, int16_t endR,int16_t endG,int16_t endB, int16_t dep){
	
	// Variableutilisée pour vérifierque s'il est nécessaire de mettre à jour le ruban
		bool updateNecessaryFlag=true;		
	
	// dep : déplacement 0 à 1000 (0..500 = dans 1 sens / 500..1000 = dans l'autre sens)
	//-- On limte la valeur de vitesse (issue par exemple d'un capteur)
		if(dep>1000) dep=1000; 
		if(dep<-1000)dep=-1000;


	//-- On calcule un curseur
		int cur = millis()*(dep)/2000;		// Donne un rythm au déplacement 1,2,3...

	// On calcule l'adrese de début du ruban
			uint16_t startIndex = (startLed+cur)%m_taille;			//%m_taille permet de ne jamais dépasser la taille du ruban
	
	//-- Si aucun changement => on baisse le flag
		if(startIndex==m_startIndex && r==m_r && g==m_g && b==m_b && widthGroupe==m_widthGroupe && step==m_step && endR==m_endR && endG==m_endG && endB==m_endB ){
			Serial.println("IDEM");
			updateNecessaryFlag = false;
		}
	
	//-- Si un changement détecté 
		if(updateNecessaryFlag){
			//- On met à jour le context
				m_startIndex=startIndex;
				m_r=r;
				m_g=g;
				m_b=b;
				m_widthGroupe=widthGroupe;
				m_step=step;
				m_endR=endR;
				m_endG=endG;
				m_endB=endB;
		}
		
	
	//-- On gère la préparation du nouveau segment et l'enregistrement dans le tableau memory
		//- On actualise le tableau memory (effacé par show())
			// On calcule une adresse de fin. Si l'adresse de fin est au dela la la fin du ruban, on enlève la largeur du ruban
				uint16_t endIndex   = startIndex+widthGroupe;
			
			// Cas où l'index de fin déborde >>>[1111E0000000S1111] S=stratIndex E=endIndex
			if(endIndex>m_taille){
				//- On ajuste le endIndex 
					endIndex-=m_taille;
				//- On enregistre la partie S1111 du ruban
					for(uint8_t j=startIndex; j<m_taille; j+=step){
						saveBit(1,j,memory);
					}
				//- On enregistre la partie 1111E du ruban
					for(uint8_t j=0; j<endIndex; j+=step){
						saveBit(1,j,memory);
					}
			}
			// Cas où il n'y a pas débardement >>>[S111E00000000000] S=stratIndex E=endInde
			else{
				//- On enregistre tout le segment S111E
					for(uint8_t j=startIndex; j<endIndex; j+=step){
						saveBit(1,j,memory);
					}
			}
			

					
		//- On fait la préparation effective du ruban si besoin
			if(updateNecessaryFlag){
				Serial.println("UP");
				Adafruit_NeoPixel::preparerPixel(startIndex, r ,g ,b ,widthGroupe,step ,endR ,endG ,endB );
			}
}

void Duinoedu_NeoPixelAdd::afficher(){show();}
void Duinoedu_NeoPixelAdd::show(void){
	//-- On prépare à 0 tous les pixels n'étant pas sollicités par les segments = tous les pixels à 0 dans memory
	for(uint16_t k=0; k<m_taille; k++){
		// Toutes les cases à 0 correspondent à des pixels à effacer
		if(readBit(k)==0){
			setPixel(k,0);
		}
	}
	//-- On affiche les pixels	
		Adafruit_NeoPixel::show();
	
	//-- On raz memory
		for(uint16_t i=0; i<m_taille; i++){
			saveBit(0,i);
		}
	/*	
	// DEBUG
		for(uint16_t t=0; t<m_taille; t++){
			Serial.print(readBit(t));
		}		
		Serial.println();
	*/
}


//========== SETUP

//========== METHODES OPERATIVES

//========== MODIFICATION D'OPTIONS





//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


/*EDU US*/	void Duinoedu_NeoPixelAdd::saveBit(bool value, uint16_t place, byte *tab){
// Permet de placer un bit unique dans un tableau de byte (divise / 8 la place nécessaire)

	//---- Calcul de l'index du tableau réel
		uint16_t indexTab;
		indexTab = place/8;	
	
	//---- Calcul de l'emplacement du bit dans le mot
		uint8_t indexBit;
		indexBit = place%8;
	
	//---- Sauvegarde du bit sans toucher aux autres
		//-- On récupère le mot concerné
			uint8_t word = memory[indexTab];
		//-- On force le seul bit concerné	
			if(value==1) {
				word |= B00000001<<indexBit ;
				//bitSet(word,indexTab);
				memory[indexTab]=word;		
			}else{
				word &= ~(B00000001<<indexBit);
				//bitClear(word,indexTab);
				memory[indexTab]=word;		
			}	
}

/*EDU US*/	bool Duinoedu_NeoPixelAdd::readBit(uint16_t place, byte *tab){
// Permet de placer un bit unique dans un tableau de byte (divise / 8 la place nécessaire)	
	
	//---- Calcul de l'index du tableau réel
		uint16_t indexTab;
		indexTab = place/8;	
	
	//---- Calcul de l'emplacement du bit dans le mot
		uint8_t indexBit;
		indexBit = place%8;
		
	//---- Lecture du bit
		//-- On récupère le mot concerné
			uint8_t	word = memory[indexTab];
		//-- On teste la valeur du bit en question	
			if(bitRead(word,indexBit)==1){
				return true;
			}else{
				return false;
			}
}











