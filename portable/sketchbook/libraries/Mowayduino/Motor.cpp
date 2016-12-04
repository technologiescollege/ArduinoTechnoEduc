//********************************************************************************
//*    Motor.cpp                                            			               *
//*    Version: 1.1                                                              *
//*    This library contains all the necessary functions to control mowayduino   *
//*		 motors.                                    									             *
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

#include "Motor.h"
#include "MowayduinoIo.h"
#include <EEPROM.h>

//**************************************************
//  NAME: beginMotor
//  DESCRIPTION: Reads the values for tuning motors     
//**************************************************
void Motor::beginMotor()
{  
  // Stop motors
	digitalWrite(MOT_R_N,1);
  digitalWrite(MOT_R_P,1);
  digitalWrite(MOT_L_N,1);
  digitalWrite(MOT_L_P,1);
	
  digitalWrite(MOT_R_N,0);
  digitalWrite(MOT_R_P,0);
  digitalWrite(MOT_L_N,0);
  digitalWrite(MOT_L_P,0);	
}

//**************************************************
//  NAME: Straight
//  DESCRIPTION: Activates de PWM for moving motors.
//  Calculates the offset of the wheels speed.
//**************************************************
void Motor::Straight(byte speed)
{   	
	if(speed > SPEED_MAX)
		speed = SPEED_MAX;	
		
	// Activate PWM
  digitalWrite(MOT_L_N,0);
	digitalWrite(MOT_R_N,0);	
  analogWrite(MOT_L_P, speed + EEPROM.read(EE_OFFSET_L));
  analogWrite(MOT_R_P, speed + EEPROM.read(EE_OFFSET_R));	
}

//**************************************************
//  NAME: Straight
//  DESCRIPTION: Robot goes straight forward
//  PARAMETERS: - speed->Motors speed
//              - distance->Distance
//**************************************************
void Motor::Straight(byte speed, int distance)
{
  unsigned long time,timeout;  
  int count;
  byte encoder;	
	
	ReduceInertia();
	
  count = distance/ENCODER_DISTANCE;
  encoder = digitalRead(ENC_R);
  time=millis();
  Straight(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_R))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_R);
    count--;
    time=millis();
  }
  ReduceInertia();			
}

//**************************************************
//  NAME: Back
//  DESCRIPTION: Activates de PWM for moving motors     
//**************************************************
void Motor::Back(byte speed)
{				
	if(speed > SPEED_MAX)
		speed = SPEED_MAX;
		
	// Tested speed equivalence between forward and backward	
	speed = 130 - speed/2;		
	
	// Activate PWM
  digitalWrite(MOT_L_N, 1);
	digitalWrite(MOT_R_N, 1);
  analogWrite(MOT_L_P, speed - EEPROM.read(EE_OFFSET_BACK_L));
  analogWrite(MOT_R_P, speed - EEPROM.read(EE_OFFSET_BACK_R));	
}

//**************************************************
//  NAME: Back
//  DESCRIPTION: Robot goes straight backward
//  PARAMETERS: - speed->Motors speed
//              - distance->Distance
//**************************************************
void Motor::Back(byte speed, int distance)
{
  unsigned long time,timeout;
  int count;
  byte encoder;
	
	ReduceInertia();
	
  count = distance/ENCODER_DISTANCE;
  encoder = digitalRead(ENC_R);
  time=millis();
  Back(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_R))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_R);
    count--;
    time=millis();
  }
  ReduceInertia();		
}

//**************************************************
//  NAME: TurnRight
//  DESCRIPTION: Activates de PWM for moving the left motor    
//  PARAMETER: - speed->Left motor speed
//**************************************************
void Motor::TurnRight(byte speed)
{  	
	if(speed > SPEED_MAX)
		speed = SPEED_MAX;
	  
  digitalWrite(MOT_R_N, 1);
  digitalWrite(MOT_L_N, 0);
	digitalWrite(MOT_R_P, 1);
  analogWrite(MOT_L_P, speed);
}

//**************************************************
//  NAME: TurnRight
//  DESCRIPTION: Robot turns over the right wheel    
//  PARAMETERS:  - speed->Left motor speed
//               - angle->Rotation angle
//**************************************************
void Motor::TurnRight(byte speed, int angle)
{
  unsigned long time,timeout;
  byte count;
  byte encoder;

	ReduceInertia();
	
  count = EncodeAngle(speed, angle);
  encoder = digitalRead(ENC_L);
  time=millis();
  TurnRight(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_L))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_L);
    count--;
    time=millis();
  }
  ReduceInertia();
}

