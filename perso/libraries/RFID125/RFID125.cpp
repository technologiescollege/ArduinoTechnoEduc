
#include "RFID125.h"

#include <avr/pgmspace.h>

//////////////////// PUBLIC

// << CONSTRUCTOR >>
/*EDU US*/ RFID125::RFID125(){
		

}


/*EDU FR*/	void RFID125::brancher(int8_t pin1, int8_t pin2){
		
		branch(pin1,pin2);
		setLang(EDU_FR);
		
}//*/
/*EDU US*/	void RFID125::branch(int8_t pin1, int8_t pin2){
	//---- Récupération des broches pour le softwareSerial
		if(pin2==-1){pin2=pin1+1;}
	//---- Création du port virtuel sur les broches choisies et initialisation
		mp_softSerial = new SoftwareSerial(pin1,pin2);
		mp_softSerial->begin(9600);
	//---- Initialisation 
		count=0;													
		for(int8_t i=0;i<RFID125_WIDTH_BUF;i++){buffer[i];}			// RAZ buffer
		serialInfosOn();
		//m_serialInfos=true;											// Infos séries actives par défaut
		m_bufString ="";
		m_bufInt=0;
		m_flagBufInfos=true;										// Par défaut, pas d'infos affichables dans le buffer
	//---- Gestion mémoire
		m_selectMemType=0;											// Sélection de la RAM par défaut (pour les codes badges)
		m_flagAllCodesIsSaved=true;									// Le code est nouveau par défaut
	//---- Gestion codes
		m_lastCodeIsPresentString="";
		m_flagDataCodeDisplayed=true;								// Il n'y a pas à afficher la mémoire des code par défaut
		m_flagForcedTest=false;										// Pas de forçage d'affichage du résultat de test par défaut
		m_tempo=2000;											// Duree de vie du code lu
		m_t1=0;														// Dernière mesure de temps
		m_flagCodeIsPresentCapable=false;									// Un test ne peut pas être réalisé par défaut
	//---- Pause 
		delay(100);													
}//*/

/*EDU FR*/	String RFID125::lireCode(){return readCode();}
/*EDU US*/	String RFID125::readCode(){
	//---- Lecture du code RFID si des données sont présentes
		if(mp_softSerial->available()){
			//--- On marque qu'il y a du nouveau 
				m_flagBufInfos=true;
			//--- On vide le buffer
				m_bufInt=0;
				m_bufString="";
				count=0;
			while(mp_softSerial->available()){	
				//DEBUG2("Taille du softwareSerial : ",mp_softSerial->available()); 
					delay(5);
				//-- On lit un caractère 
					char newChar=mp_softSerial->read();
				//-- On ignore les 3 premiers et les 3 derniers caractères	 _xxHHHHHHHHxx_
					if(count>2 && count<11){
					//-- On complète le buffer
						m_bufString+=newChar;
					}
				//-- On décale le compteur				
					count++;
				//-- On déclare qu'un affichage du test est possible
					m_flagForcedTest=true;
				//-- On teste si la fin de la chaîne HEXA 
				if(m_bufString.length()==14){					// Le buf fait 1 + 12 + 1 caractères. 
					//DEBUG2("Le buf avant traitement vaut : ",m_bufString); 
					
					break;
				}
			}
		
		//INFO : à ce stade, le buffer est plein avec 8 chiffres HEXA
				
		//---- Conversion du code en valeur décimale
			for(int8_t i=0;i<m_bufString.length();i++){	
				//--- Conversion en code ASCII
					int8_t digit=(int)m_bufString[i];
				//--- Translation ASCII de 48 (pour récupérer les nbr)
					digit=digit-48;
				//--- Translation ASCII de 7 (pour les lettres)
					if(digit>9){
						digit-=7;
					}	
				//--- Contruction du nombre à partir des digits (16*16*16*digit)
				//--- Constitution du multiplicateur
					int32_t multiplicator=1;
					int8_t iterations=(8-i);
					for(int8_t k=0;k<iterations-1;k++){
						multiplicator*=16;
					}
				//--- Remplissage effectif du buffer (effet mémoire). Ce buffer doit se périmer après un certain temps
					m_bufInt+=digit*multiplicator;
					
					
				//--- On marque l'heure à laquelle le buffer à été complété (pour gérer la durée de vie)
					m_t1=millis();
					
					
				//--- Debug
					//DEBUG2("hexa : ", m_bufString[i]);
					//DEBUG2("conv : ", digit);
					//DEBUG2("Nbr : ", m_bufInt);
			}
		
		
			//---- Gestion affichage des infos séries
			//--- Si le drapeau indique qu'une nouvelle info est affichable
			if(m_flagBufInfos){
				//--- On écrit le contenu du buffer si infos série actives 
					if(m_serialInfos){
						Serial.print("CODE\t");
						Serial.println(m_bufInt);
					}
				//--- On marque que les infos nouveautés sont affichées
					m_flagBufInfos=false;
			}
		}
	//---- Si aucune données ne sont présentes, on gère la durée de vie des données du buffer
		if(!mp_softSerial->available()){
			//--- On compare l'heure actuelle à la dernière heure d'enregistrement des données du buffer
			int32_t deltaTime=millis()-m_t1;
			//--- Si la durée écoulée est supérieur à la durée de vie du code, on RAZ
			if(deltaTime>m_tempo){
				//-- Si le buf n'est pas déjà à 0
				if(m_bufInt!=0){
					m_bufString="";
					m_bufInt=0;
					//DEBUG2("RAZ du bufInt et bufString : ", m_bufInt);
					m_flagCodeIsPresentCapable=false;
				}
			}
		}
				
			
		
		
		
		
	
	//---- Return
		if(m_bufInt==0){return "";}
		if(m_bufInt!=0){return (String)m_bufInt;}
		
}



