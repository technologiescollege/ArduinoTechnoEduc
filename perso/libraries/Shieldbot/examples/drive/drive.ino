#include <Shieldbot.h>

Shieldbot shieldbot = Shieldbot();

void setup(){
  Serial.begin(9600);
  shieldbot.setMaxSpeed(255);//255 is max
}

void loop(){
  //leftSquare();  
  //leftCircle();
  infinity();
}

void leftSquare(){
  shieldbot.drive(127,127); //straight forward
  delay(750);
  shieldbot.drive(-128,127);   //turn left on a dime
  delay(325);  //to turn 90, it depends on surface, ~450 on hard floors, ~325 on carpet
}

void infinity(){
  leftCircle();
  delay(4800);
  rightCircle();
  delay(4800);
}

void leftCircle(){
  shieldbot.drive(25,127);
}

void rightCircle(){
  shieldbot.drive(127,25);
}