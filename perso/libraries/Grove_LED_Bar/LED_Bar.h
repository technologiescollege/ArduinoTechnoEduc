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
  
  Add-on buy Souder David for www.duinoedu.com
*/
#ifndef LED_Bar_H
#define LED_Bar_H

#define CMDMODE     0x0000                          // Work on 8-bit mode
#define ON          0x00ff                          // 8-bit 1 data
#define SHUT        0x0000                          // 8-bit 0 data

// LED par LED
   #define allumerLed(val)       setSingleLed(10-val,1)
   #define eteindreLed(val)      setSingleLed(10-val,0)

// Toutes les LEDs
   // .effacerTout
   // .allumerTout
   
   #define effacer               effacerTout

// Certaines LEDs
   // .afficher(1,0...)
   
   
class LED_Bar
{

private:

    int __pinClk;                                   // pin of clk
    int __pinDta;                                   // pin of data
    
    unsigned int __led_state;                       // led state
    
private:

    void send16bitData(unsigned int data);          // send a word to led bar
    void latchData(void);                           // latch data
    
public:
    
/*ARD US*/ LED_Bar(int pinClk, int pinDta);                
/*EDU FR*/ void effacerTout(void);  
/*EDU US*/ void clearAll(void);                      
/*EDU FR*/ void allumerTout(void);                         
/*EDU FR*/ void afficher(int a=0, int b=0,int c=0,int d=0,int e=0,int f=0,int g=0,int h=0,int i=0,int j=0);  // EDU
/*EDU FR*/ void display(int a=0, int b=0,int c=0,int d=0,int e=0,int f=0,int g=0,int h=0,int i=0,int j=0);

/*EDU FR*/ float afficherTensionEntree(int const analogPin);
/*EDU US*/ float displayVoltagePin(int const analogPin);

/*EDU FR*/ float afficherTension(long val, long min=0, long max=1000);
/*EDU FR*/ float displayVoltage(long val, long min=0, long max=1000);

/*ARD US*/ void ledIndexBit(unsigned int index_bits);      // index bit led
/*ARD US*/ void setLevel(int level);                       // set level
/*ARD US*/ void setSingleLed(int num, int st);             // set si

protected:
#define     EDU_LEDBAR_NBR_LED 10		
/*EDU US*/	typedef struct{
				int   pin;                     // Pin used for the measure
				bool  pinModeOk;               // PinMode OK ?
				int   range;                   // Default range for the CAN
				int   nbrOfLed;			       // Number of Led used for the ledbar
				int   measureCan;
				float measureInVolts;
				int   measureInNbrOfLed;
				long  val;
				long  minVal;                  
				long  maxVal;
			}voltmeter;
			
			voltmeter m_mem;

			bool m_mempinModeOk;
			
/*EDU US*/  bool ledMemory[EDU_LEDBAR_NBR_LED];
/*EDU US*/  bool ledNewSet[EDU_LEDBAR_NBR_LED];
				
				



};

#endif
