#include <EduMp3.h>
#include <SoftwareSerial.h>

EduMp3 monMp3(2,3);

void setup(){      
    monMp3.brancher();
    monMp3.reglerVolume(800);      // Volume de 0 à 1000
}

void loop(){
  monMp3.lireMorceau(1);
  while(1);
}