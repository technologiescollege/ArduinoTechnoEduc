/*
  SeeedOLED.cpp - SSD130x OLED Driver Library
  2011 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author: Visweswara R
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Wire.h"
#include "SeeedOLED.h"

#include <avr/pgmspace.h>



// 8x8 Font ASCII 32 - 127 Implemented
// Users can modify this to support more characters(glyphs)
// BasicFont is placed in code memory.

// This font be freely used without any restriction(It is placed in public domain)
const unsigned char BasicFont[][8] PROGMEM=
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
  {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
  {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
  {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
  {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
  {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
  {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
  {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
  {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
  {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
  {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
  {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
  {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
  {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
  {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
  {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
  {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
  {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
  {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
  {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
  {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
  {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
  {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
  {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
  {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
  {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
  {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
  {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
  {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
  {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
  {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
  {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
  {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
  {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
  {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
  {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
  {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
  {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
  {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
  {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
  {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
  {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
  {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
  {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
  {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
  {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
  {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
  {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
  {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
  {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
  {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
  {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
  {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
  {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
  {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
  {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
  {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
  {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
  {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
  {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
  {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
  {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
  {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00} 
};

/*EDU FR*/ void SeeedOLED::brancher(){branch();}
/*EDU US*/ void SeeedOLED::branch(){
	Wire.begin();			// I2C 
	//---- Propre à cet afficheur
	init();
	DDRB|=0x21;   			// Seeed     
	PORTB |= 0x21;			// Seeed
	setPageMode();          // Mode page par défaut
	clearDisplay();         // Effacement
	setBrightness(255);     // Lum max par défaut
	setNormalDisplay();		// Ecriture blanche par défaut
	setTextXY(0,0);         // Curseur en 0,0
	SeeedOled.desactiverDefilement(); // L'OLED garde en mémoire le défilement
	
	//---- Initialisation des buffers
	NBR_ROWS=8;                     // L'OLED fait 8 lignes
	NBR_COLS=16;                    // L'OLED fait 16 colonnes
	delay(1000);
	
	//---- Configuration mémoire à 0
	  //-- Memory => remplissage par défaut
	  for(int8_t i=0;i<NBR_COLS;i++){
		m_memory[i][0]='=';
	  }
	  for(int8_t i=0;i<NBR_COLS;i++){
		m_memory[i][1]='.';
	  }
	  
	//--- Affichage de memory => Serial
	DEBUG_TAB2("MEMORY contient initialement : ", m_memory,0,15,0,1);
		
	//----- Position curseur par défaut
	  m_curentRow=0;
	  m_curentCol=0;
	  
	//----- Initialisation de la taille max du nombre attaché à la position (EX: 4 = les dernier mot faisait 4 cractères)
	  for(int8_t m=0;m<NBR_ROWS;m++){
		for(int8_t i=0;i<NBR_COLS;i++){
			m_maxLength[i][m]=0;
		}
	  }
}
void SeeedOLED::init(void){
    sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
    delay(5); 
    sendCommand(SeeedOLED_Display_On_Cmd); 	    //display on
    delay(5); 
    sendCommand(SeeedOLED_Normal_Display_Cmd);  //Set Normal Display (default)
}

void SeeedOLED::sendCommand(unsigned char command){
  Wire.beginTransmission(SeeedOLED_Address); // begin I2C communication
#if defined(ARDUINO) && ARDUINO >= 100
  Wire.write(SeeedOLED_Command_Mode);	     // Set OLED Command mode
  Wire.write(command);
#else
  Wire.send(SeeedOLED_Command_Mode);	     // Set OLED Command mode
  Wire.send(command);
#endif
  Wire.endTransmission();    		         // End I2C communication
}


/*EDU FR*/ void SeeedOLED::reglerLuminositeA(unsigned char Brightness){setBrightness(Brightness);}
/*ARD US*/ void SeeedOLED::setBrightness(unsigned char Brightness){
   sendCommand(SeeedOLED_Set_Brightness_Cmd);
   sendCommand(Brightness);
}

/*EDU FR*/ void SeeedOLED::activerMode1Ligne(){setHorizontalMode();}
/*ARD US*/ void SeeedOLED::setHorizontalMode()
{
    addressingMode = HORIZONTAL_MODE;
    sendCommand(0x20); 			//set addressing mode
    sendCommand(0x00); 			//set horizontal addressing mode
}

