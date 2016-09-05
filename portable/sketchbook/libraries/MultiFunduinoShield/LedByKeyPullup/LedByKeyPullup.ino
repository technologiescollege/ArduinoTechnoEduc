/*----------------------------------------------------
Name: LedByKey

Description: Toggles the leds in sync with the key pressed
              as a toggle.

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
  pinMode(pinKeyA1, INPUT_PULLUP);  
  pinMode(pinKeyA2, INPUT_PULLUP);  
  pinMode(pinKeyA3, INPUT_PULLUP);  
}

void loop()
{
  iKeyState = checkKeyPress(pinKeyA1);  // Function to return key pressed event.
  if (iKeyState == 1)                   // Key has been pressed.
    digitalWrite(pinLedD2, !digitalRead(pinLedD2)); // Toggle the LED.

  iKeyState = checkKeyPress(pinKeyA2);  // Function to return key pressed event.
  if (iKeyState == 1)                   // Key has been pressed.
    digitalWrite(pinLedD3, !digitalRead(pinLedD3)); // Toggle the LED.

  // Does the same as the above but saves 8 bytes of storage space.
  if (checkKeyPress(pinKeyA3) == 1)                 // Key has been pressed.
    digitalWrite(pinLedD4, !digitalRead(pinLedD4)); // Toggle the LED.
}

int checkKeyPress(int pinKey)	
{
  int iKeyPressed = 0;
  
  if (digitalRead(pinKey) == LOW)
  {
    delay(20);	// To handle debounce of the switch.		
    if (digitalRead(pinKey) == LOW)	
    {
      iKeyPressed = 1;		
      while (digitalRead(pinKey) == LOW);
    }
  }
  return iKeyPressed;
}

