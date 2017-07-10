#include <IRremote.h>

//libraries at http://duinoedu.com/dl/lib/grove/EDU_IRremote_GroveDupont/
IRrecv monRecepteur_pin2(2);     // Broche sur laquelle est le récepteur IR

void setup(){
  monRecepteur_pin2.brancher(); 
}

void loop(){
   if(monRecepteur_pin2.testerTouche("POWER",IRREMOTE_MEMORISER_TOUCHE)){
      //Noms utilisés pour les touches de la télécommande TEL1 de www.duinoedu.com  
        //"POWER" ou "ON/OFF"ou "ON" 
	//"MENU" 
	//"TEST" 
	//"+" 
	//"ANNULER" 
	//"RETOUR" 
	//"LECTURE"
	//"AVANCE"  
	//"0"
	//"-" 
	//"C"  
	//"1" ... "9"    
     //OPTIONS INDISPENSABLES
        // IRREMOTE_NE_PAS_MEMORISER = un nombre = -1  = La touche est valide une fraction de seconde
        // IRREMOTE_MEMORISER_200_MS = un nombre = 200 = Le test est valide pendant 200ms
        // IRREMOTE_MEMORISER_2000_MS= un nombre = 2000= Le test est valide 2 secondes
        // On peut mettre directement un nombnre de ms
     
     //Ce que doit faire votre programme lors d'un appui
        //...
   }   
}

/* REMARQUES
  > Toutes les touches de la télécommande TEL1 sont enregistrées
  > On test ici la touche et pas le code de la touche
  > Ne fonctionne qu'avec la télécommande TEL1
*/
    


