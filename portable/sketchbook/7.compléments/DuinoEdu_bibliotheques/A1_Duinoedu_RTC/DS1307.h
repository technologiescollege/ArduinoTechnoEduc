/****************************************************************************/	
//	Function: Header file for DS1307
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
/****************************************************************************/

/* 
	Modifié et augmenté par David Souder pour DuinoEDU.com - souder.d@gmail.com 
	Version du 18/02/2015
*/


#ifndef __DS1307_H__
#define __DS1307_H__

#include <Arduino.h>

#define DS1307_I2C_ADDRESS 0x68

#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6
#define SUN 7

class DS1307
{
private:
	uint8_t decToBcd(uint8_t val);
	uint8_t bcdToDec(uint8_t val);

public:

/*EDU US*/		    DS1307();

/*EDU FR*/void 		brancher();
/*ARD US*/void 		begin();

/*ARD US*/void		startClock	(void);
/*ARD US*/void 		stopClock	(void);
/*ARD US*/void 		setTime		(void);
/*ARD US*/void 		getTime		(void);

/*EDU FR*/void		ecrireHeure	(uint8_t heures, uint8_t minutes=0, uint8_t secondes=0);
/*ARD US*/void 		fillByHMS	(uint8_t _hour, uint8_t _minute, uint8_t _second);

/*EDU FR*/void		ecrireDate	(uint8_t jour, uint8_t mois, uint16_t annee);	
/*ARD US*/void 		fillByYMD	(uint16_t _year, uint8_t _month,  uint8_t _day, boolean plus2000=false);

/*EDU FR*/int		lireSecondes();	
/*EDU FR*/int		lireMinutes();	
/*EDU FR*/int		lireHeures();

/*EDU FR*/int		lireJour();	
/*EDU FR*/int		lireMois();	
/*EDU FR*/int		lireAnnee();
	

	void fillDayOfWeek(uint8_t _dow);

	// Pas de possibilité d'utiliser des alias (dommage)
	uint8_t 	second;
	uint8_t 	minute;
	uint8_t 	hour; 
	uint8_t 	dayOfWeek;		// day of week, 1 = Monday
	uint8_t 	dayOfMonth;
	uint8_t 	month;
	uint16_t 	year;
};

#endif
