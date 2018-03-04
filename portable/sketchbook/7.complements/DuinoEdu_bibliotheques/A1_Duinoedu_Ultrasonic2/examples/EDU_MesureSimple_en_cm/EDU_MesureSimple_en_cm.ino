// CAPTEUR DE DISTANCE (2 modèles possibles)
// GROVE  : http://duinoedu.com/store1/05-modules-evolues/68-capteur-de-distance-ultrasons.html
// DUPONT : http://duinoedu.com/store1/05-modules-evolues/85-capteur-de-distance-par-ultrasons-dupont.html

#include <Ultrasonic.h>       // http://duinoedu.com/dl/lib/grove/EDU_Ultrasonic_GroveDupont.zip
Ultrasonic monUltrasonic(4);  // 4 ou autre broche ou (4,5) si Dupont
void setup(){
  Serial.begin(9600);
}
void loop(){
  long mesure = monUltrasonic.mesurer();              
  Serial.println(mesure);
  delay(500);
}
