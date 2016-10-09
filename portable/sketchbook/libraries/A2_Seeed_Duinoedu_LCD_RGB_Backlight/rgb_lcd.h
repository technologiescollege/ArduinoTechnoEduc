/*
  rgb_lcd.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  add rgb backlight fucnction @ 2013-10-15
  
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
/*
  EDU FR => Addon by Souder David - www.duinoedu.com - 10/09/2016 - souder.d@gmail.com
  Dernières modifications : le 04/09/2015
  
*/



#ifndef __RGB_LCD_H__
#define __RGB_LCD_H__

#include <inttypes.h>
#include "Print.h"


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

// Device I2C Arress
#define LCD_ADDRESS     (0x7c>>1)
#define RGB_ADDRESS     (0xc4>>1)


// color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00



class rgb_lcd : public Print{

public:
				rgb_lcd();
  /*EDU FR*/ 	void brancher(void);
  /*EDU US*/ 	void branch(void);
				void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  
  /*EDU FR*/ 	void effacer(void);
  /*ARD US*/ 	void clear();
  
  /*EDU FR*/ 	//void activerAide();
				void home();
  
  /*EDU FR*/ 	void desactiverTexte();
  /*EDU FR*/ 	void desactiverEcriture();
				void noDisplay();
				
  /*EDU FR*/ 	void reactiverTexte();
  /*EDU FR*/ 	void reactiverEcriture();
  /*EDU FR*/ 	void activerTexte();
  /*EDU FR*/ 	void activerEcriture();
				void display();
				void noBlink();
				void blink();
  /*EDU FR*/    void desactiverCurseur();
				void noCursor();
				void curseur(void);
  /*EDU FR*/ 	void activerCurseur();
				void cursor();
				void scrollDisplayLeft();
				void scrollDisplayRight();
				void leftToRight();
				void rightToLeft();
				void autoscroll();
				void noAutoscroll();
				void createChar(uint8_t, uint8_t[]);
  /*EDU FR*/ 	void placerCurseur(long row, long col=0);
  /*EDU FR*/ 	void placerCurseurEn(long row, long col=0);
  /*EDU FR*/   	void setCursorOn(long row, long col=0);
  /*ARD US*/ 	void setCursor(uint8_t, uint8_t); 
				
				virtual size_t write(uint8_t);
				
				void command(uint8_t);
  
				// color control
  /*EDU FR*/ 	void retroeclairage(String couleur);
  /*EDU FR*/ 	void retroeclairage(long r=1023, long g=1023, long b=1023);
  /*ARD US*/ 	void setRGB(unsigned char r, unsigned char g, unsigned char b);               // set rgb
				void setPWM(unsigned char color, unsigned char pwm){setReg(color, pwm);}      // set pwm
				void setColor(unsigned char color);
				void setColorAll(){setRGB(0, 0, 0);}
				void setColorWhite(){setRGB(255, 255, 255);}
				
				// Ecrire 
/*EDU FR*/ 		void ecrire(int16_t nbr);								// INT
/*EDU US*/ 		void newWrite(int16_t nbr);	
/*EDU US*/		void ecrire(double nbr);								// FLOAT
/*EDU US*/		void ecrire(long nbr);									// long			

/*EDU US*/		void newWrite(double nbr);								
/*EDU FR */ 	void ecrire(String newString);							// STRING
/*EDU FR*/ 		void newWrite(String newString);	
/*EDU FR*/      void effectiveNewWrite(String newString);				// On écrit au final toujours une chaîne	
				using Print::write;
				
 /*EDU US*/		String	floatToString(float val, uint8_t widthNbr=16, uint8_t minWidth=1);

				
					uint8_t nbrOfDec;
				
private:
				void send(uint8_t, uint8_t);
				void setReg(unsigned char addr, unsigned char dta);
				//int aide;
				uint8_t _displayfunction;
				uint8_t _displaycontrol;
				uint8_t _displaymode;
				uint8_t _initialized;
				uint8_t _numlines,_currline;
  /*EDU FR*/	uint8_t etatCurseur; 

  
			//----- Mémoire
				int8_t NBR_ROWS;
				int8_t NBR_COLS;
				char m_memory[16][2];           //Mémoire de tous les caractères
				int8_t m_maxLength[16][2];      //Taille maxi du nbr attaché à la position
				
				int8_t m_curentRow;
				int8_t m_newCurentRow;
				
				int8_t m_curentCol;
				int8_t m_newCurentCol;
				
				
};

#endif
