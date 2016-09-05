#include <RFID125.h>
#include <SoftwareSerial.h>

RFID125 monRFID;
String monCode;
#define monInterr 2

void setup(){
  Serial.begin(9600);
  monRFID.brancher(4,5);
  monRFID.RAMliste="10267514 07388281 ";
}

void loop(){
  monCode = monRFID.lireCode();          
  if(digitalRead(monInterr)==HIGH){ monRFID.ecrireCode(monCode); }
  if(digitalRead(monInterr)==LOW ){ monRFID.testerCode(monCode); }                         
}
  
  

