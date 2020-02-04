#include <uStepperSLite.h>

#define MICROSTEP 16//microstep setting of uStepper
#define RES (200.0*MICROSTEP)/360.0//calculate step pr. degree
#define STEPPRMM 53.55//step pr. mm for the rail used in the demo
#define MMPRSTEP 1/STEPPRMM//mm pr. step
#define MMPRDEG MMPRSTEP*RES//mm pr. degree
#define STALLSENSITIVITY 0.9//sensitivity of the stall detection, between 0.0 and 1.0 - high number is less sensitive

uStepperSLite stepper;

void setup() {
  // put your setup code here, to run once:
  stepper.setup();
  stepper.setCurrent(50.0);//use software current setting
  stepper.setMaxAcceleration(20000);
  stepper.setMaxVelocity(4000);
  Serial.begin(9600);
}

void loop() {
  float railLength;

  stepper.moveToEnd(CW, STALLSENSITIVITY);      //Reset to CW endpoint
  Serial.println(railLength*MMPRDEG);//find end positions and read out the recorded end position
  railLength = stepper.moveToEnd(CCW, STALLSENSITIVITY);    //Go to CCW end
  Serial.println(railLength*MMPRDEG);//find end positions and read out the recorded end position
  
  while(1);
}