/*PAP1 card test : Two Stepper Motor controller in ABSOLUTE mode
/* Author : Grosse Christophe 
/* Révision : 1.0
/* Date : 06/08/14
  
 The circuit: 
 
 * Connect a 100k ohm potentiometer to A0 ( to control MotorA absolute position )

 Note:
 
 This example code is in the public domain.
 
 */
#include <Wire.h>
#include <Pap1.h>

//create the instance of the pap controler at the I2C address 16
Pap1 MyStepper(16);

int valA;
int old_valA;

void setup()  
{
   Wire.begin(); // join i2c bus before calling the stepper constructor
  //Choose the stepper mode below
  //SINGLE_STEPPING : low torque, low current, low heat, high speed, 2048 step/rev
  //FULL_STEPPING   : high torque, high current, high heat, high speed , 2048 step/rev
  //HALF_STEPPING   : medium torque, medium current, medium heat, low speed, 4096 step/rev
  //AUTO_RELAX      : When motor halted, no maintain current, not maintained in position
  //NO_AUTO_RELAX   : when motor halted, maintain curent, maintained in position
  
  MyStepper.Begin(SINGLE_STEPPING,AUTO_RELAX);
  //MyStepper.Begin(FULL_STEPPING,AUTO_RELAX);
  //MyStepper.Begin(HALF_STEPPING,AUTO_RELAX);
  //MyStepper.Begin(SINGLE_STEPPING,NO_AUTO_RELAX);
  //MyStepper.Begin(FULL_STEPPING,NO_AUTO_RELAX);
  //MyStepper.Begin(HALF_STEPPING,NO_AUTO_RELAX);
  
  MyStepper.SetMode(IDLE); //Motor stopped by default
  //MyStepper.DirA(CW);      //Motor A set Clockwise
  //MyStepper.DirB(CCW);     //Motor B set Counter Clockwise
  MyStepper.SpeedA(255);     //Motor A speed to 255
  MyStepper.SpeedB(0);     //Motor B speed to 0
  MyStepper.OrigineA();  //Set Actual position of Motor A as his origine 
  MyStepper.OrigineB();  //Set Actual position of Motor B as his origine
  MyStepper.TargetB(0); //Motor B will not move because we target to 0 
}

void loop() // run over and over
{
  valA=analogRead(A0); //Read Position on the potentiometer
  if (abs(old_valA-valA)>5) { //If the position change
    old_valA=valA;
    MyStepper.SetMode(IDLE);
    delay(1);
    MyStepper.TargetA(valA);  //Fix the new position of the motor A
    delay(1);
    MyStepper.SetMode(ABSOLUTE);   //Enter in Absolute mode, Motor run as previously set   
    delay(10); //wait a little before pooling
    while (MyStepper.Busy()) {
      //loop until mouvement are done on the two motors
      delay(10); //Pool not too fast
    }  
  }  
}