//---------- Méthodes liées à la gestion des codes RFID
// Ecriture des codes RFID

/*EDU US*/	void RFID125::ecrireCodes(String codes){
		
		ecrireCode(codes);
}
/*EDU US*/	void RFID125::writeCodes(String codes){
		writeCode(codes);
}

/*EDU US*/	void RFID125::ecrireCode(String code){

		writeCode(code);
}		
/*EDU US*/	void RFID125::writeCode(String code){
	//---- On vérifie que le code a la bonne longueur

	//--- On vérifie que le code est bien différent du précédent
		if(code==m_curentCodeString){
			//DEBUG2("Le code est le meme que precedement : ", code); delay(1000);
			
			return;
		} else {
			//--- Ce code devient alors le code courant
				m_curentCodeString=code;
				//DEBUG2("Nouveau code courant : ", code);
			//--- On marque qu'il y a un nouveau code courant
				m_flagAllCodesIsSaved=false;
			//--- On marque qu'un affichage de résulat de test peut être affiché à nouveau même si le code est le même (nouvel enregistrement)
				m_flagForcedTest=true;
		}
	//--- On teste si ce code n'est pas déjà dans la base et si tous les codes n'ont pas déjà été sauveagradés
		if(codeIsPresent(m_curentCodeString,1)==false&&m_flagAllCodesIsSaved==false){
			//-- Si le SerialInfos est actif
				if(m_serialInfos==true){
					//- Sélecteur de langue
					switch(m_lang){
						case EDU_US:Serial.println("CODE\tSAVED");break;
						case EDU_FR:Serial.println("CODE\tSAUVEGARDE");break;
					}
				}
			//-- On enregistre effectivement le code
				effectiveWriteCode(m_curentCodeString);
			//-- On marque que tous les codes sont sauvegardés
				m_flagAllCodesIsSaved=true;
		}else{
			//-- Si le SerialInfos est actif est que le code n'est pas vide
				if(m_serialInfos==true&&m_bufString!=0){
					//- Sélecteur de langue
					switch(m_lang){
						case EDU_US:Serial.println("CODE\tPRESENT");break;
						case EDU_FR:Serial.println("CODE\tDEJA PRESENT");break;
					}
				}
		
		}
	
	
	
	
	
}//*/	
/*EDU US*/	void RFID125::effectiveWriteCode(String code){
	//---- On enregistre le code RFID et le séparateur
		m_dataCodesString+=code;
		m_dataCodesString+=' ';
		//DEBUG2("EN MEMOIRE: ", m_dataCodesString); 	
	
	//---- On gère le Serial-infos
		if(m_serialInfos==true){
			Serial.print("LISTE\t");
			Serial.println(m_dataCodesString);
		}
}//*/

/*EDU US*/	void RFID125::effacerCodes(void){

		clearCodes();
}
/*EDU US*/	void RFID125::clearCodes(void){
		
		m_dataCodesString="";
}

