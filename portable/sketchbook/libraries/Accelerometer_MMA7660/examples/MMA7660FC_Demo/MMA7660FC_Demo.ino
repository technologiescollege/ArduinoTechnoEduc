/*****************************************************************************/
//	Function:    Get the accelemeter of the x/y/z axis. 
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 10,2013
//	Version: v0.9b
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
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <Wire.h>
#include "MMA7660.h"
MMA7660 accelemeter;
void setup()
{
	accelemeter.init();  
	Serial.begin(9600);
}
void loop()
{
	int8_t x;
	int8_t y;
	int8_t z;
	float ax,ay,az;
	accelemeter.getXYZ(&x,&y,&z);
	
	Serial.print("x = ");
    Serial.println(x); 
    Serial.print("y = ");
    Serial.println(y);   
    Serial.print("z = ");
    Serial.println(z);
	
	accelemeter.getAcceleration(&ax,&ay,&az);
    Serial.println("accleration of X/Y/Z: ");
	Serial.print(ax);
	Serial.println(" g");
	Serial.print(ay);
	Serial.println(" g");
	Serial.print(az);
	Serial.println(" g");
	Serial.println("*************");
	delay(500);
}