/*EDU FR*/ void SeeedOLED::activerModeMultiLignes(){setPageMode();}
/*ARD US*/ void SeeedOLED::setPageMode()
{
    addressingMode = PAGE_MODE;
    sendCommand(0x20); 			//set addressing mode
    sendCommand(0x02); 			//set page addressing mode
}

//----- Cursor 
/*EDU FR*/ void SeeedOLED::placerCurseurEn(unsigned char ligne, unsigned char colonne){
	setCursorOn(ligne, colonne);
}
/*EDU US*/ void SeeedOLED::setCursorOn(long row, long col){
    //-- On mémorise la position du curseur 
	m_curentRow=row;
	m_curentCol=col;
	setTextXY(row,col); 
}
/*ARD US*/ void SeeedOLED::setTextXY(unsigned char Row, unsigned char Column){
    sendCommand(0xB0 + Row); 					//set page address
    sendCommand(0x00 + (8*Column & 0x0F)); 		//set column lower address
    sendCommand(0x10 + ((8*Column>>4)&0x0F)); 	//set column higher address
}

//----- Clear display
/*EDU FR*/ void SeeedOLED::effacer(){
	clearDisplay();
}
/*ARD US*/ void SeeedOLED::clearDisplay(){
  unsigned char i,j;
  sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
  for(j=0;j<8;j++)
  {	
    setTextXY(j,0);    
    {
      for(i=0;i<16;i++)  //clear all columns
      {
        putChar(' ');    
      }
    }
  }
  sendCommand(SeeedOLED_Display_On_Cmd); 	//display on
  setTextXY(0,0);    
}

//----- Send I2C comand
void SeeedOLED::sendData(unsigned char Data){
     Wire.beginTransmission(SeeedOLED_Address); // begin I2C transmission
#if defined(ARDUINO) && ARDUINO >= 100
     Wire.write(SeeedOLED_Data_Mode);            // data mode
     Wire.write(Data);
#else
     Wire.send(SeeedOLED_Data_Mode);            // data mode
     Wire.send(Data);
#endif
     Wire.endTransmission();                    // stop I2C transmission
}

//----- Writing
// Int => String
/*EDU FR*/ void SeeedOLED::ecrire(int32_t nbr){newWrite(nbr);}//*/
/*EDU US*/ void SeeedOLED::newWrite(int32_t nbr){
	//--- On convertit en String
		DEBUG2("Le nbr recu est : ", nbr); 
		//- Enregistrement
		String nbrToStr;
		nbrToStr=String(nbr);
		DEBUG2("Le nbr converti en chaine est : ", nbrToStr);
		effectiveNewWrite(nbrToStr);
}//*/

// String => String
/*EDU FR*/ void SeeedOLED::ecrire(String chaine){
		newWrite(chaine);
}//*/
/*EDU US*/ 	void SeeedOLED::newWrite(String newString){
	//--- On convertit en String
		DEBUG2("Pas de conversion, c est deja une chaine ", newString); 
		//- Enregistrement
		DEBUG2("La chaine reste : ", newString);
		effectiveNewWrite(newString);
}//*/

// Float => String
/*EDU FR / void SeeedOLED::ecrire(float nbr){
		newWrite(nbr);
}//*/
/*EDU US / void SeeedOLED::newWrite(float nbr){
	//--- On convertit en String
		DEBUG2("Le nbr recu est : ", nbr); 
		//- Enregistrement
		String nbrToStr;
		nbrToStr=String(nbr);
		DEBUG2("Le nbr converti en chaine est : ", nbrToStr);
		effectiveNewWrite(nbrToStr);
}//*/


//----- Writing UNSIGNED INT (no effective)
/*EDU FR/ unsigned char SeeedOLED::ecrire(unsigned int UInt){
	static unsigned char valRetourUInt;
	if(curentUInt[m_curentRow]!=UInt){
	curentUInt[m_curentRow]=UInt;
	long longNumber = UInt;
	valRetourUInt = putNumber(longNumber);
	}
	return valRetourUInt;
}//*/

//----- Writing CHAR 
/*EDU FR/void SeeedOLED::ecrire(char Char){
	
	if(curentChar[m_curentRow]!=Char){
	curentChar[m_curentRow]=Char;
	unsigned char uChar = Char;
	putChar(uChar);
	}

}//*/

