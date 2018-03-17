#include <Adafruit_NeoPixel.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_Adafruit_NeoPixel_FlexStrip_Grove/ 
Adafruit_NeoPixel monRuban_pin2 = Adafruit_NeoPixel(60,2, NEO_GRB + NEO_KHZ800);

int adresse=0;

void setup()
{
  monRuban_pin2.brancher();

}

void loop()
{
  
  // Dégradé circulaire
  monRuban_pin2.preparerPixel(adresse,1000,0,0,30,1,0,0,1000);    // Roge > bleu 30 LED
  monRuban_pin2.preparerPixel(adresse+30,0,0,1000,30,1,1000,0,0);    // Bleu > rouge 30 LED
    
  adresse=adresse+1;   // Incrément
  delay(50);           // Limitation vitesse
  
  
  
  monRuban_pin2.afficher();
}


