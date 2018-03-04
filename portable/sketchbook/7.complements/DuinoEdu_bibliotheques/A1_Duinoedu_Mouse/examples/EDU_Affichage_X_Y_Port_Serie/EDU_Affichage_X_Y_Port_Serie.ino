// Prise PS2 Grove pour souris http://duinoedu.com/store1/04-modules-de-base/205-prise-ps2-pour-souris.html 


#include <Mouse.h>    // http://duinoedu.com/dl/lib/grove/EDU_Mouse_Grove.zip 
Mouse maSouris;


void setup(){
    maSouris.brancher(6,7);    // Ou autre couple 2/3 - 4/5...
    Serial.begin(9600);
}


void loop(){
    Serial.print("POSITION EN X : ");
    Serial.println(maSouris.lireX());
    
    Serial.print("POSITION EN Y : ");
    Serial.println(maSouris.lireY());
    
}










