/*
  Librairie par David Souder - souder.d@gmail.com
*/

#include <SD.h>
#include <SDPlus.h>
#include <Wire.h>
#include <DS1307.h>

SDPlus maSD;
DS1307 clock;

void setup(){
  maSD.brancher(GROVE_CS);
  clock.brancher();
  
}

void loop(){
  maSD.ecrire("Température");
  maSD.ecrire(analogRead(A0));
  maSD.ecrire("Pression");
  maSD.ecrire(analogRead(A1));
  maSD.ecrire(clock.lireHeures());
  maSD.ecrire(clock.lireMinutes());
  maSD.ecrire(clock.lireSecondes());
  maSD.nouvelleLigne(1);
}

/* AUTRES FONCTIONS
maSD.definirFichierCourant("MONFICHIER.TXT"); // Création/sélection du fichier de travail
maSD.definirSeparateur("  ");       // Change le séparateur par défaut
maSD.espace(5);                     // Ajoute 5 espaces
maSD.tabulation(2);                 // Ajoute 2 tabulations  
maSD.serialInfos=OUI;
*/
