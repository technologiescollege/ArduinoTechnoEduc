/*----------------------------------------------------------------
Name: Buzzer

Description: Sounds the buzzer.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino

Author: Bill Brouwer
Date:   14Nov2014

This code is in the public domain.
----------------------------------------------------------------*/
int pinBuzzer = 3;   // The buzzer is connected to digital pin 3

void setup() 
{ 
  pinMode(pinBuzzer, OUTPUT);
} 

void loop() 
{ 
  // The outer loop determines the frequency of the sound pulse.
  for (unsigned char iOuter = 1; iOuter < 5; iOuter++) 
  {
    for(unsigned char iInner = 0; iInner < 100; iInner++) 
    { 
      digitalWrite(pinBuzzer, HIGH);
      delay(iOuter); // Setting the delay determines the sound pulse frequency.
      
      digitalWrite(pinBuzzer, LOW);
      delay(iOuter); // Setting the delay determines the sound pulse frequency.
    } 
  }
}

