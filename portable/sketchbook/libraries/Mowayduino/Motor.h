//********************************************************************************
//*    Motor.h                                                         *
//*    version: 1.1                                                              *
//*    This library contains all the necessary functions to control mowayduino   *
//*    				                                           									       *
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

#ifndef Motors_h
#define Motors_h

//***********************************
//  Libraries
//***********************************
#include "Arduino.h"

//***********************************
//  Definitions
//***********************************
#define SPEED_MIN 20
#define SPEED_MAX 200
#define MAX_TIMEOUT 1024
#define ENCODER_DISTANCE 2
#define ENCODER_ANGLE 1.906

#define TURN_CORR 30
#define BACK_CORR 165

// Speed for tuning motors
#define V_MAX  180
#define V_MIN  80

// EEPROM addresses
#define EE_OFFSET_R    0xF4
#define EE_OFFSET_L    0xF5
#define EE_OFFSET_BACK_R    0xF6
#define EE_OFFSET_BACK_L    0xF7

//***********************************
//  Class
//***********************************
class Motor{
public:
  byte offset;
  byte offset_l;
  byte offset_r;

  void beginMotor();
  void Straight(byte speed);
  void Straight(byte speed, int distance);
  void Back(byte speed);
  void Back(byte speed, int distance);
  void TurnRight(byte speed);
  void TurnRight(byte speed, int angle);
  void TurnLeft(byte speed);
  void TurnLeft(byte speed, int angle);
  void RotateRight(byte speed);
  void RotateRight(byte speed, int angle);
  void RotateLeft(byte speed);
  void RotateLeft(byte speed, int angle);
  
  void Move(int left_speed, int right_speed);
  void Stop();
    
	void Free(byte speedR, boolean backR, byte speedL, boolean backL);	
	void Free(byte speedR, boolean backR, byte speedL, boolean backL, int sdistance);  
		
	void ReduceInertia();	
	void MotorTune(byte speed);
	int EncoderDifference(int counts);
		
private:
  byte EncodeAngle(byte speed, int angle);
};

#endif

