#include <Adafruit_NeoPixel.h>
#include <Duinoedu_NeoPixelAdd.h>

// Avec Duino augmenté http://duinoedu.com/duinoaugmente.html - David Souder / Karl Thomas

Duinoedu_NeoPixelAdd monRuban_pin2 = Duinoedu_NeoPixelAdd(60,2, NEO_GRB + NEO_KHZ800);

void setup(){
  monRuban_pin2.brancher();
  monRuban_pin2.afficher();
}

void loop(){
  monRuban_pin2.preparerPixel(20,1000 ,1000 ,1000 ,30 ,1 ,1000 ,1000 ,1000, 0);
  monRuban_pin2.afficher();
}

/* Préparer pixels(
   LED de début, [0,1..]
   quantité de rouge [0..1000]
   quantité de vert
   quantité de blue
   nombre de LED du groupe
   pas (tout les x LED) [1,2..]
   quantité de rouge en fin de groupe [0..1000]
   quantité de rvert en fin de groupe
   quantite de blue en fin de groupe
   défilement [-1000..0..1000]
*/


   
  




