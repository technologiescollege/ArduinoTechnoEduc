//********************************************************************************
//*    MowayduinoRobot.h                                                           *
//*    version: 1.1                                                              *
//*    This library contains all the necessary functions to control mowayduino   *
//*    				                                                 *
//*    Copyright (C) 2013  Minirobots S.L.                                       *
//*                                                                              *
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

#ifndef mowayduinorobot_h
#define mowayduinorobot_h

#include "Arduino.h"
#include "MowayduinoIo.h"
#include "Motor.h"
#include "MowayduinoRf.h"
#include "Camera.h"

#define IR_LL 0
#define IR_CL 1
#define IR_CR 2
#define IR_LR 3

#define LINE_L 0
#define LINE_R 1

//**********IO Expander registers**********//
#define IO_ADDRESS_R 	0x41
#define IO_ADDRESS_W 	0x40
#define TRIS_ADD		0x00
#define PORT_ADD		0x12
#define OLAT_ADD		0x12
//***DEBUG: brake led estaba configurado como entrada
//#define TRISA_IO		0x1F
#define TRISA_IO		0x1E
#define TRISB_IO		0x9C

//**********Accelerometer registers**********//
#define 	XOUT8_R		(0x06)<<1
#define 	YOUT8_R		(0x07)<<1
#define 	ZOUT8_R		(0x08)<<1
#define 	I2CDD_W		((0x0D)<<1)|0x80
#define 	MCTL_W		((0x16)<<1)|0x80

#define  X_ACC  0
#define  Y_ACC  1
#define  Z_ACC  2

//**********Battery******************//
#define MIN_BAT_ADC  518    //Minimun of battery equal to 3.3 V
#define MAX_BAT_ADC  648    //Maximun of battery equal to 4.2 V

//**********EEPROM addresses**********//
#define EE_MOTOR_R  0xF0
#define EE_MOTOR_L  0xF1
#define EE_X_ACC    0xF2
#define EE_Y_ACC    0xF3
#define EE_Z_ACC    0xF4


class mowayduinorobot:
public Motor, public Rf , public Camera
{
public:
  mowayduinorobot();
  void beginMowayduino();

  uint8_t PORTA_IO;
  uint8_t PORTB_IO;
  uint16_t Obstacle[4];
  void readObstacle();

  byte Line[2];
  void readLine();
  
	// Sensors
  int readLight();		//Light Sensor  
  int readBatt();			//Battery Level  
  int readMic();			//Microphone

  //Speaker
  void Speakeron();
  void Speakeroff();

  //Accelerometer
  float Accelerometer[3];
  void readAcc();

  //RGB Led
  void Redon();
  void Redoff();
  void Greenon();
  void Greenoff();
  void Blueon();
  void Blueoff();

  //Front Led
  void Fronton();
  void Frontoff();

  //Brake Led
  void Brakeon();
  void Brakeoff();
	void Ledsoff();
	
  void TuneMotors();	   
  void TuneAcc();		
	
	// Data for using in Python
	char accel_data[3];	
  
private:
  int glb_x_offset;
  int glb_y_offset;
  int glb_z_offset;
  
  void beginIO();
  void updateIO();
  void readIO();
  void beginAcc();
};


#endif