//**************************************************
//  NAME: TurnLeft
//  DESCRIPTION: Activates de PWM for moving the right motor    
//  PARAMETER: - speed->Right motor speed
//**************************************************
void Motor::TurnLeft(byte speed)
{    	
  if(speed > SPEED_MAX)
		speed = SPEED_MAX;
		
  digitalWrite(MOT_R_N,0);
  digitalWrite(MOT_L_N,1);
  analogWrite(MOT_R_P, speed);
	digitalWrite(MOT_L_P, 1);
}

//**************************************************
//  NAME: TurnLeft
//  DESCRIPTION: Robot turns over the left wheel    
//  PARAMETERS:  - speed->Right motor speed
//              - angle->Rotation angle
//**************************************************
void Motor::TurnLeft(byte speed, int angle)
{
  unsigned long time,timeout;
  byte count;
  byte encoder;

	ReduceInertia();
	
  count = EncodeAngle(speed, angle);
  encoder = digitalRead(ENC_R);
  time=millis();
  TurnLeft(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_R))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_R);
    count--;
    time=millis();
  }
  ReduceInertia();
}

//**************************************************
//  NAME: RotateRight
//  DESCRIPTION: Activates de PWM for moving the motors    
//  PARAMETER: - speed->Motors speed
//**************************************************
void Motor::RotateRight(byte speed)
{
	if(speed > SPEED_MAX)
		speed = SPEED_MAX;
	  
  digitalWrite(MOT_R_N,1);
  digitalWrite(MOT_L_N,0);
  analogWrite(MOT_R_P, SPEED_MAX - speed - TURN_CORR);
  analogWrite(MOT_L_P, speed);
}

//**************************************************
//  NAME: RotateRight
//  DESCRIPTION: Robot rotates over the center   
//  PARAMETERS: - speed->Motor speed
//              - angle->Rotation angle
//**************************************************
void Motor::RotateRight(byte speed, int angle)
{
  unsigned long time,timeout;
  byte count;
  byte encoder;

	ReduceInertia();
	
  count = angle/(2*ENCODER_ANGLE);
  encoder = digitalRead(ENC_L);
  time=millis();
  RotateRight(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_L))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_L);
    count--;
    time=millis();
  }
	ReduceInertia();
}

//**************************************************
//  NAME: RotateLeft
//  DESCRIPTION: Activates de PWM for moving the motors    
//  PARAMETER: - speed->Motors speed
//**************************************************
void Motor::RotateLeft(byte speed)
{
	if(speed > SPEED_MAX)
		speed = SPEED_MAX;
	  
  digitalWrite(MOT_R_N, 0);
  digitalWrite(MOT_L_N, 1);
  analogWrite(MOT_R_P, speed);
  analogWrite(MOT_L_P, SPEED_MAX - speed - TURN_CORR);
}

//**************************************************
//  NAME: RotateLeft
//  DESCRIPTION: Robot rotates over the center   
//  PARAMETERS: - speed->Motor speed
//              - angle->Rotation angle
//**************************************************
void Motor::RotateLeft(byte speed, int angle)
{
  unsigned long time,timeout;
  byte count;
  byte encoder;

	ReduceInertia();
	
  count = angle/(2*ENCODER_ANGLE);
  encoder = digitalRead(ENC_R);
  time=millis();
  RotateLeft(speed);
  while (count > 0)
  {
    while (encoder == digitalRead(ENC_R))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)
      {        
        return;
      }
    }
    encoder=digitalRead(ENC_R);
    count--;
    time=millis();
  }
  ReduceInertia();
}

//**************************************************
//  NAME: Move
//  DESCRIPTION: Activates the wheels independently
//  PARAMETERS:  - left_speed->Left motor speed
//               - right_speed->Right motor speed
//**************************************************
void Motor::Move(int left_speed, int right_speed)
{
	offset_l = EEPROM.read(EE_OFFSET_L);
	offset_r = EEPROM.read(EE_OFFSET_R);
	
  if (left_speed >= 0)
  {
    digitalWrite(MOT_L_N,0);
    analogWrite(MOT_L_P,byte(left_speed) + offset_l);
  }
  else if (left_speed < 0)
  {
    digitalWrite(MOT_L_N,1);
    analogWrite(MOT_L_P,SPEED_MAX - byte(-left_speed) - offset_l);
  }
  if (right_speed >= 0)
  {
    digitalWrite(MOT_R_N,0);
    analogWrite(MOT_R_P,byte(right_speed) + offset_r);
  }
  else if (right_speed < 0)
  {
    digitalWrite(MOT_R_N,1);
    analogWrite(MOT_R_P,SPEED_MAX - byte(-right_speed) - offset_r);
  }
}

