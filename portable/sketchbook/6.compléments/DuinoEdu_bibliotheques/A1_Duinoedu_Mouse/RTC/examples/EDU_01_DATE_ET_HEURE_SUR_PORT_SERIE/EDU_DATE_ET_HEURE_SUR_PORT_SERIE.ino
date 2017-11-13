// Exemple de programme inspiré de l'exemple Seeed Grove
// Par david Souder pour DuinoEDU.com - le 18/02/2015

#include <Wire.h>
#include "DS1307.h"

DS1307 clock;

void setup(){
	Serial.begin(9600);
	clock.begin();
	//clock.ecrireDate(18,2,2015); 	// Normalement déjà fait
	//clock.ecrireHeure(18,15,0);	// Normalement déjà fait
}

void loop(){
	//--- Date
    Serial.print(clock.lireJour(), DEC);
    Serial.print('/');
    Serial.print(clock.lireMois(), DEC);
    Serial.print('/');
    Serial.print(clock.lireAnnee(), DEC);
    Serial.print("\t");
    
	/--- Heure
    Serial.print(clock.lireHeures(), DEC);
    Serial.print(':');
    Serial.print(clock.lireMinutes(), DEC);
    Serial.print('.');
    Serial.print(clock.lireSecondes(), DEC);
    Serial.println();
}

