/*----------------------------------------------------
Name: Blink4.

Description: Blinks the 4 LED pins.

Description: Cascade Blinks the LED pins.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino
                  
Author: Bill Brouwer
Date:   14Nov2014

This code is in the public domain.
------------------------------------------------------*/
// Define the LED's labeled D1 to D4 on the board.
char ledPinD1 = 13; 
char ledPinD2 = 12; 
char ledPinD3 = 11; 
char ledPinD4 = 10; 

void setup()
{
  // Set the led pins to be output.
  pinMode(ledPinD1, OUTPUT); 
  pinMode(ledPinD2, OUTPUT);  
  pinMode(ledPinD3, OUTPUT);  
  pinMode(ledPinD4, OUTPUT);  
}

void loop()
{
  // Turn on all LED lights.
  digitalWrite(ledPinD1, HIGH); 
  digitalWrite(ledPinD2, HIGH); 
  digitalWrite(ledPinD3, HIGH); 
  digitalWrite(ledPinD4, HIGH); 
  delay(1000); // Delay 1 seconds
  
  // Turn off all LED lights.
  digitalWrite(ledPinD1, LOW); 
  digitalWrite(ledPinD2, LOW); 
  digitalWrite(ledPinD3, LOW); 
  digitalWrite(ledPinD4, LOW); 
  delay(1000);
}

