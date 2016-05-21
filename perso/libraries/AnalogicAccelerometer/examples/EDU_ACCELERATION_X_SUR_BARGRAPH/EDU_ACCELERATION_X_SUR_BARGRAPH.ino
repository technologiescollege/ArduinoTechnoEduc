#include <AnalogicAccelerometer.h>
#include <LED_Bar.h>


AnalogicAccelerometer monAccelerometre;
LED_Bar mesLeds_pin23(3,2);

void setup(){
  Serial.begin(9600);
  monAccelerometre.branch(A0);
}

void loop(){
  mesLeds_pin23.displayVoltage(monAccelerometre.read('X'));
  Serial.println(monAccelerometre.read('X'));
}

