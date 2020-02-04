/*
*			DropIn example sketch!
*
*	Pin connections:
*	-----------------------------
*	| Controller | uStepperS Lite|
*	|----------------------------|
*	|	Enable   |		D2		 |
*	|	Step     |		D3		 |
*	|	Dir      |		D4		 |
*	|	GND      |		GND		 |
*	------------------------------	
*/

#include <uStepperSLite.h>
uStepperSLite stepper;

void setup(void)
{
	stepper.setup(DROPIN,3200.0,50,1,0.0,true);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.dropinCli();
}