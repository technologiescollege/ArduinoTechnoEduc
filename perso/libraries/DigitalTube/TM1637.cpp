//  Author:Frankie.Chu
//  Date:9 April,2012
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
//  Modified record:
//
/*******************************************************************************/
#include "TM1637.h"
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};//0~9,A,b,C,d,E,F                        
TM1637::TM1637(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
  m_lastValueInt=-1;
  m_lastDate=0;
  m_minRefresh=50;
  m_moyRefresh=400;

}

/*EDU FR*/void	TM1637::brancher(void){
	init();
	//---- On régle la luminosité par défaut
	set(7);			
}
void TM1637::init(void){
  clearDisplay();
}

/*EDU US*/ void TM1637::ecrire(long number){writeInt(number);}
/*EDU US*/ void TM1637::writeInt(long number){

	//---- On teste si la valeur n'est pas trop répétitive
	if(m_lastDate+m_minRefresh>millis()){return;}
	
	//---- Si la valeur peut être rafraichie, on regarde si l'écart est significatif
	if(millis()<m_lastDate+m_moyRefresh){
		//on regarde si l'écart est significatif
		if(abs(  m_lastValueInt-number)<20){
			return;
		}
	}

	m_lastDate=millis();
	
	int16_t  digit0;
	int16_t  digit1;
	int16_t  digit2;
	int16_t  digit3;
	
	if(number==m_lastValueInt){return;}
	m_lastValueInt=number;
	
	clearDisplay();

//---- X0:00
	if(number>999){
		digit3=number/1000;
		display(0, digit3);
		delay(1);
	}
	else{ 
		digit3=0;
	}

//---- 0X:00
	if(number>99){
		digit2=number/100;
		digit2%=10*digit3;
		display(1, digit2);
		delay(1);
	}
	else{
		digit2=0;
	}

//---- 00:X0			
	if(number>9){
		digit1=number/10;
		Serial.println(digit1);
		int16_t diviseur =(100*digit3+10*digit2);
		digit1%=(diviseur);
		Serial.print("Diviseur : ");Serial.println(diviseur);
		display(2, digit1);
		delay(1);
	}
	else{
		digit1=0;
	}

//---- 00:0X	
	if(number>=0){
		digit0=number;
		int32_t diviseur =1000*digit3+100*digit2+10*digit1;
		digit0%=(diviseur);
			display(3, digit0);
			delay(1);
	}
	
}


void TM1637::writeByte(int8_t wr_data)
{
  uint8_t i,count1;   
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);      
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;      
    digitalWrite(Clkpin,HIGH);
      
  }  
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);     
  pinMode(Datapin,INPUT);
  while(digitalRead(Datapin))    
  { 
    count1 +=1;
    if(count1 == 200)//
    {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
     count1 =0;
    }
    pinMode(Datapin,INPUT);
  }
  pinMode(Datapin,OUTPUT);
  
}
//send start signal to TM1637
void TM1637::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH); 
  digitalWrite(Datapin,LOW); 
  digitalWrite(Clkpin,LOW); 
} 
//End of transmission
void TM1637::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH); 
}
//display function.Write to full-screen.
void TM1637::display(int8_t DispData[])
{
  int8_t SegData[4];
  uint8_t i;
  for(i = 0;i < 4;i ++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(Cmd_SetAddr);//
  for(i=0;i < 4;i ++)
  {
    writeByte(SegData[i]);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}
//******************************************
void TM1637::display(uint8_t BitAddr,int8_t DispData)
{
  int8_t SegData;
  SegData = coding(DispData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(BitAddr|0xc0);//
  writeByte(SegData);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

/*EDU *FR*/void TM1637::effacer(void){
	//---- On signal un effacement
		m_lastValueInt=-1;
	//---- Effacement effectif
		clearDisplay();
}
void TM1637::clearDisplay(void){
	//---- On efface les 4 digits
		display(0x00,0x7f);
		display(0x01,0x7f);
		display(0x02,0x7f);
		display(0x03,0x7f);  
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(boolean PointFlag)
{
  _PointFlag = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  for(uint8_t i = 0;i < 4;i ++)
  {
    if(DispData[i] == 0x7f)DispData[i] = 0x00;
    else DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t TM1637::coding(int8_t DispData)
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + PointData;
  return DispData;
}
