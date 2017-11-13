/* Barometer demo V1.0
* Based largely on code by  Jim Lindblom
* Get pressure, altitude, and temperature from the BMP085.
* Serial.print it out at 9600 baud to serial monitor.
*
* By:http://www.seeedstudio.com
*/
#include "Barometer.h"
#include <Wire.h>
float temperature;
float pressure;
float atm;
float altitude;
Barometer myBarometer;
void setup(){
  Serial.begin(9600);
  myBarometer.init();
  
}

void loop()
{
   temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
   pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
   altitude = myBarometer.calcAltitude(pressure); //Uncompensated caculation - in Meters 
   atm = pressure / 101325; 
  
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C");

  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  Serial.print("Ralated Atmosphere: ");
  Serial.println(atm, 4); //display 4 decimal places

  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" m");

  Serial.println();

  delay(1000); //wait a second and get values again.
}



