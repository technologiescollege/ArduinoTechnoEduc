#include <EasyTransfer.h>
#include <EEPROM.h>
#include <Guino.h>

int _ABVAR_1_Guinolecture = 0 ;
//libraries at http://duinoedu.com/dl/lib
int amplitude = 0;

void setup()
{
  //--- Pilotage
  pinMode(8, OUTPUT);
  GUINO_BRANCHER();
}

void GUINO_DEFINIR_INTERFACE(){
  
  GUINO_AFFICHER_POTENTIOMETRE("REGLAGE LED", amplitude, 3, 127);
}

void loop(){
  GUINO_GERER_INTERFACE();
  analogWrite(8, 2*amplitude);
}


