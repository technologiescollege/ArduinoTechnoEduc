#include <Servo.h>
#include <ServoPlus360.h>


ServoPlus360 monServoPlus2;

void setup()
{
  monServoPlus2.brancher(2);
  pinMode(A0, INPUT);
  pinMode(4, INPUT);

}

void loop()
{
  if(digitalRead(4)==true)   monServoPlus2.inverserRotation(OUI);
  else                       monServoPlus2.inverserRotation(NON);
  
  monServoPlus2.ecrireVitesse(analogRead(A0));
}


