/*----------------------------------------------------
Name: Blink4.

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
int pinLedBase = 10;  // Starting at LED labeled D1.
int iCount     = 4;   // The total number of Led's on the shield.
int iDelay     = 200; // Sets the speed of the cascade effect.

void setup()
{
   for (int iPinLedNo = pinLedBase; iPinLedNo < (pinLedBase + iCount); iPinLedNo ++) 
   {
     pinMode(iPinLedNo, OUTPUT);   // Set I/O of pins for output.
   }
}

void loop()
{
   for (int iPinLedNo = pinLedBase; iPinLedNo < (pinLedBase + iCount); iPinLedNo ++) 
   {
     digitalWrite(iPinLedNo, LOW);   // Turn off LED.
     delay(iDelay);               
   }
   
   for (int iPinLedNo = pinLedBase; iPinLedNo < (pinLedBase + iCount); iPinLedNo++) 
   {
     digitalWrite(iPinLedNo, HIGH);  // Turn on LED.
     delay(iDelay);                   
   }  
}
