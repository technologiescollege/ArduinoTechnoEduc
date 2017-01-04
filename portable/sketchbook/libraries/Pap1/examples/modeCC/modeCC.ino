/*PAP1 card test : Two Stepper Motor controller in CC mode
/* Author : Grosse Christophe 
/* Révision : 1.0
/* Date : 06/08/14
  
 The circuit: 
 
 * Connect a 100k ohm potentiometer to A0 ( to control MotorA speed and dir )
 * Connect a 100k ohm potentiometer to A1 ( to control MotorB speed and dir ) 
 
 Note:
 
 This example code is in the public domain.
 
 */
#include <Wire.h>
#include <Pap1.h>

//Set the controler instance to the I2C Address 16 
Pap1 MyStepper(16);

int valA;
int valB;
int old_valA;
int old_valB;

void setup()  
{
  Wire.begin(); // join i2c bus before calling the stepper constructor
  //Choose the stepper mode below
  //SINGLE_STEPPING : low torque, low current, low heat, high speed, 2048 step/rev
  //FULL_STEPPING   : high torque, high current, high heat, high speed , 2048 step/rev
  //HALF_STEPPING   : medium torque, medium current, medium heat, low speed, 4096 step/rev
  //AUTO_RELAX      : When motor halted, no maintain current, not maintained in position
  //NO_AUTO_RELAX   : when motor halted, maintain curent, maintained in position
  
  //MyStepper.Begin(SINGLE_STEPPING,AUTO_RELAX);
  MyStepper.Begin(FULL_STEPPING,AUTO_RELAX);
  //MyStepper.Begin(HALF_STEPPING,AUTO_RELAX);
  //MyStepper.Begin(SINGLE_STEPPING,NO_AUTO_RELAX);
  //MyStepper.Begin(FULL_STEPPING,NO_AUTO_RELAX);
  //MyStepper.Begin(HALF_STEPPING,NO_AUTO_RELAX);
  MyStepper.SetMode(IDLE); //Motor stopped by default
  MyStepper.DirA(CW);      //Motor A set Clockwise
  MyStepper.DirB(CCW);     //Motor B set Counter Clockwise
  MyStepper.SpeedA(0);     //Motor A speed to 0
  MyStepper.SpeedB(0);     //Motor B speed to 0
  MyStepper.SetMode(CC);   //Enter in Continious Current Mode 
}

void loop() // run over and over
{
  valA=1023-analogRead(A0); //Read Motor A speed and dir
  valB=1023-analogRead(A1); //Read Motor B speed and dir
  if (old_valA!=valA) {
    old_valA=valA;
    if (valA<512) {
      MyStepper.DirA(CCW);
      MyStepper.SpeedA((511-valA)/2);
    } else {
      MyStepper.DirA(CW);
      MyStepper.SpeedA((valA-512)/2);
    }
  }
  if (old_valB!=valB) {
    old_valB=valB;
    if (valB<512) {
      MyStepper.DirB(CCW);
      MyStepper.SpeedB((511-valB)/2);
    } else {
      MyStepper.DirB(CW);
      MyStepper.SpeedB((valB-512)/2);
    }
  }
  delay(100);
}

