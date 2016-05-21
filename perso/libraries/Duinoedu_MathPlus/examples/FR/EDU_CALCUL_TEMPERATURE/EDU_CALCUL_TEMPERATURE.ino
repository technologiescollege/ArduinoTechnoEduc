// Exemple : permet de convertir une valeur lue sur A0 en degrés
// Pour Grove Temperature sensor
// Par David Souder pour DuinoEDU

#include <Duinoedu_MathPlus.h>

void setup(){
  Serial.begin(9600);
}

void loop(){
  
  float temperature = convertirEnDegres(analogRead(A0));
  Serial.println(temperature,2);
  
  convertirEnDegres(12);
}