/*EDU FR*/	bool RFID125::testerCode(String code){

		return codeTestOk(code);
}//*/
/*EDU US*/	bool RFID125::codeTestOk(String code){
		
		return codeIsPresent(code);

}//*/
/*EDU US*/	bool RFID125::codeIsPresent(String code, bool lowLevel){
		
		String subStringCode		= code;
		int posSubStringCode		= -1;
		bool returnValue			= false;		

	//---- Si le code à tester est vide, on retourne faux
		if(subStringCode==""){
			return false;
		}
		
		
	//---- On vérifie la mise en forme de la base de données en RAM (RAM=0)
		if(m_selectMemType=0){
			checkRamBase();
		}
			
	//---- On crée une chaîne avec un espace pour garantir la prise en compte du code complet
		subStringCode+=' ';
		//DEBUG("subStringCode : DEBUT");
		//DEBUG(subStringCode);
		//DEBUGLN("FIN");
		//DEBUG("Base : DEBUT");
		//DEBUG(m_dataCodesString);
		//DEBUGLN("FIN");
	//---- On  calcule la position du code (chaîne) dans la base (-1 = chaîne non trouvée) 
	
		posSubStringCode=m_dataCodesString.indexOf(subStringCode);
		//DEBUG2("posSubStringCode : ", posSubStringCode);
	//---- On teste la position du subString
		if(posSubStringCode<0){
			returnValue=false;
		}else{
			//DEBUGLN("CODE TROUVE");
			returnValue=true;
		}
		
		//Serial.println(posSubStringCode);
	
	//---- On gère le Serial info si on est pas en utilisation lowLevel 
		if(m_serialInfos==true&&lowLevel==false&&m_bufString!=""){
			//DEBUGLN("POINT TEST");
			//--- On s'assure que le code n'est pas le même que précédement
				//DEBUG2("Code: ", code);
				//DEBUG2("lastCode: ", m_lastCodeIsPresentString);
				if((code!=m_lastCodeIsPresentString)||m_flagForcedTest){
					//-- On enregistre le code en cours
						m_lastCodeIsPresentString=code;
						//DEBUG2("Dernier code enregistre: ", m_lastCodeIsPresentString);
					//-- On écrit sur le port série que le code testé est présent ou pas dans la base
						if(returnValue==true){
							Serial.println("TEST\tOK");
						}else{
							switch(m_lang){
								case EDU_US:Serial.println("TEST\tBAD");break;
								case EDU_FR:Serial.println("TEST\tINCONNU");break;
							}
						}
					//-- On marque qu'aucun affichage de test n'est nécessaire 
					m_flagForcedTest=false;
				}
		}
		
		
		return returnValue;

}//*/


//---------- Modification des options
/*EDU FR*/	void RFID125::desactiverSerialInfos(){serialInfosOff();}
/*EDU US*/	void RFID125::serialInfosOff(){
		
		m_serialInfos=false;
}

/*EDU FR*/	void RFID125::activerSerialInfos(){serialInfosOn();}
/*EDU US*/	void RFID125::serialInfosOn(){
		Serial.begin(9600);
		m_serialInfos=true;
		Serial.print("CODE\t--------\n\n");
}

/*EDU FR*/	void RFID125::ecrireTypeMemoire(String mem){setMemType(mem);}
/*EDU US*/	void RFID125::setMemType(String mem){
	//---- Sauvegarde RAM
		if(mem=="ram"){
			if(m_selectMemType!=0){
				//--- On enregistre le nouveau type de memoire
					m_selectMemType=0;
				//--- On affiche le SerialInfo
					if(m_serialInfos==true){
						Serial.println("MEM\tRAM");
					}
			}
		}
	//---- Sauvegarde ROM
		if(mem=="rom"){
			if(m_selectMemType!=1){
				//--- On enregistre le nouveau type de memoire
					m_selectMemType=1;
				//--- On affiche le SerialInfo
					if(m_serialInfos==true){
						Serial.println("MEM\tROM");
					}
			}
		}
}

/*EDU US*/	void RFID125::setLang(int8_t lang){
	
		if(lang!=m_lang){
			m_lang=lang;
	
		}
}//*/

/*EDU US*/ void RFID125::ecrireTempo(int32_t tempo){

		setTempo(tempo);
}
/*EDU US*/ void RFID125::setTempo(int32_t tempo){
		
		m_tempo=tempo;
		if(m_serialInfos==true){Serial.print("TEMPO\t");Serial.print(m_tempo);Serial.println(" ms\n");}
}

//////////////////// PRIVATE

/*EDU US*/	void RFID125::checkRamBase(){

	//Permet de vérifier que la base de donnée en RAM est bien mise en forme. Il faut :
	//		> Vérifier que la base se termine bien par un espace
	
	//RAPPEL : 
	//		> m_dataCodesString est la chaîne qui contient tous les codes en RAM séparés par un espace
	//		> RAMlist, RAMliste sont des alias utilisateur
	
	//---- On mesure la longueur de la RAMlist
		int16_t lenCode=m_dataCodesString.length();
	//---- on prélève le dernier caractère de la chaîne
		char lastCharCode=m_dataCodesString[lenCode-1];
	//---- on ajoute l'espace de fin si besoin
		if(lastCharCode!=' '){
			//--- On ajoute l'espace de fin
				m_dataCodesString+=' ';
		
		}
}//*/








////////////////////:: Reste
// supprimerBadge("EEPROM" / "FLASH"); 
// Gestion EEPROM







