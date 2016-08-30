/****************************************************************************/	
//	Function: Set time and get the time from RTC chip(DS1307) and display 
//			  it on the serial monitor.
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
#include <Wire.h>
#include "DS1307.h"

DS1307 clock;
void setup()
{
	clock.begin();
	clock.ecrireDate(18,2,2015);
	clock.ecrireHeure(18,15,0);

}
void loop(){

}