//----- Writing UNSIGNED CHAR
/*EDU FR/ void SeeedOLED::ecrire(unsigned char UChar){
	if(curentUChar[m_curentRow]!=UChar){
	curentUChar[m_curentRow]=UChar;
	putChar(UChar);
	}
}//*/


/*EDU US*/ 	void SeeedOLED::effectiveNewWrite(String newString){
	
	//--- On teste pour voir si la chaîne est nulle
		if(newString==""){return;}

	//--- On crée des variables locales 
		int8_t userCol;			// Utilisée pour sauvegarder la position du curseur utilisateur
	
	//--- On sauvegarde la position du curseur de l'utilisateur
		userCol = m_curentCol;

	//--- On récupère la ligne courante
		DEBUG2("La ligne courante est : ", m_curentRow); 
				
	//--- On récupère la colonne courante
		DEBUG2("La colonne courante est : ", m_curentCol);
	
	//--- On récupère la dernière taille max à partir de temp
		int8_t maxLength=m_maxLength[m_curentCol][m_curentRow];
		DEBUG2("La derniere taille max enregistree est : ", maxLength);
	
	//--- Détermination de la taille du nbr reçu
		int8_t  newMaxLength=newString.length();
		DEBUG2("La nouvelle taille max est : ", newMaxLength);
			
	//--- On compare et enregistre la taille max
		int8_t curentLength;
		if(newMaxLength>maxLength){
		// Il faut sauvegarder la nouvelle valeurs
			m_maxLength[m_curentCol][m_curentRow]=newMaxLength;
		// Sauvegarde dans une variable pratique 
			curentLength=newMaxLength;
			DEBUG2("On enregistre la nouvelle taille max avec : ", m_maxLength[m_curentCol][m_curentRow]);
		} 
		else {
		// Sauvegarde dans une variable pratique 
			curentLength=maxLength;
			DEBUG2("On garde l ancienne valeur de taille max : ", m_maxLength[m_curentCol][m_curentRow]);
		}
			
	//--- TEMP: On construit une chaine à partir de la ligne courante et de la position du curseur
		//-- On construit 1 chaîne vide
		char tempRow[16];
		//-- On remplit la chaine avec des valeurs par défaut
		for(int8_t k=0;k<NBR_COLS;k++){
			tempRow[k]='-';	
		}
			
	//--- TEMP: On ajoute le nombre reçu à la ligne courante et à l'endroit du curseur 
		//-- On complète la chaîne newString pour qu'elle ait la longueur max constatée
		//-- La taille de la chaîne doit être de curentLength
		//-- On compare la longeur réelle et la longeur souhaitée
		int8_t diffLength = maxLength-newMaxLength;
		DEBUG2("La difference de longeur (pour cette position) est de :", diffLength);
		
		//-- On teste si le nombre à diminué de longueur
		if(diffLength>0){
			//- Il faut alors complèter la chaîne car elle est trop courte
			for(int8_t i=0;i<=diffLength;i++){
				//- On ajoute un x à la chaîne
				newString+=' ';
			}
			DEBUG2("Des 0 ont été ajoutés. La chaîne vaut maitenant : ", newString);
		}
			
	//--- On enregistre la nouvelle chaîne temp. On commence la boucle à l'endroit du curseur
		for(int8_t  i=0;i<curentLength;i++){
			//-Ecriture du nouveau mot dans la ligne courente du tableau temp
			tempRow[m_curentCol+i]=newString[i];
		}
				
		//-- Affichage de la ligne TEMP => Serial
		DEBUG_TAB("La chaine temp vaut maintenant : ",tempRow,0,16);
		//for(int8_t i=0;i<16;i++){Serial.print(tempRow[i]);} Serial.println(" "); 
		
		//--- Affichage de memory => Serial
		DEBUG_TAB2("MEMORY (avant modifications) contient : ", m_memory,0,15,0,1);
		//for(int8_t i=0;i<16;i++){Serial.print(m_memory[i][0]);} Serial.println(" "); 
		//for(int8_t i=0;i<16;i++){Serial.print(m_memory[i][1]);} Serial.println(" ");	
		
	
	//--- On va comparer la ligne temp avec m_memory uniquement sur la plage taille max
		DEBUG("Debut comparaison entre la ligne temp et m_memory - ");
		DEBUGLN("La comparaison se fait case par case sur la ligne courante");
		
		// INFOS : Debut de la boucle=position curseur (m_curentCol)
		// INFOS : Fin de la boucle = position curseur+largeur max

		//-- On calcule le début de la boucle
		int8_t startTest = m_curentCol;
			
		//-- On calcule la fin de la boucle
		int8_t endTest = startTest+curentLength;
		
		//-- Rappel colone utilisateur
		DEBUG2("Colonne utilisateur : ", userCol);
		
		//-- On lance la comparaison sur la plage active
		for(int8_t c=startTest;c<endTest;c++){
		
			//-- Comparaison case par case
			if(tempRow[c]!=m_memory[c][m_curentRow]){
				DEBUG2("Case : ", c);
				//Serial.print("Case : "); Serial.print(c); 
				DEBUG2(" : ", tempRow[c]);
				//Serial.print(" : "); Serial.print(tempRow[c]); 
				DEBUG2(" different de : ", m_memory[c][m_curentRow]);
				//Serial.print(" different de : "); Serial.print(m_memory[c][m_curentRow]); 
				DEBUG(" => Requete I2C necessaire ! ");
				//Serial.print(" => Requete I2C necessaire ! "); 
								
				//-- On enregistre la nouvelle valeur dans m_memory
				m_memory[c][m_curentRow]=tempRow[c];
				//-- On se rappelle qu'écire déplace le curseur
				//-- On envoit une reqête I2C

				//-On place le curseur à l'endroit de la modification
				setCursorOn(m_curentRow,c);
				DEBUG2("Curseur place en : ",c);
				
				// >>> EFECTIVE WRITE !
				DEBUG2("Ecriture I2C de : ", tempRow[c]);
				
				//write(tempRow[c]); => If RGB_LCD
				putChar(tempRow[c]);	
			}
			else{
				
				DEBUG2("Case : ", c);
				DEBUG2(" : ", tempRow[c]);	
				DEBUG2(" egale a : ", m_memory[m_curentRow][c]);				
				
			}
		}
			
			//-- On replace le curseur après la fin du mot courant
				//- On calcule la nouvelle position souhaitée pour le curseur
				int8_t newPos;
				newPos = userCol + curentLength;
				setCursorOn(m_curentRow, newPos);   // setCursor change automatiquement la position courante
				
	//--- Affichage de memory => Serial
		DEBUG_TAB2("MEMORY (apres modifications) contient : ", m_memory,0,15,0,1);
				
}


