//*********************************************************************
//
//  PROGRAM:      TuneMotors
//  DESCRIPTION:  mOwayduino autotunes its motors. It goes straight
//                50cm and compares the difference between encoders.
//                It repeats until the calibration is OK.
//
//*********************************************************************

//****************************************
//  Libraries
//****************************************
#include "MowayduinoRobot.h"
#include <SPI.h>
#include <EEPROM.h>

//****************************************
//  Objects
//****************************************
mowayduinorobot robot;

//****************************************
//  RF event function (necessary for compiling)
//****************************************
void RfDataReady();
void RfDataReady()
{}


//****************************************
//
//  Main Program
//
//****************************************
void setup()
{
  robot.beginMowayduino();
}

void loop()
{
  // Wait 5 seconds to place the robot over a flat surface
  for(int i=0; i<5 ;i++)
  {
    robot.Redon();
    Beep();
    delay(300);
    robot.Redoff();
    delay(500);
  }  
  
  // The robot starts the calibration for a speed of 100
  robot.MotorTune(100);
  
  // Calibration ends
  robot.Greenon();
  Beep();
  delay(1000);
  robot.Greenoff();
  
  // Calibration test. Make a square
  for(int i=0; i< 4; i++)
  {
    robot.Straight(100);
    delay(1000);
    robot.TurnRight(100, 90);
  }  
  
  while(1);
}

//****************************************
//
//  Functions
//
//****************************************
void Beep()
{
  robot.Speakeron();
  delay(200);
  robot.Speakeroff();
}

