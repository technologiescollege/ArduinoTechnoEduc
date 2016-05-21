/****************************************************************************/	
//	Function: Cpp file for DS1307
//	Hardware: Grove - RTC
//	Arduino IDE: Arduino-1.0
//	Author:	 FrankieChu		
//	Date: 	 Jan 19,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
//	Augmentépar David Souder
// 	Pour www.duinoedu.com
//	Dernière modification le : 31/05/2015
//
/****************************************************************************/

#include <Wire.h>
#include "DS1307.h"

/*EDU US*/ DS1307::DS1307(){
	second=0;
	minute=0;
	hour=0; 
	dayOfWeek=0;		
	dayOfMonth=0;
	month=0;
	year=0;

}

uint8_t DS1307::decToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}

//Convert binary coded decimal to normal decimal numbers
uint8_t DS1307::bcdToDec(uint8_t val)
{
	return ( (val/16*10) + (val%16) );
}

void DS1307::brancher(){begin();}
void DS1307::begin(){
	Wire.begin();
}
/*Function: The clock timing will start */
void DS1307::startClock(void){        // set the ClockHalt bit low to start the rtc
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  second = Wire.read() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)second);                    // write seconds back and start the clock
  Wire.endTransmission();
}
/*Function: The clock timing will stop */
void DS1307::stopClock(void)         // set the ClockHalt bit high to stop the rtc
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  second = Wire.read() | 0x80;       // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)second);                    // write seconds back and stop the clock
  Wire.endTransmission();
}
/****************************************************************/
/*Function: Read time and date from RTC	*/
void DS1307::getTime(){
    // Reset the register pointer
	Wire.beginTransmission(DS1307_I2C_ADDRESS);
	Wire.write((uint8_t)0x00);
	Wire.endTransmission();  
	Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
	// A few of these need masks because certain bits are control bits
	second	   = bcdToDec(Wire.read() & 0x7f);
	minute	   = bcdToDec(Wire.read());
	hour	   = bcdToDec(Wire.read() & 0x3f);// Need to change this if 12 hour am/pm
	dayOfWeek  = bcdToDec(Wire.read());
	dayOfMonth = bcdToDec(Wire.read());
	month      = bcdToDec(Wire.read());
	year	   = bcdToDec(Wire.read())+2000;
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void DS1307::setTime(){
	Wire.beginTransmission(DS1307_I2C_ADDRESS);
	Wire.write((uint8_t)0x00);
	Wire.write(decToBcd(second));// 0 to bit 7 starts the clock
	Wire.write(decToBcd(minute));
	Wire.write(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6 
	Wire.write(decToBcd(dayOfWeek));
	Wire.write(decToBcd(dayOfMonth));
	Wire.write(decToBcd(month));
	if(year<2000){
		Wire.write(decToBcd(year));
	}else{
		Wire.write(decToBcd(year-2000));
	}
	Wire.endTransmission();
}

//-----REGLAGE DE L'HEURE
/*EDU FR*/void DS1307::ecrireHeure(uint8_t heures, uint8_t minutes, uint8_t secondes){
	fillByHMS(heures, minutes, secondes);
	setTime();
}
/*ARD US*/void DS1307::fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second){
	// assign variables
	hour = _hour;
	minute = _minute;
	second = _second;
}

/*EDU FR*/void DS1307::ecrireDate(uint8_t jour, uint8_t mois, uint16_t annee){
	fillByYMD(annee, mois, jour, true);
	setTime();
}
/*ARD US*/void DS1307::fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day, boolean plus2000){
	if(plus2000==true){
		year = _year;

	}else{
		year = _year-2000;
	}
	
	month = _month;
	dayOfMonth = _day;
}
void DS1307::fillDayOfWeek(uint8_t _dow){
	dayOfWeek = _dow;
}

//-----RECUPERATION DES VALEURS
/*EDU FR*/int		DS1307::lireSecondes(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();
	//-- Retour
		return second;
}
/*EDU FR*/int		DS1307::lireMinutes(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();	// Peut-être optimiser plus tard les requêtes I2C
	//-- Retour
		return minute;
}
/*EDU FR*/int		DS1307::lireHeures(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();	// Peut-être optimiser plus tard les requêtes I2C
	//-- Retour
		return hour;
}

/*EDU FR*/int		DS1307::lireJour(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();	// Peut-être optimiser plus tard les requêtes I2C
	//-- Retour
		return dayOfMonth;
}
/*EDU FR*/int		DS1307::lireMois(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();	// Peut-être optimiser plus tard les requêtes I2C
	//-- Retour
		return month;
}
/*EDU FR*/int		DS1307::lireAnnee(){
	//-- On lance une lecture => les valeurs sont sauvegardées dans les attributs de l'objets
		getTime();	// Peut-être optimiser plus tard les requêtes I2C
	//-- Retour
		return year;
}	
	
