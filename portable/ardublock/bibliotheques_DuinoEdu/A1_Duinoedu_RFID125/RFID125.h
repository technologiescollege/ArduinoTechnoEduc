/*
	Librairie créée par David Souder - 25/05/2017
	Librairie inspirée par le programme Seeed Grove disponible ici :
	http://www.seeedstudio.com/wiki/Grove_-_125KHz_RFID_Reader
	Cette librairie est librement utilisable
	www.duinoedu.com

*/

// Voir infos en bas de page


#ifndef RFID125_H
#define RFID125_H

#include <SoftwareSerial.h>


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


//----- Début classe
class RFID125 {

public:

		/*EDU US*/	RFID125();

	//---- Initialisation
		/*EDU FR*/	void brancher(int8_t pin1, int8_t pin2=-1);
		/*EDU US*/	void branch(int8_t pin1, int8_t pin2=-1);

		
	//---- Commandes de gestion des codes
		/*EDU FR*/	String lireCode();
		/*EDU US*/	String readCode();
	
		/*EDU FR*/	void ecrireCodes(String codes);
		/*EDU US*/	void writeCodes(String codes);
		
		/*EDU FR*/	void ecrireCode(String code);
		/*EDU US*/	void writeCode(String code);
		
		
		/*EDU FR*/	void effacerCodes(void);
		/*EDU US*/	void clearCodes(void);
		
		
		/*EDU US*/	void clearCode(String code);
		
		/*EDU US*/	bool testerCode(String code);				//[01]
		/*EDU US*/	bool codeTestOk(String code);				//[01]
		/*EDU US*/	bool codeIsPresent(String code, bool lowLevel=false);
	
		
		
	//---- Modification options
		/*EDU FR*/	void ecrireTypeMemoire(String mem);
		/*EDU US*/	void setMemType(String mem);		
		
		/*EDU FR*/	void activerSerialInfos();
		/*EDU US*/	void serialInfosOn();	

		/*EDU FR*/	void desactiverSerialInfos();
		/*EDU US*/	void serialInfosOff();	
		
		/*EDU US*/	void setLang(int8_t lang);


		/*EDU US*/ void setTempo(int32_t tempo);	
		
		
	//---- Variables publiques
		String codesStringRam;								// Liste des codes sauvegardés (BASE DE DONNEES)
			/*EDU US*/	#define RAMlist  codesStringRam		// Alias US
			/*EDU FR*/ 	#define RAMliste codesStringRam		// Alias FR
			
		
	//---- Méthodes
		/*EDU US*/	void checkRamBase();
	

	//---- Objets internes
		SoftwareSerial *mp_softSerial;
	
	//---- Propriétés
		#define RFID125_WIDTH_BUF 64
		int8_t m_lang;											// Langue utilisée
			#define EDU_US 0
			#define EDU_EN 0
			#define EDU_FR 1
		unsigned char buffer[RFID125_WIDTH_BUF];
		String 			m_bufString;
		int32_t 		m_bufInt;
		bool 			m_bufIsNew;									// Du nouveau dans le buffer ?
		int8_t 			count;
		bool 			m_serialInfos;								// Active ou pas les infos séries
		bool 			m_flagBufInfos;								// Vrai si de nouvelles infos peuvent être affichées
		int32_t 		temp;									// Durée de vie du code...
		int32_t 		m_t1;
		
	//---- Gestion de la mémoire 
		#define RFID125_WIDTH_CODE 8
		int8_t 		m_selectMemType;								// RAM=0 / ROM=1
		//String 	m_stringDataBase;	
		bool 		m_flagAllCodesIsSaved;
		String 		m_curentCodeString;		
		
		String 		m_lastCodeIsPresentString;
		bool 		m_flagDataCodeDisplayed;						// Vrai si toute la mémoire à été affichée
		int8_t 		m_flagMemTypeInfos;								// 0=RAM infos / 1=EEPROM infos
		bool 		m_flagForcedTest;								// Vrai si le résultat d'un test doit être affiché
		bool 		m_flagCodeIsPresentCapable;						// Vrai si un test peut être fait

	protected:
		
		};


#endif

/* Lien entre chaîne Hexa lue et retournée et valeur décimale 

	Chaîne HEXA :	_XXHHHHHHHHXX_	_ espace	X non utilisé	H valeur Hexa
	
	Le code marqué sur le badge est la conversion de la valeur hexa : HHHHHHHH en binaire
		
			HEXA+CLE			HEXA			DEC				DEC complété
	Modèle	_XXHHHHHHHHXX_	=>	HHHHHHHH	=>	DDDDDDDD	=>	00DDDDDDDD
	Ex	 	 85009CC5538F 	=>	009CC553	=>	10274131	=>	0010274131
			Moniteur série										Inscrit sur la carte

*/

/* Comment enregistrer des codes à la main

	  monRFID.RAMliste="10267514 07388281 ";
	  
*/





