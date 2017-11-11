#include <Ultrasonic.h>

Ultrasonic ultrasonic(7);
void setup()
{
  
  Serial.begin(9600);
}
void loop()
{
  ultrasonic.MeasureInCentimeters();
  Serial.println(ultrasonic.RangeInCentimeters);
  delay(100);
}
