#include <IRremote.h>
#include <Servo.h>
#include <ServoPlus.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_IRremote_GroveDupont/
IRrecv monRecepteur_pin2(2);
//libraries at http://duinoedu.com/dl/lib/grove/EDU_ServoPlus_GroveDupont/ 
ServoPlus monServoPlus4;

void setup()
{
  Serial.begin(9600);
  monRecepteur_pin2.enableIRIn(); 
  pinMode( 8 , OUTPUT);
  monServoPlus4.brancher(4);

}

void loop()
{
  if (monRecepteur_pin2.testerTouche("CH-")){monServoPlus4.ecrireAngle(15,1000);}
  if (monRecepteur_pin2.testerTouche("CH")){monServoPlus4.ecrireAngle(20,1000);}
  if (monRecepteur_pin2.testerTouche("CH+")){monServoPlus4.ecrireAngle(25,1000);}
  if (monRecepteur_pin2.testerTouche("PREV")){monServoPlus4.ecrireAngle(30,1000);}
  if (monRecepteur_pin2.testerTouche("NEXT")){monServoPlus4.ecrireAngle(35,1000);}
  if (monRecepteur_pin2.testerTouche("PLAY/PAUSE")){monServoPlus4.ecrireAngle(40,1000);}
  if (monRecepteur_pin2.testerTouche("VOL-")){monServoPlus4.ecrireAngle(45,1000);}
  if (monRecepteur_pin2.testerTouche("VOL+")){monServoPlus4.ecrireAngle(50,1000);}
  if (monRecepteur_pin2.testerTouche("EQ")){monServoPlus4.ecrireAngle(55,1000);}
  if (monRecepteur_pin2.testerTouche("0")){monServoPlus4.ecrireAngle(60,1000);}
  if (monRecepteur_pin2.testerTouche("FL+")){monServoPlus4.ecrireAngle(65,1000);}
  if (monRecepteur_pin2.testerTouche("FL-")){monServoPlus4.ecrireAngle(70,1000);}
  if (monRecepteur_pin2.testerTouche("1")){monServoPlus4.ecrireAngle(75,1000);}
  if (monRecepteur_pin2.testerTouche("2")){monServoPlus4.ecrireAngle(80,1000);}
  if (monRecepteur_pin2.testerTouche("3")){monServoPlus4.ecrireAngle(85,1000);}
  if (monRecepteur_pin2.testerTouche("4")){monServoPlus4.ecrireAngle(90,1000);}
  if (monRecepteur_pin2.testerTouche("5")){monServoPlus4.ecrireAngle(95,1000);}
  if (monRecepteur_pin2.testerTouche("6")){monServoPlus4.ecrireAngle(100,1000);}
  if (monRecepteur_pin2.testerTouche("7")){monServoPlus4.ecrireAngle(105,1000);}
  if (monRecepteur_pin2.testerTouche("8")){monServoPlus4.ecrireAngle(110,1000);}
  if (monRecepteur_pin2.testerTouche("9")){monServoPlus4.ecrireAngle(115,1000);}
}


