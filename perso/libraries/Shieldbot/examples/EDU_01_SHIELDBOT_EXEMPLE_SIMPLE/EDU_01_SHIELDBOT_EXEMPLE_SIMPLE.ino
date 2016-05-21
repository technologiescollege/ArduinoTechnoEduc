#include <Shieldbot.h>
Shieldbot monBot = Shieldbot();

void setup(){
  monBot.brancher();
  Serial.begin(9600);
}

void loop(){
  monBot.avancer();
  delay(2000);
  monBot.stopper();
  delay(2000);
  Serial.println(monBot.lire("S1"));
  //monBot.reculer();
  //delay(2000);
  //monBot.pivoter("droite");
  //delay(2000);
  //monBot.pivoter("gauche");
  //delay(2000);

}
  
 