//**************************************************
//  NAME: Free
//  DESCRIPTION: Moves the motors independently
//  PARAMETERS: * speedR -> Right wheel speed
//							* backR -> Right wheel direction
//              * speedL -> Left wheel speed
//							* backL -> Left wheel direction
//**************************************************
void Motor::Free(byte speedR, boolean backR, byte speedL, boolean backL)
{	
	digitalWrite(MOT_R_N, 1);
	digitalWrite(MOT_R_P, 1);
	digitalWrite(MOT_L_N, 1);
	digitalWrite(MOT_L_P, 1);
	
	if(backR == true)
	{
		digitalWrite(MOT_R_N, 1);
		analogWrite(MOT_R_P, (180 - speedR + (speedR - 100)/2));
	}
  else
	{
		digitalWrite(MOT_R_N, 0);
		analogWrite(MOT_R_P, speedR);
	}

	if(backL == true)
	{
		digitalWrite(MOT_L_N, 1);
		analogWrite(MOT_L_P, (180 - speedL + (speedL - 100)/2));
	}
  else
	{
		digitalWrite(MOT_L_N, 0);
		analogWrite(MOT_L_P, speedL);
	}
}

//**************************************************
//  NAME: Free
//  DESCRIPTION: Moves the motors independently
//  PARAMETERS: * speedR -> Right wheel speed
//							* backR -> Right wheel direction
//              * speedL -> Left wheel speed
//							* backL -> Left wheel direction
//**************************************************
void Motor::Free(byte speedR, boolean backR, byte speedL, boolean backL, int distance)
{
  unsigned long time,timeout;  
  int count;
  byte encoder;
	byte side;
	
	ReduceInertia();
	
	if(speedR >= speedL)
		side = ENC_R;
	else
		side = ENC_L;

  count = distance/ENCODER_DISTANCE;
  encoder = digitalRead(side);
  time = millis();
  Free(speedR, backR, speedL, backL);
  while (count > 0)
  {
    while (encoder == digitalRead(side))
    {
      timeout=millis();
      if (timeout - time > MAX_TIMEOUT)      
        return;      
    }
    encoder = digitalRead(side);
    count--;
    time = millis();
  }
  ReduceInertia();		
}

//**************************************************
//  NAME: Stop
//  DESCRIPTION: Deactivates PWM
//**************************************************
void Motor::Stop()
{
  digitalWrite(MOT_R_N,1);
  digitalWrite(MOT_R_P,1);
  digitalWrite(MOT_L_N,1);
  digitalWrite(MOT_L_P,1);
}

//****************************************************************
//
//  PRIVATE FUNCTIONS
//
//****************************************************************
//**************************************************
//  NAME: EncodeAngle
//  DESCRIPTION: Converts degrees to encoder steps
//**************************************************
byte Motor::EncodeAngle(byte speed, int angle)
{
  byte ret;
  
  if(angle > 5.67)  
    ret = (0.596*angle) - 5.67 + (120.0/speed);  
  else
    ret = 0;
    
  return ret;
}

