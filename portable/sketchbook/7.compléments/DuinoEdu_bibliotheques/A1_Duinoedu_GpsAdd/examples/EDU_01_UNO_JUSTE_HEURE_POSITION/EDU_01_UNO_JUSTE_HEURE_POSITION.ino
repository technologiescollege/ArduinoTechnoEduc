//==============================================================
// Lib : duinoeduGpsAdd par David Souder pour www.duinoedu.com
// Adafruit_GPS par Adafruit
// SoftwareSerial par Arduino
// Version du 28/07/2015
//==============================================================

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <duinoeduGpsAdd.h>

duinoeduGpsAdd monGps;

void setup(){
  Serial.begin(9600);                             // Passer à 115200 ici et dans le moniteur série si trop lent
  monGps.brancher(4,5);                           // Autres couples possibles  
  monGps.definirDecalageHoraire(2);               // Car heure universelle  
}

EDUGPS_INTERRUPTION(HORLOGE_0_DEPASSE_VALEUR_A) {
  monGps.lireUnCaractere();
}

void loop(){
  Serial.println(monGps.heure());
  Serial.println(monGps.positionGoogleMaps());
}
