
//********************************************************************************
//*    MowayduinoRobot.cpp                                                       *
//*    This library contains all the necessary functions to control mowayduino   *
//*		 DATE: 16/12/2013    Rev 1.3    Minirobots, S.L.													 *
//*				- Added "Ledsoff" function																						 *
//********************************************************************************
//*    This program is free software; you can redistribute it and/or modify      *
//*    it under the terms of the GNU General Public License as published by      *
//*    the Free Software Foundation; either version 3 of the License, or         *
//*    (at your option) any later version.                                       *
//*                                                                              *
//*    This program is distributed in the hope that it will be useful,           *
//*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
//*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
//*    GNU General Public License for more details.                              *
//*                                                                              *
//*    You should have received a copy of the GNU General Public License along   *
//*    with this program; if not, write to the Free Software Foundation, Inc.,   *
//*    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
//********************************************************************************
#include "MowayduinoRobot.h"
#include <SPI.h>
#include <EEPROM.h>

mowayduinorobot::mowayduinorobot()
{
}

//**************************************************
//  NAME: beginMowayduino
//  DESCRIPTION: Configures microcontroler
//**************************************************
void mowayduinorobot::beginMowayduino()
{
  digitalWrite(CS_IO,HIGH);
  digitalWrite(RF_CS,HIGH);
  digitalWrite(CS_ACC,HIGH);
  digitalWrite(CS_EXP,HIGH);
  pinMode(RF_CE, OUTPUT); 
  pinMode(CS_EXP, OUTPUT); 
  pinMode(IRQ_EXP, INPUT_PULLUP);     // Pull up for Wifi module
  pinMode(RF_IRQ, INPUT); 
	
	digitalWrite(CS_EXP,HIGH);
  digitalWrite(CE_EXP,HIGH);
  
  pinMode(SPEAKER, OUTPUT);    
	pinMode(CS_ACC, OUTPUT);  
  pinMode(RF_CS, OUTPUT);   
  pinMode(CE_EXP, OUTPUT); 
  pinMode(CS_IO, OUTPUT); 
  pinMode(ENC_L, INPUT); 
  pinMode(ENC_R, INPUT);  	 
	
	// Motor configuration
	pinMode(MOT_L_N, OUTPUT);
	pinMode(MOT_R_P, OUTPUT);
	pinMode(MOT_R_N, OUTPUT);  
  pinMode(MOT_L_P, OUTPUT); 	
	// Sleep motors controller IC (Avoid consumption)
/*	
  digitalWrite(MOT_L_P, LOW);
  digitalWrite(MOT_L_N, LOW);
  digitalWrite(MOT_R_P, LOW);
  digitalWrite(MOT_R_N, LOW);	
*/
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  delay(10);

  //Acceloremeter config
  beginAcc();

  //IO Expander config
  beginIO();
  beginMotor();	
}

//**************************************************
//  NAME: readObstacle
//  DESCRIPTION: Reads the values of the obstacle sensors     
//**************************************************
void mowayduinorobot::readObstacle()
{    
  bitWrite(PORTA_IO,6,1);
  updateIO();
  delayMicroseconds(40);
  Obstacle[IR_CL]=analogRead(IR_CL_AD);
  bitWrite(PORTA_IO,6,0);
  updateIO();  
  while(analogRead(IR_CL_AD) > 0);
  
  delayMicroseconds(500);
    
  bitWrite(PORTA_IO,6,1);
  updateIO();
  delayMicroseconds(40);
  Obstacle[IR_CR]=analogRead(IR_CR_AD);
  bitWrite(PORTA_IO,6,0);    
  updateIO();
  while(analogRead(IR_CR_AD) > 0);   
  
  delayMicroseconds(500);  
  
  bitWrite(PORTA_IO,6,1);
  updateIO();
  delayMicroseconds(40);
  Obstacle[IR_LR]=analogRead(IR_LR_AD);
  bitWrite(PORTA_IO,6,0);
  updateIO();    
  while(analogRead(IR_LR_AD) > 0);
  
  delayMicroseconds(500);
 
  bitWrite(PORTA_IO,6,1);
  updateIO();
  delayMicroseconds(40);
  Obstacle[IR_LL]=analogRead(IR_LL_AD);
  bitWrite(PORTA_IO,6,0);
  updateIO();
  while(analogRead(IR_LL_AD) > 0);          
}

//**************************************************
//  NAME: readLine
//  DESCRIPTION: Reads the values of the line sensors     
//**************************************************
void mowayduinorobot::readLine()
{
  bitWrite(PORTB_IO,5,1);
  updateIO();
	
  delay(1);
  readIO();
  Line[LINE_L]=bitRead(PORTA_IO,2);
  Line[LINE_R]=bitRead(PORTA_IO,1);

  bitWrite(PORTB_IO,5,0);
  updateIO();
}

