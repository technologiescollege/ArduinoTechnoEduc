
/*
  rgb_lcd.cpp
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

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

#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Wire.h>

#include "rgb_lcd.h"



//========== FONCTIONS
void i2c_send_byte(unsigned char dta){
    Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
    Wire.write(dta);                            // sends five bytes
    Wire.endTransmission();                     // stop transmitting
}

void i2c_send_byteS(unsigned char *dta, unsigned char len){
    Wire.beginTransmission(LCD_ADDRESS);        // transmit to device #4
    for(int i=0; i<len; i++)
    {
        Wire.write(dta[i]);
    }
    Wire.endTransmission();                     // stop transmitting
}



//========== METHODES
//---------- Constructeur
rgb_lcd::rgb_lcd(){
	nbrOfDec =2;
}

//---------- Branchement de l'afficheur
/*EDU FR*/ void rgb_lcd::brancher(void){ branch();} 
/*EDU US*/ void rgb_lcd::branch(void){
	NBR_ROWS=2;
	NBR_COLS=16;

	begin(NBR_COLS, NBR_ROWS);
	setRGB(255, 255, 255);
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
	//----- Initialisation de la taille max du nombre attaché à la position
		// 0 = pas de nbr attaché à la position
		// 4 = le nombre attaché à la position à au moins 4 chiffres
	  for(int8_t m=0;m<NBR_ROWS;m++){
		for(int8_t i=0;i<NBR_COLS;i++){
			m_maxLength[i][m]=0;
		}
	  }
}

/*ARD US*/ void rgb_lcd::begin(uint8_t cols, uint8_t lines, uint8_t dotsize){

    Wire.begin();
    
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    delayMicroseconds(50000);


    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);


    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);
    
    
    // backlight init
    setReg(0, 0);
    setReg(1, 0);
    setReg(0x08, 0xAA);     // all led control by pwm
    
    setColorWhite();

}

//---------- Effacer
/*EDU FR*/ void rgb_lcd::effacer(void){	clear();}
/*ARD US*/ void rgb_lcd::clear(){
    command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
    delayMicroseconds(2000);          // this command takes a long time!
	
	/*EDU */
	for(uint8_t l=0;l<NBR_ROWS;l++){
		for(uint8_t c=0;c<NBR_COLS;c++){
			m_memory[c][l]=0;
		}
	}
	m_curentRow=0;
	m_curentCol=0;
	/* END EDU*/
	
}

//---------- Placement du curseur
void rgb_lcd::home(){
    command(LCD_RETURNHOME);        // set cursor position to zero
    delayMicroseconds(2000);        // this command takes a long time!
}

/*EDU FR*/ void rgb_lcd::placerCurseur(long row, long col){
	setCursorOn(row, col);
}
/*EDU FR*/ void rgb_lcd::placerCurseurEn(long row, long col){
	setCursorOn(row, col);
}

/*EDU FR*/ void rgb_lcd::setCursorOn(long row, long col){
	// Formatage des valeurs
	uint8_t colUI;
	uint8_t rowUI;
	if(row<0){row=0;}
	if(row>1){row=1;}
	rowUI=row;
	if(col<0){col=0;}
	if(col>15){col=15;}
	colUI=col;
	//Invertion
	setCursor(colUI,rowUI);
}

/*ARD US*/ void rgb_lcd::setCursor(uint8_t col, uint8_t row){
	
	//----- Mémorisation
		if(row!=m_curentRow){m_curentRow=row;}
		if(col!=m_curentCol){m_curentCol=col;}
		//-- Actualisation I2C
		col = (row == 0 ? col|0x80 : col|0xc0);
		unsigned char dta[2] = {0x80, col};
		i2c_send_byteS(dta, 2);
		
}

