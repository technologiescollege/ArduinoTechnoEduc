//********************************************************
//             Mini robot Joystic
//********************************************************

#include <Servo.h>
#include <ServoPlus360.h>

ServoPlus360 monServoPlus2;
ServoPlus360 monServoPlus3;

void setup()
{
  
  monServoPlus2.brancher(2);
  monServoPlus2.inverserRotation(OUI);
  monServoPlus3.brancher(3);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

}

void loop(){  
  monServoPlus2.ecrireRotation(analogRead(A0)-(analogRead(A1)-500));
  monServoPlus3.ecrireRotation(analogRead(A0)+(analogRead(A1)-500));
}


