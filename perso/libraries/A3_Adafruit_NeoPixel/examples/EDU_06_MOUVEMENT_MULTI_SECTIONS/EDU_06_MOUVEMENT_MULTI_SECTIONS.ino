#include <Adafruit_NeoPixel.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_Adafruit_NeoPixel_FlexStrip_Grove/ 
Adafruit_NeoPixel monRuban_pin2 = Adafruit_NeoPixel(60,2, NEO_GRB + NEO_KHZ800);

int adresse=0;

void setup()
{
  monRuban_pin2.brancher();
  monRuban_pin2.afficher();
}

void loop()
{
  // Valeur des LED non initialisées (0,0,0)
  monRuban_pin2.preparerPixel(0,0,0,0,60);                       // Section de 50 LED éteintes
  
  // Section blanche
  monRuban_pin2.preparerPixel(adresse, 1000,1000,1000,10);       // Section de 10 LED allumées
  
  // Section verte
  monRuban_pin2.preparerPixel(-adresse+10, 0,1000,0,10);         // Section de 10 LED allumées

  // Section rouge
  monRuban_pin2.preparerPixel(2.5*adresse, 1000,0,0,10);      // Section de 10 LED allumées  
  
  // Section rose
  monRuban_pin2.preparerPixel(-0.5*adresse, 1000,0,500,10);      // Section de 10 LED allumées  
  
  
  adresse=adresse+1;   // Incrément
  delay(50);           // Limitation vitesse
  
  
  
  monRuban_pin2.afficher();
}


