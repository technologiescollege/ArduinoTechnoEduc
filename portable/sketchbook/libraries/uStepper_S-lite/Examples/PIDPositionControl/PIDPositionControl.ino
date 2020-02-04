/*
*      Example sketch for PID position control!
*
* This example demonstrates how easy closed loop position control can be achieved using the uStepper !
* The only thing needed to activate closed loop control, is in the stepper.setup() function, where the
* object is initiated with the keyword "PID", followed by the number of steps per revolution setting, and the P, I and D coefficients.
* For more information, check out the documentation:
* http://ustepper.com/docs/ustepperslite/html/index.html
*
* Once the PID is activated, the use of the library is the same as without the PID !
*
*/

#include <uStepperSLite.h>

uStepperSLite stepper;

void setup(void)
{
  Serial.begin(9600);
  stepper.setup(PID,3200.0,20,0.5,0.0,true);   //Initiate the stepper object to use closed loop PID control
                                                        //The behaviour of the controller can be adjusted by tuning 
                                                        //the P, I and D paramenters in this initiation (the three two parameters).
                                                        //check out the documentation:
                                                        //http://ustepper.com/docs/html/index.html

  stepper.moveSteps(3200,CCW,HARD);                 //turn shaft 3200 steps, counterClockWise (equal to one revolution)
}

void loop(void)
{
  Serial.println(stepper.encoder.getAngleMoved());    //print out the current angle of the motor shaft.
}