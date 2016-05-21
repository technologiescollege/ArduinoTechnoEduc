// ÉCRAN OLED graphique 128x64 http://duinoedu.com/store1/05-modules-evolues/188-ecran-oled-i2c-grove.html

#include <Wire.h>          // Déjà dans Arduino
#include <SeeedOLED.h>     // http://duinoedu.com/dl/lib/grove/EDU_OLED_Display12864_Grove.zip

void setup(){
  SeeedOled.brancher();
}

void loop(){
  SeeedOled.placerCurseurEn(0);      // Ligne 0
  SeeedOled.ecrire("MON TITRE");
  
  SeeedOled.placerCurseurEn(2);      // Ligne 2    
  SeeedOled.ecrire("Valeur :");

  SeeedOled.placerCurseurEn(4);      // Ligne 4     
  SeeedOled.ecrire(1234);
} 
