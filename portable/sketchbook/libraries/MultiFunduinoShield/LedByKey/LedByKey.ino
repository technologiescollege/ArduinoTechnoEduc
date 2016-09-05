/*----------------------------------------------------
Name: LedByKey

Description: Set the leds in sync with the key pressed.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino

Author: Bill Brouwer
Date:   14Nov2014

This code is in the public domain.
------------------------------------------------------*/
int pinLedD2 = 12;  // Define the D2 pin as led.
int pinLedD3 = 11;  // Define the D3 pin as led.
int pinLedD4 = 10;  // Define the D4 pin as led.
int pinKeyA1 = A1;  // Define key 1 pin label s1-a1.
int pinKeyA2 = A2;  // Define key 2 pin label s2-a2.
int pinKeyA3 = A3;  // Define key 3 pin label s3-a3.

int iKeyState;      // Key state.

void setup()
{
  // Define the led pins to be output.
  pinMode(pinLedD2, OUTPUT); 
  pinMode(pinLedD3, OUTPUT); 
  pinMode(pinLedD4, OUTPUT); 

  // Define the key switchs to be input.
  pinMode(pinKeyA1, INPUT);  
  pinMode(pinKeyA2, INPUT);  
  pinMode(pinKeyA3, INPUT);  
}

void loop()
{
  iKeyState = digitalRead(pinKeyA1); // Read the key state into variable..
  if (iKeyState == LOW)              // Test the state - Low - off, high pressed.
    { digitalWrite(pinLedD2, LOW);}  // Button not pressed turn off the LED.
  else
    { digitalWrite(pinLedD2, HIGH);} // Button pressed turn on the LED.
    
  // Another way of doing the above.
  iKeyState = digitalRead(pinKeyA2); // Read the key switch into variable.
  digitalWrite(pinLedD3, iKeyState); // Set the LED to be the same as the key state variable.
    
  // Yet another way of doing the above.
  digitalWrite(pinLedD4, digitalRead(pinKeyA3)); // Set the LED to be the same state as the key.
}

