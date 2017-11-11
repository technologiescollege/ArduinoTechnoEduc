#include <Servo.h>
#include <ServoPlus.h>

ServoPlus monServoPlus;

void setup()
{
  monServoPlus.brancher(5);

}

void loop()
{
  monServoPlus.ecrireAngle(45,500);
  delay(2000);
  monServoPlus.ecrireAngle(135,500);
  delay(2000);
}


