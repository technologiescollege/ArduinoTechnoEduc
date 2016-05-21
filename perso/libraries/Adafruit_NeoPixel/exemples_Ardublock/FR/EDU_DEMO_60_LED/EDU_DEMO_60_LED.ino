#include <Adafruit_NeoPixel.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_Adafruit_NeoPixel_FlexStrip_Grove/ 
Adafruit_NeoPixel monRuban_pin2 = Adafruit_NeoPixel(60,2, NEO_GRB + NEO_KHZ800);
int adresse;

void setup()
{
  Serial.begin(9600);
  monRuban_pin2.brancher();
  monRuban_pin2.afficher();
  adresse=0;
}

void loop()
{
  // .preparerPixel(n° 1ère LED, quantité rouge début, v, b, largeur du groupe, pas, quantité rouge fin, v, b) 
  monRuban_pin2.preparerPixel(adresse,1000 ,0,0,30,1,0,0,1000);
  monRuban_pin2.preparerPixel(adresse+30,0 ,0,1000,30,1,1000,0,0);
  monRuban_pin2.preparerPixel(2*adresse,0 ,1000,0,4);
  monRuban_pin2.preparerPixel(-1.1*adresse,1000 ,1000,0,2);
  monRuban_pin2.afficher();
  delay(50);
  adresse+=2;
  
}


