// PARTIE EMETTEUR //////////////
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
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}


void loop(){

  monSerialPlus.print(analogRead(A0), CANAL0);
  monSerialPlus.print(analogRead(A1), CANAL1);
  //delay(10);
}
  

  
  
  
  

