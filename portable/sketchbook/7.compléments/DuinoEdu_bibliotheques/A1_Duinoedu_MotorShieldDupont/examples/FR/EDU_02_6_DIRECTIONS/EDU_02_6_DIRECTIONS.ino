#include <MotorShieldDupont.h>

MotorShieldDupont monMotorShield;

void setup(){
  monMotorShield.brancher();
  monMotorShield.ecrireVitesse(1000,1000);    // Moteur droit, gauche
  monMotorShield.ecrireDerive(500);           // 500 = neutre
  
  
}

void loop(){
  monMotorShield.avancer("tout_droit");
  delay(5000);
  monMotorShield.avancer("droite");
  delay(5000);
  monMotorShield.avancer("gauche");
  delay(5000);
  monMotorShield.reculer("tout_droit");
  delay(5000);
  monMotorShield.reculer("droite");
  delay(5000);
  monMotorShield.reculer("gauche");
  delay(5000);
  
}
