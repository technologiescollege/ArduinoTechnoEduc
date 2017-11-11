// DuinoEDU.com
// Aide : https://docs.google.com/document/d/1oVZ7rr-QR3uaaPbLCNabH4d8OYHT2vW4nTq3AHtu-eo/pub

#include <Wire.h>        
#include <MMA7660.h>  // http://duinoedu.com/dl/lib/grove/EDU_MMA7660_Grove.zip
MMA7660 monAccelerometre;

void setup(){
    monAccelerometre.brancher();
    Serial.begin(9600);
}

void loop(){
    float variation = monAccelerometre.mesurerMouvement();
    Serial.println(variation);
} 
