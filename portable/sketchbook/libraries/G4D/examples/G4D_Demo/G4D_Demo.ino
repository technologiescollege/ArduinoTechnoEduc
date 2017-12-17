#include <SoftwareSerial.h>                         // bibliothèque pour la liaison série logicielle
#include <G4D.h>                                    // bibliothèque 4DSystems G4D

#define tx_G4D         4                            // broche TX de l'arduino pour la liaison série logicielle
#define rx_G4D         5                            // broche RX de l'arduino pour la liaison série logicielle
#define reset_G4D      6                            // broche reset de l'afficheur sur la broche 6 de l'arduino

SoftwareSerial oledSerial(rx_G4D, tx_G4D);          // déclaration de la liaison série logicielle
G4D display(reset_G4D, &oledSerial);                // déclaration du module afficheur

void setup(void)    
{  
  	oledSerial.begin(57600);                    // Communication sur la liaison série logicielle à 57600 bauds
        display.reset();                            // reset de l'afficheur  
        display.clear();                            // effacement de l'écran
        display.opacity(true);                      // texte opaque
        display.replaceBackground(BLACK);           // fond d'écran noir
}
void loop(void)
{                                    
  switch(display.joystick(0x08))                    // lecture de l'état du joystick
  {
    case UP :                                       // si haut
    {
      display.circle(48,30,30,MAGENTA);             // affichage d'un cercle
      display.sound(10,100);                        // gnération d'une note
    }break;
    case DOWN :                                     // si bas
    {
      display.triangle(95,30,0,0,48,59,BLUE);       // affichage d'un triangle
      display.sound(20,100);                        // gnération d'une note
    }break;    
    case LEFT :                                     // si gauche
    { 
      display.line(0,0,95,59,MAROON);               // affichage d'une ligne
      display.sound(30,100);                        // gnération d'une note
    }break;   
    case RIGHT :                                    // si droit
    {
      display.rectangle(30,20,70,50,RED);           // affichage d'un rectangle
      display.sound(40,100);                        // gnération d'une note
    }break;   
    case FIRE :                                     // si bouton central
    {
      display.string(0,2,2,GREEN,"Hello World!");   // affichage d'un texte
      display.sound(50,100);                        // gnération d'une note
    }break;
    case NONE :                                     // si joystick inactif
    {
      display.clear();                              // effacement écran
    }break;       
    delay(50);                                      // on attend
  }
}