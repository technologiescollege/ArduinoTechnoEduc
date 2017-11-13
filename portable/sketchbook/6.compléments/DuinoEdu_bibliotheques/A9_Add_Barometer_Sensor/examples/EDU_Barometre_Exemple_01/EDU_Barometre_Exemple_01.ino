/* Barometer demo V1.0
* Based largely on code by  Jim Lindblom
* Get pressure, altitude, and temperature from the BMP085.
* Serial.print it out at 9600 baud to serial monitor.
*
* By:http://www.seeedstudio.com
*
* Augmenté par http://duinoedu.com
* Librairie : http://duinoedu.com/dl/lib/grove/EDU_BarometerSensor_Grove/
*
*/

#include "Barometer.h"
#include <Wire.h>

float temperature;
float pression;
float altitude;

Barometer monBarometre;
void setup(){
  Serial.begin(9600);
  monBarometre.brancher();
}

void loop(){
   temperature  = monBarometre.lireTemperature();
   pression     = monBarometre.lirePression();
   altitude     = monBarometre.lireAltitude(); 
}



