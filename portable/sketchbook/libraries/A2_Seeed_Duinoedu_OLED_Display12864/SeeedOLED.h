/*
  SeeedOLED.h - SSD130x OLED Driver Library
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

#ifndef SeeedOLED_data_H
#define SeeedOLED_data_H

// SeeedOLED Instruction set addresses

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif



#define SeeedOLED_Max_X 		127	//128 Pixels
#define SeeedOLED_Max_Y 		63	//64  Pixels

#define PAGE_MODE				01
#define HORIZONTAL_MODE			02

#define SeeedOLED_Address				0x3c
#define SeeedOLED_Command_Mode			0x80
#define SeeedOLED_Data_Mode				0x40
#define SeeedOLED_Display_Off_Cmd		0xAE
#define SeeedOLED_Display_On_Cmd		0xAF
#define SeeedOLED_Normal_Display_Cmd	0xA6
#define SeeedOLED_Inverse_Display_Cmd	0xA7
#define SeeedOLED_Activate_Scroll_Cmd	0x2F
#define SeeedOLED_Dectivate_Scroll_Cmd	0x2E
#define SeeedOLED_Set_Brightness_Cmd	0x81

#define Scroll_Left				0x00
#define Scroll_Right			0x01

#define Scroll_2Frames			0x7
#define Scroll_3Frames			0x4
#define Scroll_4Frames			0x5
#define Scroll_5Frames			0x0
#define Scroll_25Frames			0x6
#define Scroll_64Frames			0x1
#define Scroll_128Frames		0x2
#define Scroll_256Frames		0x3


/*EDU US*/ // DEBUG !
//#define DEBUG   		// Uncoment if debug !
#ifdef DEBUG
	#define DEBUG(val) Serial.print(val)
	#define DEBUGLN(val) Serial.println(val)
	
	#define DEBUG2(val1,val2) do								\
		{														\
			Serial.print(val1);									\
			Serial.print(" ");									\
			Serial.println(val2);								\
		} while(0)
	
	#define DEBUG_TAB(nom,tab,val1,val2) do     				\
		{														\
			Serial.print("\n");                 				\
			Serial.println(nom);                				\
			Serial.print("Col:"); 		   						\
			Serial.print("["); 		    						\
			Serial.print("0"); 									\
			Serial.print("] : "); 		    					\
			for(int16_t i=val1;i<val2;i++){ 					\
				Serial.print(tab[i]); 							\
				Serial.print(" "); 		    					\
			}                                   				\
			Serial.print("\n");             					\
		}while(0)
		
		#define DEBUG_TAB2(nom,tab,val11,val12,val21,val22) do  \
		{														\
			Serial.println("");                 				\
			Serial.println(nom);                				\
			for(int16_t j=val21;j<=val22;j++){ 					\
				Serial.print("Col:"); 		   					\
				Serial.print("["); 		    					\
				Serial.print(j); 								\
				Serial.print("] : "); 		    				\
				for(int16_t i=val11;i<=val12;i++){ 				\
					Serial.print(tab[i][j]); 					\
					Serial.print(" "); 		    				\
				}												\
				Serial.print("\n");             				\
			}   												\
			Serial.print("\n");      							\
		}while(0)	
	
	
#else 
	#define DEBUG(val)
	#define DEBUGLN(val)
	#define DEBUG2(val1,val2)
	#define DEBUG_TAB(nom,tab,val1,val2)
	#define DEBUG_TAB2(nom,tab,val11,val12,val21,val22)
#endif


class SeeedOLED {

public:

//----- Initialisation + Gestion DDRB and PORTB + Wire.begin
/*EDU FR*/ void brancher();
/*EDU US*/ void branch();

char addressingMode;

void init(void);

/*EDU FR*/ void retablirCouleurs();
/*ARD US*/ void setNormalDisplay();

/*EDU FR*/ void inverserCouleurs();
/*ARD US*/ void setInverseDisplay();

void sendCommand(unsigned char command);
void sendData(unsigned char Data);

/*EDU FR*/ void activerModeMultiLignes();
/*ARD US*/ void setPageMode();

/*EDU FR*/ void activerMode1Ligne();
/*ARD US*/ void setHorizontalMode();

//----- Cursor position
/*EDU FR*/ void placerCurseurEn(unsigned char ligne, unsigned char colonne=0);
/*EDU US*/ void setCursorOn(long row, long col=0); // (equal setTextXY)
/*ARD US*/ void setTextXY(unsigned char Row, unsigned char Column);

/*EDU FR*/ void effacer();
/*ARD US*/ void clearDisplay();

/*EDU FR*/ void reglerLuminositeA(unsigned char Brightness);
/*ARD US*/ void setBrightness(unsigned char Brightness);


//////////////////////////////////////////////////

//----- Writing

/*EDU FR*/ void ecrire(int32_t nbr);                   // int
/*EDU US*/ void newWrite(int32_t nbr);                 // int => String

/*EDU FR*/ void ecrire(String chaine);                 // String
/*EDU US*/ void newWrite(String newString);            // String => String

/*EDU FR/ void ecrire(float nbr);                      // Float
/*EDU US/ void newWrite(float nbr);                    // Float => String


/*EDU US*/ void effectiveNewWrite(String newString);   // String => Effective displaying


/*EDU FR/ unsigned char ecrire(unsigned int UInt);
/*EDU FR/ void ecrire(char Char);
/*EDU FR/ void ecrire(unsigned char UChar);

//---- Méthode utilisée pour écrire
/*ARD US*/ void putChar(unsigned char C);

/*EDU FR/ void ecrire(const char *String);     
/*ARD US*/ void putString(const char *String);

/*EDU FR/ unsigned char ecrire(long long_num);
/*ARD US*/ unsigned char putNumber(long long_num);

/*EDU FR/ unsigned char ecrire(float floatNumber,unsigned char decimal);
/*ARD US*/ unsigned char putFloat(float floatNumber,unsigned char decimal);

/*EDU FR/ unsigned char ecrire(float floatNumber);
/*ARD US*/ unsigned char putFloat(float floatNumber);




void drawBitmap(unsigned char *bitmaparray,int bytes);

/*EDU FR*/ void reglerDefilement(bool direction=0,unsigned char startPage=0, unsigned char endPage=7, unsigned char scrollSpeed=15);
/*ARD US*/ void setHorizontalScrollProperties(bool direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed);

/*EDU FR*/ void activerDefilement();
/*ARD US*/ void activateScroll();

/*EDU FR*/ void desactiverDefilement();
/*ARD US*/ void deactivateScroll();

// Mémoire affichage pour optimisation des transmissions
String curentString[8];
char          curentChar[8];
int           curentInt[8];
long          curentLong[8];
float         curentFloat[8];
unsigned int  curentUInt[8];
unsigned char curentUChar[8];

//AE unsigned char curentLigne;
//AE unsigned char curentRow;


protected:
			//----- Mémoire / Buffere...
				int8_t NBR_ROWS;
				int8_t NBR_COLS;
				char m_memory[16][8];           // Mémoire de tous les caractères
				int8_t m_maxLength[16][8];      // Taille maxi du nbr attaché à la position
				int8_t m_curentRow;             // Ligne courante
				int8_t m_curentCol;             // Colonne courante
};

// Création de l'objet SeeedOled
extern SeeedOLED SeeedOled;  // SeeedOLED object 


#endif