// Ajout d'un caractère.
/*ARD US*/ void SeeedOLED::putChar(unsigned char C){
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
		C=' '; //Space
    }	
    unsigned char i=0;
    for(i=0;i<8;i++)
    {
       //Read bytes from code memory
       sendData(pgm_read_byte(&BasicFont[C-32][i])); //font array starts at 0, ASCII starts at 32. Hence the translation
    }
}

//----- STRING
/*EDU FR/ void SeeedOLED::ecrire(const char *String){
	if(curentString[m_curentRow]!=String){
	//Serial.println("Enregistrement");
	curentString[m_curentRow]=String;
	putString(String);
	}
}//*/
/*ARD US*/ void SeeedOLED::putString(const char *String){
    unsigned char i=0;
    while(String[i])
    {
        putChar(String[i]);     
        i++;
    }
}

//----- LONG (+int)
/*EDU FR/ unsigned char SeeedOLED::ecrire(long long_num){
	static unsigned char valRetour;
	if(curentLong[m_curentRow]!=long_num){
	//Serial.println("Enregistrement");
	curentLong[m_curentRow]=long_num;
	valRetour = putNumber(long_num);
	}
	return valRetour;
	
}//*/
/*ARD US*/ unsigned char SeeedOLED::putNumber(long long_num){
  unsigned char char_buffer[10]="";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0) 
  {
    f=1;
    putChar('-');
    long_num = -long_num;
  } 
  else if (long_num == 0) 
  {
    f=1;
    putChar('0');
    return f;
  } 

  while (long_num > 0) 
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f=f+i;
  for(; i > 0; i--)
  {
    putChar('0'+ char_buffer[i - 1]);
  }
  return f;

}

