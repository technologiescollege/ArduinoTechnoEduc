/*----------------------------------------------------
Name: LedDisplayTimer02.

Description: Displays a seconds timer on the 4 digit
              enhanced with decimal point led display.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino

Author: Bill Brouwer
Date:   15Nov2014

This code is in the public domain.
------------------------------------------------------*/
// Define the pins for the 4 digit LED display using 2 MC74HC595AD chips
// labeled u2 and u3 on the Funduino Shield.
int pinLatch = 4; // Latch pin (LCHCLK - pin 12 both u2 and u3).
int pinClock = 7; // Clock pin (SFTCLK - pin 11 both u2 and u3).
int pinData  = 8; // Data pin  (SD1 - pin 14 - u2 only - u3 SD1 pin 14 goes to u2 SD0 pin 9).

// Map the led segements into the letters '0'-'9'.
unsigned char ucCharacterMap[]     = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};  
// Buffer to hold the mapped letter to display.
unsigned char ucCharacterBuffer[]  = {0xF1, 0xF2, 0xF4, 0xF8};                                      

unsigned long ulTimeCurrent;
unsigned long ulTimeLast    = 0;
unsigned int  uiTimeDisplay = 0;

void setup ()
{
  // Define the pins as output;
  pinMode(pinLatch, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinData,  OUTPUT); 
}

void loop()
{
  ulTimeCurrent = millis();

  // Adjust the interval also requires changing the decimal position.
  // 1000 corresponds to position 4 (or 0 for none)
  // 100              to          3 
  // 10               to          2.
  if (ulTimeCurrent - ulTimeLast > 1000) {
    
    ulTimeLast = ulTimeCurrent;  // Set the last time to the current.
    uiTimeDisplay++;             // Increment the display timer value.
    if (uiTimeDisplay > 9999) uiTimeDisplay = 0; // Reset to 0 as the maximum displayed is 9999
  } 
  
  // Display the time with the decimal position - corresponds to the iterval above.
  ledNumberWrite(uiTimeDisplay, 4);
}

void ledNumberWrite(unsigned int uiNumber, byte bDecimalPosition)
{
  // Do the lowest order digits first to make the math easier.
  for (byte bDigitPosition = 4; bDigitPosition > 0; bDigitPosition--) { 
    ledSegmentWrite((bDigitPosition - 1),                  // Digit position from left to right (0 - 3).
                    (uiNumber % 10),                       // The modulus of the time display.
                    (bDigitPosition == bDecimalPosition)); // the decimal position from left to right (1 - 4)
                                                           //   '==' is a comparison evaluating to 1 if true
                                                           //   0 if false. 
    uiNumber /= 10; // Remove the lowest order digit by truncation.
  }
}

void ledSegmentWrite(byte bDigitPosition, byte bDigitValue, byte bDecimal)
{
  unsigned char ucLedCharacterValue = ucCharacterMap[bDigitValue];

  // Light the decimal segment by using the and (&) boolean operator.
  if (bDecimal) ucLedCharacterValue = ucCharacterMap[bDigitValue] & 0x7F;
  
  digitalWrite(pinLatch, LOW); 
  shiftOut(pinData, pinClock, MSBFIRST, ucLedCharacterValue); 
  shiftOut(pinData, pinClock, MSBFIRST, ucCharacterBuffer[bDigitPosition] );
  digitalWrite(pinLatch, HIGH);    
}
