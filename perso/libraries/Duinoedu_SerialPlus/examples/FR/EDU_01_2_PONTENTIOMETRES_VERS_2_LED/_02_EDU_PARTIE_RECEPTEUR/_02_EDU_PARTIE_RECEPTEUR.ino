// PARTIE RECEPTEUR //////////////
/* SerialPlus exemple 
   Recevoir les infos de 2 potentiomètres sur 2 LED 
   POT1 => LED1
   POT2 => LED2
   David Souder - 19/04/2015
*/
#include <SerialPlus.h>

SerialPlus monSerialPlus;

void setup(){
  monSerialPlus.branch(&Serial);
  monSerialPlus.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(10,OUTPUT);
}


void loop(){
    analogWrite(6,monSerialPlus.readNbr(CANAL0)/4); 
    analogWrite(10,monSerialPlus.readNbr(CANAL1)/4);
}
