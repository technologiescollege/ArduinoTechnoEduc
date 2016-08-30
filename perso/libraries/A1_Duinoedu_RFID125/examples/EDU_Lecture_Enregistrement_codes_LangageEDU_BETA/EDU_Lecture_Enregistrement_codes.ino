#include <RFID125.h>
#include <SoftwareSerial.h>

utiliser RFID125  monRFID;
utiliser chaine   monCode;
utiliser nombre   monInterr=2;

faire initialisation
{
  utiliser portSerie;
  monRFID.brancher(4,5);

}

faire boucle
{
  monCode = monRFID.lireCode();          
  si(lectureEtat(monInterr)==HIGH){ monRFID.ecrireCode(monCode); }
  si(lectureEtat(monInterr)==LOW ){ monRFID.testerCode(monCode); }                         
}
  
  

