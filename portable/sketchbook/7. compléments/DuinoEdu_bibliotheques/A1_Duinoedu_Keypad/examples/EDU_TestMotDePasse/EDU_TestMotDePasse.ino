//========== EXEMPLE DE PROGRAMME
//
//  Enregistre et teste la validité d'un mot de passe
//  Aide-mémoire : https://docs.google.com/document/d/1gM7IOO7I0LjB7bM7shGV29H8wXxdH8QqjNqLDHpkzEg/pub
//  Souder David pour http://www.duinoedu.com
//  06/2014
#include <Keypad.h> // http://duinoedu.com/dl/lib/dupont/EDU_Keypad_Dupont.zip
Keypad keypad;
void setup(){
  Serial.begin(9600);
  keypad.brancher(2,9);
  keypad.ecrireMotDePasse("1234");
  pinMode(A0,OUTPUT);
}


void loop(){
  if(keypad.testerMotDePasseOk()==true){
    digitalWrite(A0,HIGH);
  }
  else {
    digitalWrite(A0,LOW);
  }
}
    
    
 