//**************************************************
//  NAME: readLight
//  DESCRIPTION: Reads the value of the light sensor
//  RETURNS: 0->No light
//           1024->Maximum brightness  
//**************************************************
int mowayduinorobot::readLight()
{
  return (analogRead( LIGHT_AD ));
}

//**************************************************
//  NAME: readBatt
//  DESCRIPTION: Reads the percenatge of battery level
//  RETURNS: 0% - 100%
//**************************************************
int mowayduinorobot::readBatt()
{
  int batt;
  
  batt = analogRead(BATT_AD);	
  
  if(batt > MAX_BAT_ADC)
    batt = 100;
  else if(batt < MIN_BAT_ADC)
    batt = 0;
  else
    batt = 0.77*(batt - MIN_BAT_ADC);
  
  return batt;
}

//**************************************************
//  NAME: readMic
//  DESCRIPTION: Reads the microphone detected level
//  RETURNS: 0 (silence) - 1024 (loud noise)
//**************************************************
int mowayduinorobot::readMic()
{
  return (analogRead( MIC_AD ));
}

//**************************************************
//  NAME: readAcc
//  DESCRIPTION: Reads the acceleration values of 3 axis.
//  RETURNS:  * Accelerometer[X_ACC] -> x axis value
//            * Accelerometer[Y_ACC] -> y axis value
//            * Accelerometer[Z_ACC] -> z axis value
//**************************************************
void mowayduinorobot::readAcc()
{
  // take the SS pin low to select the chip:
  digitalWrite(CS_ACC,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(XOUT8_R);
  accel_data[X_ACC]=SPI.transfer(0x00) - glb_x_offset;
  digitalWrite(CS_ACC,HIGH); 

  // take the SS pin low to select the chip:
  digitalWrite(CS_ACC,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(YOUT8_R);
  accel_data[Y_ACC]=SPI.transfer(0x00) - glb_y_offset;
  digitalWrite(CS_ACC,HIGH); 

  // take the SS pin low to select the chip:
  digitalWrite(CS_ACC,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(ZOUT8_R);
  accel_data[Z_ACC]=SPI.transfer(0x00) - glb_z_offset;
  digitalWrite(CS_ACC,HIGH); 
  
  // Format data in "g"
  Accelerometer[X_ACC] = (float)accel_data[X_ACC]/0x3F;
  Accelerometer[Y_ACC] = (float)accel_data[Y_ACC]/0x3F;
  Accelerometer[Z_ACC] = (float)accel_data[Z_ACC]/0x3F;
}

//**********************************
//	LEDs control
//**********************************
void mowayduinorobot::Redon()
{
  bitWrite(PORTB_IO,1,0);	
  updateIO();
}

void mowayduinorobot::Redoff()
{
  bitWrite(PORTB_IO,1,1);
  updateIO();
}

void mowayduinorobot::Greenon()
{
  bitWrite(PORTA_IO,7,0);
  updateIO();
}

void mowayduinorobot::Greenoff()
{
  bitWrite(PORTA_IO,7,1);
  updateIO();
}

void mowayduinorobot::Blueon()
{
  bitWrite(PORTB_IO,0,0);
  updateIO();
}

void mowayduinorobot::Blueoff()
{
  bitWrite(PORTB_IO,0,1);
  updateIO();
}

void mowayduinorobot::Fronton()
{
  bitWrite(PORTA_IO,5,1);
  updateIO();
}

void mowayduinorobot::Frontoff()
{
  bitWrite(PORTA_IO,5,0);
  updateIO();
}

void mowayduinorobot::Brakeon()
{
  bitWrite(PORTA_IO,0,1);
  updateIO();
}

void mowayduinorobot::Brakeoff()
{
  bitWrite(PORTA_IO,0,0);
  updateIO();
}

void mowayduinorobot::Ledsoff()
{
	bitWrite(PORTB_IO,1,1);		// Red
	bitWrite(PORTA_IO,7,1);		// Green
	bitWrite(PORTB_IO,0,1);		// Blue
	bitWrite(PORTA_IO,5,0);		// Front
  bitWrite(PORTA_IO,0,0);		// Brake
  updateIO();
}

void mowayduinorobot::Speakeroff()
{ 
  analogWrite(SPEAKER, 0);  
}

void mowayduinorobot::Speakeron()
{
  // 50% of duty cycle
  analogWrite(SPEAKER, 128);
}


void mowayduinorobot::beginIO()
{
  PORTA_IO=0x80;
  PORTB_IO=0x43;

  // take the SS pin low to select the chip:
  digitalWrite(CS_IO,LOW);
  delayMicroseconds(1);
  //  send in the address and value via SPI:
  SPI.transfer(IO_ADDRESS_W);
  SPI.transfer(0x00);
  SPI.transfer(TRISA_IO);
  SPI.transfer(TRISB_IO);
  // take the SS pin high to de-select the chip:
  digitalWrite(CS_IO,HIGH); 

  delayMicroseconds(50);

  // take the SS pin low to select the chip:
  digitalWrite(CS_IO,LOW);
  delayMicroseconds(1);
  //  send in the address and value via SPI:
  SPI.transfer(IO_ADDRESS_W);
  SPI.transfer(OLAT_ADD);
  SPI.transfer(PORTA_IO);
  SPI.transfer(PORTB_IO);
  // take the SS pin high to de-select the chip:
  digitalWrite(CS_IO,HIGH); 
}

void mowayduinorobot::updateIO()
{
  // take the SS pin low to select the chip:
  digitalWrite(CS_IO,LOW);
  delayMicroseconds(1);
  //  send in the address and value via SPI:
  SPI.transfer(IO_ADDRESS_W);
  SPI.transfer(OLAT_ADD);
  SPI.transfer(PORTA_IO);
  SPI.transfer(PORTB_IO);
  // take the SS pin high to de-select the chip:
  digitalWrite(CS_IO,HIGH); 
}

void mowayduinorobot::readIO()
{
  // take the SS pin low to select the chip:
  digitalWrite(CS_IO,LOW);
  delayMicroseconds(1);
  //  send in the address and value via SPI:
  SPI.transfer(IO_ADDRESS_R);
  SPI.transfer(PORT_ADD);
  PORTA_IO=SPI.transfer(0x00);
  PORTB_IO=SPI.transfer(0x00);
  // take the SS pin high to de-select the chip:
  digitalWrite(CS_IO,HIGH); 
}

//**************************************************
//  NAME: beginAcc
//  DESCRIPTION: Configures accelerometer
//**************************************************
void mowayduinorobot::beginAcc()
{
  // take the SS pin low to select the chip:
  digitalWrite(CS_ACC,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(I2CDD_W);
  SPI.transfer(0xFF);
  digitalWrite(CS_ACC,HIGH); 

  delayMicroseconds(50);

  // take the SS pin low to select the chip:
  digitalWrite(CS_ACC,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(MCTL_W);
  SPI.transfer(0x05);
  digitalWrite(CS_ACC,HIGH); 

  // Read tuned values
  glb_x_offset = EEPROM.read(EE_X_ACC);
  glb_y_offset = EEPROM.read(EE_Y_ACC);
  glb_z_offset = EEPROM.read(EE_Z_ACC);  
}

//**************************************************
//  NAME: TuneAcc
//  DESCRIPTION: Tune the accelerometer value offsets.
//  The robot has to be over a flat surface and stopped
//  before calling this function.
//**************************************************
void mowayduinorobot::TuneAcc()
{
  int x_offset, y_offset, z_offset;
  char x_max, x_min, y_max, y_min, z_max, z_min;
  boolean passed;
    
  passed = false;
  Greenoff();
  Blueoff();
  Redon();
  
  while(passed == false)
  {
    x_max = -128;
    y_max = x_max;
    z_max = x_max;
    x_min = 127;
    y_min = x_min;
    z_min = x_min;
    
    for(int i=0; i<5; i++)
    {
      glb_x_offset = 0;
      glb_y_offset = glb_x_offset;
      glb_z_offset = glb_x_offset;
      readAcc();    
      
      x_offset = accel_data[X_ACC];
      y_offset = accel_data[Y_ACC];
      z_offset = accel_data[Z_ACC] - 0x3F;    // 0x3F -> +g acceleration value of z axis (9.8m/s2)
            
      // Save the amximum and minimum values of each axis
      if(x_offset > x_max)
        x_max = x_offset;
      if(x_offset < x_min)
        x_min = x_offset;
        
      if(y_offset > y_max)
        y_max = y_offset;
      if(y_offset < y_min)
        y_min = y_offset;
        
      if(z_offset > z_max)
        z_max = z_offset;
      if(z_offset < z_min)
        z_min = z_offset;                   

      delay(250);
    }
    if(x_max-x_min<4 && y_max-y_min<4 && z_max-z_min<4)
    {   
      // If the difference is less than 4, it is considered
      // that the robot is stopped
      passed = true;
      Redoff();
      Greenon();
      Speakeron();
			delay(200);
      Greenoff();
			Speakeroff();
    } 
  }
    
  // Save offset in global variables
  glb_x_offset = x_offset;
  glb_y_offset = y_offset;
  glb_z_offset = z_offset;
  
  // Save offset values in EEPROM
  EEPROM.write(EE_X_ACC, x_offset);
  EEPROM.write(EE_Y_ACC, y_offset);
  EEPROM.write(EE_Z_ACC, z_offset);    
}

void mowayduinorobot::TuneMotors()
{
  EEPROM.write(EE_MOTOR_R,0);
  EEPROM.write(EE_MOTOR_L,0);
}
