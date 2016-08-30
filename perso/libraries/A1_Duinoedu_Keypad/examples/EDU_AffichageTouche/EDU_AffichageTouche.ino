//========== EXEMPLE DE PROGRAMME
//
//  Affiche la dernière touche appuyée
//  Aide-mémoire : https://docs.google.com/document/d/1gM7IOO7I0LjB7bM7shGV29H8wXxdH8QqjNqLDHpkzEg/pub
//  Souder David pour http://www.duinoedu.com
//  06/2014
#include <Keypad.h> // http://duinoedu.com/dl/lib/dupont/EDU_Keypad_Dupont.zip
Keypad keypad;
void setup(){
  Serial.begin(9600);
  keypad.brancher();      // 2 à 9 par défaut
}


void loop(){
  
  keypad.lire();          // Par défaut, affichage de la touche dans le port série
}
    
    
 



