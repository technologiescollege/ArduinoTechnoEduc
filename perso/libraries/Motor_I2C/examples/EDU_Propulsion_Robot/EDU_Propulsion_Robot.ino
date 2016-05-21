// Motor drive I2C Grove http://duinoedu.com/store1/05-modules-evolues/197-module-i2c-2-moteurs-ou-1-moteur-pas-a-pas.html

#include <Wire.h>         // Déjà dans Arduino
#include <MotorI2C.h>     // http://www.duinoedu.com/dl/lib/grove/EDU_Motor_I2C.zip

MotorI2C mesMoteurs;

void setup(){
  Serial.begin(9600);
  mesMoteurs.branch();
  mesMoteurs.reglerVitesse(10,10);
}

void loop(){
  mesMoteurs.avancer(); delay(1000);
  mesMoteurs.avancer("droite"); delay(1000);
  mesMoteurs.avancer("gauche"); delay(1000);
  mesMoteurs.stopper(); delay(1000);

  mesMoteurs.reculer(); delay(1000);
  mesMoteurs.reculer("droite"); delay(1000);
  mesMoteurs.reculer("gauche"); delay(1000);
  
  mesMoteurs.pivoter("gauche"); delay(1000);
  mesMoteurs.pivoter("droite"); delay(1000);
}


