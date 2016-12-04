//*********************************************************************
//
//  PROGRAM:      Colours
//  DESCRIPTION:  mOwayduino robot turns on its RGB led when it detects
//                a loud sound
//
//*********************************************************************

//****************************************
//  Libraries
//****************************************
#include "MowayduinoRobot.h"
#include <SPI.h>
#include <EEPROM.h>

//****************************************
//  Definitions
//****************************************
#define LOUD_SOUND  500

//****************************************
//  Objects
//****************************************
mowayduinorobot robot;

//****************************************
//  Variables
//****************************************
int cnt = 0;

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
  if(robot.readMic() > LOUD_SOUND)
  {
    if(cnt == 0)
    {
      robot.Ledsoff();
      robot.Redon();
      Beep();
      cnt++;
    }
    else if(cnt == 1)
    {
      robot.Ledsoff();
      robot.Greenon();
      Beep();
      cnt++;
    }
    else
    {
      robot.Ledsoff();
      robot.Blueon();
      Beep();      
      cnt = 0;
    }
  }
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

