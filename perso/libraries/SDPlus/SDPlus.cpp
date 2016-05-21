/*
	SDAdd
	Ajout pour gestion des cartes SD (pour Ardublock)
 	Original Author: David Souder pour www.duinoedu.com - souder.d@gmail.com
	Dernière modifications : 04/03/2015
  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "SDPlus.h"


#include <avr/pgmspace.h>



//========== << CONSTRUCTOR >>

/*EDU FR*/	SDPlus::SDPlus(){

	
	
}

/*EDU US*/	void SDPlus::brancher(uint8_t pinCs, SDClass *sd){branch(pinCs,sd);}
/*EDU US*/	void SDPlus::branch(uint8_t pinCs, SDClass *sd){
	
	//---- On active par défaut les infos séries
		serialInfos = 1;
		Serial.begin(9600);
	
	//---- Création de l'objet (à partir de SD déjà créé par la lib SD)
		m_SD = sd;
		
	
	//---- Démarrage de la SD
	if(m_SD->begin(pinCs)){
		serialInfosPrint("SD:OK");
	}else{
		serialInfosPrint("SD:PROBLEME!");
	}
	
	//---- Chargement des valeurs par défaut
		setCurentFile(SDPLUS_DEFAULT_FILE_NAME);
	    setSpacer(SDPLUS_DEFAULT_SPACER);
}


//----- SETAGE
/*EDU US*/	void	SDPlus::definirFichierCourant(String fileName){setCurentFile(fileName);}
/*EDU US*/	void	SDPlus::setCurentFile(String fileName){
	//---- Sauvegarde du nom du fichier courant
		for(uint8_t i=0;i<fileName.length();i++){
			*(m_fileName+i) =  fileName[i];
		}
}

/*EDU FR*/	void 	SDPlus::definirSeparateur(String spacer){setSpacer(spacer);}
/*EDU US*/	void	SDPlus::setSpacer(String spacer){
	m_spacer=spacer;
}


//----- ECRITURE
/*EDU FR*/	void 	SDPlus::ecrire(int valueL){write(valueL);}
/*EDU US*/	void 	SDPlus::write(int valueL){

	String tempS = String(valueL);
	write(tempS);		
}

/*EDU FR*/	void 	SDPlus::ecrire(float valueF){write(valueF);}
/*EDU US*/	void 	SDPlus::write(float valueF){
	//---- On extrait : partie entière et le reste
		long   all 	= valueF*100;							// Ex : 1.235 => 1.23 => 123
		long	  high  = all/100;								// Ex : 123 => 1
		long   low	= all%100;								// Ex : 123 => 23				
	//---- On construit la chaîne à transmettre
		String tempS = String(high) + "." + String(low);	// Ex : "1.23""
	
	//---- On transmet
		write(tempS);		
}

/*EDU FR*/	void 	SDPlus::ecrire(String valueS){write(valueS);}
/*EDU US*/	void 	SDPlus::write(String valueS){
	file = m_SD->open(m_fileName,FILE_WRITE);
	if(file){
		file.print(valueS);
		file.print(m_spacer);		
		file.close();
	}
}


/*EDU FR*/	void	SDPlus::espace(uint8_t nbr){space(nbr);}
/*EDU US*/	void 	SDPlus::space(uint8_t nbr){
	
	file = m_SD->open(m_fileName,FILE_WRITE);
	if(file){
		for(int8_t i=0;i<nbr;i++){
			file.print(" ");
		}
		file.close();
	}
}

/*EDU US*/	void 	SDPlus::tabulation(uint8_t nbr){
	//---- On écrit une tabulation par défaut que si aucune fonction d'espacement n'a été utilisée avant
	
	file = m_SD->open(m_fileName,FILE_WRITE);
	if(file){
		for(int8_t i=0;i<nbr;i++){
			file.print("\t");
		}
		file.close();
	}
}

/*EDU FR*/	void	SDPlus::nouvelleLigne(uint8_t nbr){newLine(nbr);}
/*EDU US*/	void 	SDPlus::newLine(uint8_t nbr){
	//---- On écrit une tabulation par défaut que si aucune fonction d'espacement n'a été utilisée avant
	
	file = m_SD->open(m_fileName,FILE_WRITE);
	if(file){
		for(int8_t i=0;i<nbr;i++){
			file.println("\t");
		}
		file.close();
	}
}


/*EDU US*/	void SDPlus::serialInfosPrint(String message){
	//---- On écrit que si l'option est active
		if(serialInfos==1){
			Serial.println(message);
		}
		
}







//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





