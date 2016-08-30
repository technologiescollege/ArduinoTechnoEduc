#include <IRremote.h>


//Récepteur IR branché en 2
IRrecv monRecepteur(2);
//LED branchée en 4

void setup(){
  monRecepteur.brancher();
  Serial.begin(9600);
  
}

void loop(){
  
  if( monRecepteur.testerTouche("+", IRREMOTE_MEMORISER_TOUCHE)){
                                      //IRREMOTE_MEMORISER_200_MS
                                      //IRREMOTE_NE_PAS_MEMORISER
                                      // ou valeur directe en ms
                                      // ou rien
    digitalWrite(4,HIGH);
  }

  else{
    digitalWrite(4,LOW);
  }
}


