#include <uStepperSLite.h>

uStepperSLite stepper;

void setup() {
  // put your setup code here, to run once:
  stepper.setup();
  stepper.setMaxAcceleration(2000);
  stepper.setMaxVelocity(1500);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!stepper.getMotorState())
  {
   
    stepper.moveSteps(3200, !stepper.getCurrentDirection(), HARD);
  }
   Serial.print("Angle: ");
   Serial.print(stepper.encoder.getAngle());
   Serial.println(" Degrees");
}