//----- FLOAT 0.????
/*EDU FR/ unsigned char SeeedOLED::ecrire(float floatNumber,unsigned char decimal){
	static unsigned char valRetourFloat;
	if(curentFloat[m_curentRow]!=floatNumber){
	curentFloat[m_curentRow]=floatNumber;
	valRetourFloat = putFloat(floatNumber,decimal);
	}
	return valRetourFloat;
}//*/
/*ARD US*/ unsigned char SeeedOLED::putFloat(float floatNumber,unsigned char decimal){
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  if(floatNumber<0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f +=1;
  }
  for (unsigned char i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
    floatNumber += rounding;
  
  temp = floatNumber;
  f += putNumber(temp);
  if(decimal>0)
  {
    putChar('.');
    f +=1;
 }
  decy = floatNumber-temp;//decimal part, 
  for(unsigned char i=0;i<decimal;i++)//4 
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f +=decimal;
  return f;
}

//----- FLOAT
/*EDU FR/ unsigned char SeeedOLED::ecrire(float floatNumber){
	static unsigned char valRetourFloat;
	if(curentFloat[m_curentRow]!=floatNumber){
	curentFloat[m_curentRow]=floatNumber;
	valRetourFloat = putFloat(floatNumber);
	}
	return valRetourFloat;
}//*/
/*ARD US*/ unsigned char SeeedOLED::putFloat(float floatNumber){
  unsigned char decimal=2;
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  if(floatNumber<0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f +=1;
  }
  for (unsigned char i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
    floatNumber += rounding;
  
  temp = floatNumber;
  f += putNumber(temp);
  if(decimal>0)
  {
    putChar('.');
    f +=1;
 }
  decy = floatNumber-temp;//decimal part, 
  for(unsigned char i=0;i<decimal;i++)//4 
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f +=decimal;
  return f;
}

void SeeedOLED::drawBitmap(unsigned char *bitmaparray,int bytes)
{
  char localAddressMode = addressingMode;
  if(addressingMode != HORIZONTAL_MODE)
  {
      //Bitmap is drawn in horizontal mode 	
      setHorizontalMode();
  }

  for(int i=0;i<bytes;i++)
  {
      sendData(pgm_read_byte(&bitmaparray[i]));
  }

  if(localAddressMode == PAGE_MODE)
  {
     //If pageMode was used earlier, restore it.
     setPageMode(); 
  }
  
}

/*EDU FR*/ void SeeedOLED::reglerDefilement(bool direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed){
	setHorizontalScrollProperties(direction,startPage,endPage,scrollSpeed);
}
void SeeedOLED::setHorizontalScrollProperties(bool direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed)
{
/*
Use the following defines for 'direction' :

 Scroll_Left			
 Scroll_Right			

Use the following defines for 'scrollSpeed' :

 Scroll_2Frames		
 Scroll_3Frames
 Scroll_4Frames
 Scroll_5Frames	
 Scroll_25Frames
 Scroll_64Frames
 Scroll_128Frames
 Scroll_256Frames

*/

   if(Scroll_Right == direction)
   {
        //Scroll Right
        sendCommand(0x26);
   }
   else
   {
        //Scroll Left  
        sendCommand(0x27);

   }
    sendCommand(0x00);
    sendCommand(startPage);
    sendCommand(scrollSpeed);
    sendCommand(endPage);
    sendCommand(0x00);
    sendCommand(0xFF);
}

/*EDU FR*/ void SeeedOLED::activerDefilement(){activateScroll();}
/*ARD US*/ void SeeedOLED::activateScroll(){
    sendCommand(SeeedOLED_Activate_Scroll_Cmd);
}

/*EDU FR*/ void SeeedOLED::desactiverDefilement(){deactivateScroll();}
/*ARD US*/ void SeeedOLED::deactivateScroll(){
    sendCommand(SeeedOLED_Dectivate_Scroll_Cmd);
}

/*EDU FR*/ void SeeedOLED::retablirCouleurs(){setNormalDisplay();}
/*ARD US*/ void SeeedOLED::setNormalDisplay(){
    sendCommand(SeeedOLED_Normal_Display_Cmd);
}


/*EDU FR*/ void SeeedOLED::inverserCouleurs(){setInverseDisplay();}
/*ARD US*/ void SeeedOLED::setInverseDisplay()
{
    sendCommand(SeeedOLED_Inverse_Display_Cmd);
}


SeeedOLED SeeedOled;  // Preinstantiate Objects

