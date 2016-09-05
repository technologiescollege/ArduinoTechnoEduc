/*----------------------------------------------------
Name: BuzzerByKey

Description: Sounds the buzzer with a different
              frequency based on which key is
              pressed.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino
  
Author: Bill Brouwer
Date:   14Nov2014

This code is in the public domain.
------------------------------------------------------*/
int pinLedD2 = 12;  // Define the D3 pin as led.
int pinLedD3 = 11;  // Define the D2 pin as led.
int pinLedD4 = 10;  // Define the D1 pin as led.
int pinKeyA1 = A1;  // Define key 1 pin label s1-a1.
int pinKeyA2 = A2;  // Define key 2 pin label s2-a2.
int pinKeyA3 = A3;  // Define key 3 pin label s3-a3.

int pinBuzzer = 3;  // The buzzer is connected to digital pin 3.

int iKeyState1;     // Key state for key 1.
int iKeyState2;     // Key state for key 2. 
int iKeyState3;     // Key state for key 3.

void setup()
{
  // Define the led pins to be output.
  pinMode(pinLedD2, OUTPUT); 
  pinMode(pinLedD3, OUTPUT); 
  pinMode(pinLedD4, OUTPUT); 
  
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, HIGH);  // Turn off the sound.

  // Define the key switchs to be input.
  pinMode(pinKeyA1, INPUT);  
  pinMode(pinKeyA2, INPUT);  
  pinMode(pinKeyA3, INPUT);  
}

void loop()
{
  // The arduino loop() runs continuously in an never ending loop with the frequency determined
  // by the speed of the processor.  Sound is based on frequency (more than this is beyond 
  // the scientific ability of this lowly systems analyst and the code uses this fact and the 
  // infinite  looping by modify the execution frequency via the delay() funciton.
  // For more on sound frequency please see see http://en.wikipedia.org/wiki/Sound
    
  iKeyState1 = digitalRead(pinKeyA1); // Read the key state into variable.
  digitalWrite(pinLedD2, iKeyState1); // Set the LED to be the same as the key state variable.
  if (iKeyState1 == LOW) SoundBuzzer(2);
  
  iKeyState2 = digitalRead(pinKeyA2); // Read the key state into variable.
  digitalWrite(pinLedD3, iKeyState2); // Set the LED to be the same as the key state variable.
  if (iKeyState2 == LOW) SoundBuzzer(5);

  iKeyState3 = digitalRead(pinKeyA3); // Read the key state into variable.
  digitalWrite(pinLedD4, iKeyState3); // Set the LED to be the same as the key state variable.
  if (iKeyState3 == LOW) SoundBuzzer(8);
  
  if ((iKeyState1 + iKeyState2 + iKeyState3 == 3)) {
    // None of the keys are pressed therefore turn the buzzer off.
    digitalWrite(pinBuzzer, HIGH);
  }
}

void SoundBuzzer(unsigned int uiFreq) {
  if (uiFreq > 0) {
    digitalWrite(pinBuzzer, HIGH);   // Tnrn buzzer on.
    delay(uiFreq);                   // Setting the delay determines frequency.
      
    digitalWrite(pinBuzzer, LOW);   // Turn buzzer off.
    delay(uiFreq);                  // Setting the delay determines frequency.
  }
  else digitalWrite(pinBuzzer, HIGH); // Turn buzzer off.
}
