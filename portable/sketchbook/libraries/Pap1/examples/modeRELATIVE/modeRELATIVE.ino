/*PAP1 card test : Two Stepper Motor controller in RELATIVE mode
/* Author : Grosse Christophe 
/* Révision : 1.0
/* Date : 06/08/14
  
 The circuit: 
 
* Connect a button to digital pin 7
 
 Note:
 
 This example code is in the public domain.
 
 */
#include <Wire.h>
#include <Pap1.h>

//Set the controler instance to the I2C address 16
Pap1 MyStepper(16);

void setup()  
{
  pinMode(7, INPUT);
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
  MyStepper.SpeedA(255);   //Motor A speed to 255
  MyStepper.SpeedB(245);   //Motor B speed to 245
  MyStepper.StepA(4096);   //Two révolution for Motor A
  MyStepper.StepB(2048);   //One révolution for Motor B
}

void loop() // run over and over
{
  if (!digitalRead(7)) {
    //If button on pin 7 is pressed then 
    MyStepper.SetMode(RELATIVE);   //Enter in Relative mode, Motor run as previously set
    delay(10); //Wait a little before to pool the controler status
    while (MyStepper.Busy()) {
      //loop until mouvement are done on the two motors
      delay(10); //Pool not too fast
    }
  //resetting the next mouvement  
  MyStepper.StepA(2048);   //One révolution for Motor A
  MyStepper.StepB(2048);   //One révolution for Motor B  
  //delay(10);
  }
}

