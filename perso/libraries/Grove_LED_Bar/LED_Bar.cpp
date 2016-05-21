/*
  LED bar library V1.0
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.

  Original Author: LG

  Modify: Loovee, 2014-2-26
  User can choose which Io to be used.
  
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
  
  -----------
  
  EDU : Librairie augmentée par David Souder (souder.d@gmail.com) pour www.duinoedu.com
  Version du 04/01/2015
  
  
  
  
*/

#include <Arduino.h>

#include "LED_Bar.h"


LED_Bar::LED_Bar(int pinClk, int pinDta)
{
    __pinClk = pinClk;
    __pinDta = pinDta;

    __led_state = 0x00;
    
    pinMode(__pinClk, OUTPUT);
    pinMode(__pinDta, OUTPUT);
	
	// Voltmeter
	/*EDU US*/ m_mem.pinModeOk=false;     // Default config analogPin = not ok !
    /*EDU US*/ m_mem.range =1023;         // Default range CAN 0~1023
	/*EDU US*/ m_mem.nbrOfLed =EDU_LEDBAR_NBR_LED;       
	
	for(char i=0;i<EDU_LEDBAR_NBR_LED;i++){ledMemory[i]=0;} // Init. table of led to 0
	clearAll();
	
}

void LED_Bar::latchData()
{
    digitalWrite(__pinDta, LOW);
    delayMicroseconds(10);

    for(int i=0; i<4; i++)
    {
        digitalWrite(__pinDta, HIGH);
        digitalWrite(__pinDta, LOW);
    }

}

void LED_Bar::send16bitData(unsigned int data)
{
    for(int i=0; i<16; i++)
    {
	    // HEX : 8000 => 1111 0000 0000 0000
        unsigned int state = data&0x8000 ? HIGH : LOW;
        digitalWrite(__pinDta, state);

        state = digitalRead(__pinClk) ? LOW : HIGH;
        digitalWrite(__pinClk, state);

        data <<= 1;
    }
}

// set led single bit, a bit contrl a led
// such as, index_bits = 0x05, then led 0  and led 3 will on, the others will off
void LED_Bar::ledIndexBit(unsigned int index_bits)
{

    send16bitData(CMDMODE);

    for (int i=0;i<12;i++)
    {
        unsigned int state = (index_bits&0x0001) ? ON : SHUT;
        send16bitData(state);

        index_bits = index_bits>>1;
    }

    latchData();
}

// set level, there level 0-10
// level 0 means all leds off while level 5 means 5led on and the other will off
void LED_Bar::setLevel(int level)
{

    if(level>10)return;

    send16bitData(CMDMODE);

    for(int i=0;i<12;i++)
    {
        unsigned int state1 = (i<level) ? ON : SHUT;
        
        send16bitData(state1);
    }

    latchData();
}

// control a single led
// num - which led
// st - 1: on   0: off
void LED_Bar::setSingleLed(int num, int st)
{
    if(num>10)return;
    __led_state = st ? (__led_state | (0x01<<num)) : (__led_state & ~(0x01<<num));
    ledIndexBit(__led_state);

}

/*EDU FR*/ void LED_Bar::effacerTout(void){clearAll();}
/*EDU US*/ void LED_Bar::clearAll(void){
 	for(int i=0;i<10;i++){
		setSingleLed(i,0);
	}
} 

void LED_Bar::allumerTout(void){
 	for(int i=0;i<10;i++){
		setSingleLed(i,1);
	}
} 

/*EDU FR*/ void LED_Bar::afficher(int a,int b,int c, int d,int e,int f,int g,int h,int i,int j){
		
		display(a,b,c,d,e,f,g,h,i,j);
}
				
/*EDU US*/ void LED_Bar::display(int a,int b,int c, int d,int e,int f,int g,int h,int i,int j){
	int segments[] = {a,b,c,d,e,f,g,h,i,j};
	for(int i=0;i<10;i++){
		setSingleLed(9-i,segments[i]);
	}
}

/*EDU US*/ float LED_Bar::afficherTensionEntree(int const analogPin){return displayVoltage(analogPin);}
/*EDU US*/ float LED_Bar::displayVoltagePin(int const analogPin ){
	// Save configuration
		m_mem.pin=analogPin;
	// Set the pin mode if not ok 
		if(!m_mem.pinModeOk){
			pinMode(m_mem.pin,INPUT);
			m_mempinModeOk=true;
		}

	// Measure
		m_mem.measureCan=analogRead(m_mem.pin);
		delay(15);

	    return displayVoltage(m_mem.measureCan);
}

float LED_Bar::afficherTension(long val, long min, long max){return displayVoltage(val,min,max);}
float LED_Bar::displayVoltage(long val, long min, long max){
	
	//-- On stocke les valeurs en mémoire
		m_mem.val=val;
		m_mem.minVal=min;
		m_mem.maxVal=max;
	
	//-- On récupère la valeur souhaitée
		m_mem.val=val;
	
	//-- On écrète si besoin (on peut aussi utiliser la fonction Arduino)
		if(m_mem.val>m_mem.maxVal){m_mem.val=m_mem.maxVal;}
		if(m_mem.val<m_mem.minVal){m_mem.val=m_mem.minVal;}
	
	//-- On calcule la correspondance en nombre de LED
		uint8_t resultNbrOfLed=map(m_mem.val,m_mem.minVal,m_mem.maxVal,0,m_mem.nbrOfLed+1); 		
		
	//-- On détermine s'il faut actualiser l'affichage		
		if(resultNbrOfLed!=m_mem.measureInNbrOfLed){
			//-- On stocke la nouvelle valeur en memoire
				m_mem.measureInNbrOfLed = resultNbrOfLed;
			//-- On met à jour le tableau des nouvelles valeurs
				for(char i=0;i<EDU_LEDBAR_NBR_LED;i++){ledNewSet[i]=0;}        // On efface
				for(char i=0;i<m_mem.measureInNbrOfLed;i++){             // On set
					ledNewSet[i]=1;
				}
		
			//-- On met à jour l'affichage 
			for(int i=0;i<EDU_LEDBAR_NBR_LED;i++){
				//-- On teste LED par LED pour voir s'il y a des changements
				if(ledNewSet[i]!=ledMemory[i]){
					//- Toggle
					setSingleLed(9-i,(int)ledNewSet[i]);
					//- On sauvegarde le changement
					ledMemory[i]=ledNewSet[i];
				}
			}
		} // END if
	
	return m_mem.val;
}

