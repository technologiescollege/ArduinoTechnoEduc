//==============================================================
// Lib : duinoeduGpsAdd par David Souder pour www.duinoedu.com
// Adafruit_GPS par Adafruit
// SoftwareSerial par Arduino
// Version du 28/07/2015
//==============================================================

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <duinoeduGpsAdd.h>

duinoeduGpsAdd monGps;

// EQUIVALENCES D'ECRITURE
#define SP Serial.print
#define BR Serial.println()
#define VR Serial.print(" , ")
#define SL Serial.print("/")
#define HH Serial.print('h');
#define MN Serial.print("m ");
#define PP Serial.print('.');
#define SS Serial.print('s');
#define KM Serial.print(" km/h");
#define DG Serial.print(" DEGRE(S)");
#define MM Serial.print(" m");

void setup(){
  Serial.begin(9600);                             // Passer à 115200 ici et dans le moniteur série si trop lent
  monGps.brancher(4,5);                           // Autres couples possibles 
                 
  
  monGps.serialInfos=OUI;                         // Optionnel : affiche quelques infos supp
  monGps.activerInterruption();                   // Optionnel desactiverInterruption()
  monGps.definirQuantiteInformation(1);           // Optionnel 1=RMC 2=RMC+GGA 3=TOUT 0=RIEN 
  monGps.definirFrequenceTrames(EDUGPS_1HZ);      // Optionnel EDUGPS_1HZ/5HZ,10HZ,100MLHZ,200MLHZ
  monGps.definirFrequencePortSerie(9600);         // Optionnel 9600 4800
  monGps.definirDecalageHoraire(2);               // +/- heure(s)
  
}

EDUGPS_INTERRUPTION(HORLOGE_0_DEPASSE_VALEUR_A) {
  monGps.lireUnCaractere();
}

void loop(){
  BR; SP("POSITION NORMALE COMME 1 NBR :");
  BR; SP(monGps.latitudeNbr() , 4);   SP(monGps.latitudeDirection());
  VR; SP(monGps.longitudeNbr(), 4);   SP(monGps.longitudeDirection());

  BR; SP("POSITION NORMALE COMME 1 CHAINE :");
  BR; SP(monGps.latitudeTxt(4)); /*nbr de décimales*/ SP(monGps.latitudeDirection());
  VR; SP(monGps.longitudeTxt(4));/*nbr de décimales*/ SP(monGps.longitudeDirection());
  
  BR; SP("POSITION GOOGLE MAPS COMME 1 NBR :");
  BR; SP(monGps.latitudeGoogleMapsNbr() , 4);
  VR; SP(monGps.longitudeGoogleMapsNbr(), 4);

  BR; SP("POSITION GOOGLE MAPS COMME 1 CHAINE :");
  BR; SP(monGps.latitudeGoogleMapsTxt(4)); /*nbr de décimales*/ 
  VR; SP(monGps.longitudeGoogleMapsTxt(4));    /*nbr de décimales*/
  
  BR; SP("DATE COMME DES NBR :");
  BR; SP(monGps.jour());
  SL; SP(monGps.mois());
  SL; SP(monGps.annee(4)); // 2 ou 4 chiffres
  
  BR; SP("HEURE COMME DES NBR :");
  BR; SP(monGps.heures());
  HH; SP(monGps.minutes());
  MN; SP(monGps.secondes());
  PP; SP(monGps.millisecondes());
  SS;
  
  BR; SP("HEURE COMME 1 CHAINE FORMATEE :");  
  BR; SP(monGps.time("HM")); // HMSms affiche xHxMxSxms - hm affiche xhxm 
  
  BR; SP("VITESSE :");
  BR; SP(monGps.vitesse()); KM;
  
  BR; SP("ANGLE DE DEPLACEMENT :");
  BR; SP(monGps.angle()); DG;     // 0 à 360°
  
  // Et si on ne veut que des données valides (GPS synchronisé) 
  if(monGps.estSynchronise()){
    //...
    BR; SP("VITESSE :");
    BR; SP(monGps.vitesse()); KM;
    //...
  }
  
  //BR; SP("ALTITUDE :");  
  //BR; SP(monGps.altitude()); MM; // (Uniquement si bonne récep) QtInfosMini:2 Ralentir loop():bcp d'infos 
  
   // VEILLE
    //monGps.mettreEnVeille();
    //monGps.reveiller();
  // APPEL DIRECT METHODES ET ATTRIBUTS ADAFRUIT
    // monGps.gps->methodeAdafruit();
    // monGps.gps->attribut;
  
  BR; delay(2000);
}
