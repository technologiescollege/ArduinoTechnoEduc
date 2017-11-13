#include <DHT.h>                // http://duinoedu.com/dl/lib/grove/EDU_DHT_Grove.zip
DHT monDHTPRO(8);               // Brancher sur D8 ou D8/D9   

void setup(){
  monDHTPRO.brancher();
  Serial.begin(9600);
}

void loop(){
  float maTemp = monDHTPRO.lireTemperature();
  Serial.print(maTemp); Serial.println(" degres");
  
  float monHum = monDHTPRO.lireHumidite();
  Serial.print(monHum); Serial.println(" %");
}