//---------- DésactiverTexte/RéactiverTexte
/*EDU FR*/ void rgb_lcd::desactiverEcriture(){noDisplay();}
/*EDU FR*/ void rgb_lcd::desactiverTexte(){noDisplay();}
/*ARD US*/ void rgb_lcd::noDisplay(){
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
/*EDU FR*/ void rgb_lcd::reactiverTexte(){display();}
/*EDU FR*/ void rgb_lcd::reactiverEcriture(){display();}
/*EDU FR*/ void rgb_lcd::activerTexte(){display();}
/*EDU FR*/ void rgb_lcd::activerEcriture(){display();}
/*ARD US*/ void rgb_lcd::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

//---------- Affichage du curseur
/*EDU FR*/ 	void rgb_lcd::desactiverCurseur(){noCursor();}
/*ARD US*/ 	void rgb_lcd::noCursor(){
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*EDU FR*/ 	void rgb_lcd::activerCurseur(){
		curseur();
}
/*EDU FR*/ 	void rgb_lcd::curseur(void){

		if(etatCurseur!=1){
			cursor();
			etatCurseur=1;
		}
}
/*ARD US*/ 	void rgb_lcd::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}


//---------- Writing
/////////////////////////////////////////////////////////////////////////////////////
// INT
/*EDU FR */ void rgb_lcd::ecrire(int16_t nbr){ 

		newWrite(nbr);
}
/*EDU US*/ 	void rgb_lcd::newWrite(int16_t nbr){

	//--- On convertit en String
		DEBUG2("Le nbr recu est : ", nbr); 
		
		//- Enregistrement
		String nbrToStr;
		nbrToStr=String(nbr);
		DEBUG2("Le nbr converti en chaine est : ", nbrToStr);
		effectiveNewWrite(nbrToStr);
}

/*EDU FR */ void rgb_lcd::ecrire(double nbr){newWrite(nbr);}
/*EDU US*/	void rgb_lcd::ecrire(long nbr){
	newWrite((double)nbr);
}								

/*EDU US*/ 	void rgb_lcd::newWrite(double nbr){

	//--- On convertit en String
		DEBUG2("Le nbr recu est : ", nbr); 
		
		//- Enregistrement
		String nbrToStr;
		nbrToStr = floatToString(nbr);
		DEBUG2("Le nbr converti en chaine est : ", nbrToStr);
		effectiveNewWrite(nbrToStr);
}

// STRING
/*EDU FR */ void rgb_lcd::ecrire(String newString){ 

		newWrite(newString);
}
/*EDU US*/ 	void rgb_lcd::newWrite(String newString){

		effectiveNewWrite(newString);
}
/*EDU US*/ 	void rgb_lcd::effectiveNewWrite(String newString){
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
				write(tempRow[c]);
					
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


/*EDU US*/	String	rgb_lcd::floatToString(float val, uint8_t widthNbr, uint8_t minWidth){

	//-- On sauvegarde la valeur;
		float floatVal=val;
	//-- On initialise un tableau;
		char resultTabChar[widthNbr]; 
		for(uint8_t i=0;i<widthNbr;i++){
			resultTabChar[i]=NULL;
		}
	//-- On utilise la fonction de conversion
		dtostrf(floatVal, minWidth, nbrOfDec, resultTabChar);
	//-- On construit une chaîne String
		String resultString ="";
		for(uint8_t i=0;i<widthNbr;i++){
			if(resultTabChar[i]==NULL) break;
			if(resultTabChar[i]=='.'){resultTabChar[i]=',';}   // Remplace le point par la virgule
			resultString+=resultTabChar[i];
		}
	return resultString;
}

// Turn on and off the blinking cursor
void rgb_lcd::noBlink(){
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void rgb_lcd::blink(){
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void rgb_lcd::scrollDisplayLeft(void){
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void rgb_lcd::scrollDisplayRight(void){
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void rgb_lcd::leftToRight(void){
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rgb_lcd::rightToLeft(void){
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void rgb_lcd::autoscroll(void){
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void rgb_lcd::noAutoscroll(void){
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void rgb_lcd::createChar(uint8_t location, uint8_t charmap[]){

    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    
    
    unsigned char dta[9];
    dta[0] = 0x40;
    for(int i=0; i<8; i++)
    {
        dta[i+1] = charmap[i];
    }
    i2c_send_byteS(dta, 9);
}

/*********** mid level commands, for sending data/cmds */

// send command
inline void rgb_lcd::command(uint8_t value){
    unsigned char dta[2] = {0x80, value};
    i2c_send_byteS(dta, 2);
}

// send data
// size_t contient le nombre de cractères 


inline size_t rgb_lcd::write(uint8_t value){
	unsigned char dta[2] = {0x40, value};
    i2c_send_byteS(dta, 2);
    return 1; // assume sucess
}


void rgb_lcd::setReg(unsigned char addr, unsigned char dta){
    Wire.beginTransmission(RGB_ADDRESS); // transmit to device #4
    Wire.write(addr);
    Wire.write(dta);
    Wire.endTransmission();    // stop transmitting
}


//---------- Gestion des strings
/*EDU FR*/ void rgb_lcd::retroeclairage(String couleur){
	String r1="Rouge", r2="rouge", r3="ROUGE", r4="Red",   r5="red",   r6="RED",   r7="255,0,0";
	String v1="Vert" , v2="vert" , v3="VERT" , v4="Green", v5="green", v6="GREEN", v7="0,255,0";
	String b1="Bleu" , b2="bleu" , b3="BLEU" , b4="Blue",  b5="blue",  b6="BLUE",  b7="0,0,255";
	String l1="Violet",l2="violet",l3="VIOLET",l4="Purple",l5="purple",l6="PURPLE",l7="255,0,255";
	String j1="Jaune", j2="jaune", j3="JAUNE" ,j4="Yellow",j5="yellow",j6="YELLOW",j7="255,255,0";
	String w1="Blanc", w2="blanc", w3="BLANC" ,w4="White",w5="white",w6="YWHITE",w7="255,255,255";
	String x1="Aucune",x2="aucune",x3="AUCUNE" ,x4="No",x5="no",x6="NO",x7="0,0,0";
	if     (couleur==r1||couleur==r2||couleur==r3||couleur==r4||couleur==r5||couleur==r6){setRGB(255,0,0);}
	else if(couleur==v1||couleur==v2||couleur==v3||couleur==v4||couleur==v5||couleur==v6){setRGB(0,255,0);}
	else if(couleur==b1||couleur==b2||couleur==b3||couleur==b4||couleur==b5||couleur==b6){setRGB(0,0,255);}
	else if(couleur==l1||couleur==l2||couleur==l3||couleur==l4||couleur==l5||couleur==l6){setRGB(255,0,255);}
	else if(couleur==j1||couleur==j2||couleur==j3||couleur==j4||couleur==j5||couleur==j6){setRGB(255,150,0);}
	else if(couleur==w1||couleur==w2||couleur==w3||couleur==w4||couleur==w5||couleur==w6){setRGB(255,255,255);}
	else if(couleur==x1||couleur==x2||couleur==x3||couleur==x4||couleur==x5||couleur==x6){setRGB(0,0,0);}
	else{
		retroeclairage(255,255,255);
	}
}

//---------- Gestion des int
/*EDU FR*/ void rgb_lcd::retroeclairage(long r, long g, long b){
	unsigned char rUC;
	unsigned char gUC;
	unsigned char bUC;
	if(r<0){r=0;}
	if(g<0){r=0;}
	if(b<0){r=0;}
	if(r>1000){r=1000;}
	if(g>1000){g=1000;}
	if(b>1000){b=1000;}
	rUC = map(r,0,1000,0,255);
	gUC = map(g,0,1000,0,255);
	bUC = map(b,0,1000,0,255);
	setRGB(rUC,gUC,bUC);
}
void rgb_lcd::setRGB(unsigned char r, unsigned char g, unsigned char b)
{
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
}

const unsigned char color_define[4][3] = 
{
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

void rgb_lcd::setColor(unsigned char color)
{
    if(color > 3)return ;
    setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
}
