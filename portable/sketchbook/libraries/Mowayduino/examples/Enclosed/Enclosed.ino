//*********************************************************************
//
//  PROGRAM:      Enclosed
//  DESCRIPTION:  mOwayduino robot moves into a black circle
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
  Serial.begin(57600);
  delay(2000);
}

void loop()
{
  LedsStraight();  
  robot.Straight(100);
  robot.readLine();
  if(robot.Line[LINE_L] == 1 || robot.Line[LINE_R] == 1)
  {
    LedsRotate();
    
    // Rotate 140 degrees with a speed value of 100
    robot.RotateRight(100, 140);
    delay(100);
  }
}

//****************************************
//
//  Functions
//
//****************************************
void LedsStraight()
{
  robot.Fronton();
  robot.Greenon();  
  robot.Brakeoff();
  robot.Redoff();  
}

void LedsRotate()
{
  robot.Frontoff();
  robot.Greenoff();  
  robot.Brakeon();
  robot.Redon();  
}

