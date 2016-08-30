#include <Shieldbot.h>
Shieldbot monBot = Shieldbot();

void setup(){
  monBot.brancher();
  monBot.ecrireVitesse(1000,1000);
}

void loop(){
  monBot.avancer("tout_droit");
  monBot.avancer("a_droite");
  monBot.avancer("a_gauche");


  monBot.reculer("tout-droit");
  monBot.reculer("a_droite");
  monBot.reculer("a_gauche");
  
  monBot.pivoter("a_droite");
  monBot.pivoter("a_gauche");

  monBot.stopper();
  
  monBot.lire("S1");
  monBot.lire("S2");
  monBot.lire("S3");
  monBot.lire("S4");
  monBot.lire("S5");
  

}
  
 