//**************************************************
//  NAME: MotorTune
//  DESCRIPTION:	Calculate motors speed difference
//								and saves the offset in EEPROM
//**************************************************
void Motor::MotorTune(byte speed)
{	
	int difference;
	int offsetR, offsetL;		
	int offsetBackR, offsetBackL;
	boolean leftFaster, leftFasterBack;
	boolean readyForward, readyBack;
	int speedBack;
	
	// Tested speed equivalence between forward and backward	
	speedBack = 130 - speed/2;
	
	// Stop motors
  digitalWrite(MOT_R_N,0);
  digitalWrite(MOT_R_P,0);
  digitalWrite(MOT_L_N,0);
  digitalWrite(MOT_L_P,0);
	Stop();
	
	if(speed < SPEED_MIN)
		return;
	
	// Initailize values	
	offsetL = 0;
	offsetR = 0;
	offsetBackL = 0;
	offsetBackR = 0;
	readyForward = false;
	readyBack = false;
	
	//**************************
	// Check the fastest motor (FWD)
	//**************************
	digitalWrite(MOT_R_N,0);
	digitalWrite(MOT_L_N,0);
	analogWrite(MOT_R_P, speed);			
	analogWrite(MOT_L_P, speed);
	
	// Discard first counts (inertia)
	delay(200);

	difference = EncoderDifference(200);
	ReduceInertia();
	delay(200);
	
	if(difference > 0)		
		leftFaster = false;
	else if(difference < 0)		
		leftFaster = true;
	else
	{	
		EEPROM.write(EE_OFFSET_L, 0);
		EEPROM.write(EE_OFFSET_R, 0);
		readyForward = true;
	}
	
	//**************************
	// Check the fastest motor (BCK)
	//**************************
	digitalWrite(MOT_R_N, 1);
	digitalWrite(MOT_L_N, 1);
	analogWrite(MOT_R_P, speedBack);			
	analogWrite(MOT_L_P, speedBack);
	
	// Discard first counts (inertia)
	delay(200);

	difference = EncoderDifference(200);	
	ReduceInertia();
	delay(200);
	
	if(difference > 0)	
		leftFasterBack = false;		
	else if(difference < 0)
		leftFasterBack = true;
		
	else
	{	
		EEPROM.write(EE_OFFSET_BACK_L, 0);
		EEPROM.write(EE_OFFSET_BACK_R, 0);
		readyBack = true;
	}
		
	while(!readyForward || !readyBack)
	{
		//*****************************
		// Moving forward
		//*****************************
		digitalWrite(MOT_R_N,0);
		digitalWrite(MOT_L_N,0);
		analogWrite(MOT_R_P, speed + offsetR);			
		analogWrite(MOT_L_P, speed + offsetL);
		
		// Discard first counts (inertia)
		delay(200);
		
		difference = EncoderDifference(200);
		ReduceInertia();
		delay(200);
		
		// Change left or right offset
		if(!readyForward)
		{
			if(leftFaster)
			{
				if(difference > 6)
					offsetR = offsetR - 5;
				else if(difference < -6)		
					offsetR = offsetR + 5;
				else if(difference > 4)		
					offsetR = offsetR - 3;
				else if(difference < -4)		
					offsetR = offsetR + 3;		
				else if(difference > 2)		
					offsetR--;
				else if(difference < -2)		
					offsetR++;		
				else			
					readyForward = true;
			}
			else
			{
				if(difference > 6)		
					offsetL = offsetL + 5;
				else if(difference < -6)		
					offsetL = offsetL - 5;		
				else if(difference > 4)		
					offsetL = offsetL + 3;
				else if(difference < -4)		
					offsetL = offsetL - 3;		
				else if(difference > 2)		
					offsetL++;
				else if(difference < -2)		
					offsetL--;
				else			
					readyForward = true;
			}
		}
				
		//*****************************
		// Moving backward
		//*****************************
		digitalWrite(MOT_R_N, 1);
		digitalWrite(MOT_L_N, 1);
		analogWrite(MOT_R_P, speedBack - offsetBackR);			
		analogWrite(MOT_L_P, speedBack - offsetBackL);
		
		// Discard first counts (inertia)
		delay(200);	
		
		difference = EncoderDifference(200);
		ReduceInertia();
		delay(200);

		// Change left or right offset
		if(!readyBack)
		{
			if(leftFasterBack)
			{
				if(difference > 4)		
					offsetBackR = offsetBackR - 3;
				else if(difference < -4)		
					offsetBackR = offsetBackR + 3;		
				else if(difference > 2)		
					offsetBackR--;
				else if(difference < -2)		
					offsetBackR++;	
				else
					readyBack = true;
			}
			else
			{
				if(difference > 4)		
					offsetBackL = offsetBackL + 3;
				else if(difference < -4)		
					offsetBackL = offsetBackL - 3;		
				else if(difference > 2)		
					offsetBackL++;
				else if(difference < -2)		
					offsetBackL--;
				else				
					readyBack = true;									
			}
		}		
	}
	
	// Save speed offset in EEPROM	
	EEPROM.write(EE_OFFSET_R, offsetR);
	EEPROM.write(EE_OFFSET_L, offsetL);	
	EEPROM.write(EE_OFFSET_BACK_R, offsetBackR);
	EEPROM.write(EE_OFFSET_BACK_L, offsetBackL);	
}


//**************************************************
//  NAME: EncoderDifference
//  DESCRIPTION: Counts encoder pulses on both wheels
//								and returns their difference
//**************************************************
int Motor::EncoderDifference(int counts)
{	
	int countR = 0;
	int countL = 0;
	byte encoderR, encoderL;
	
	while(countR < counts)
	{
		if(encoderR != digitalRead(ENC_R))
		{
			encoderR = digitalRead(ENC_R);
			countR++;
		}
		if(encoderL != digitalRead(ENC_L))
		{
			encoderL = digitalRead(ENC_L);
			countL++;
		}		
	}	
	return (countR - countL);	
}


//**************************************************
//  NAME: ReduceInertia
//  DESCRIPTION: Reduce inertia if previously moving
//**************************************************
void Motor::ReduceInertia()
{
	Stop();
	delay(100);
}
