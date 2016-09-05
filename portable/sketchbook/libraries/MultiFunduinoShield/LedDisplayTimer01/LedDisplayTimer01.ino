/*----------------------------------------------------
Name: LedDisplayTimer01.

Description: Displays a seconds timer on the 4 digit
              led display.

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

unsigned char ucCharacterMap[]     = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};  // Map the led segements into the letters '0'-'9'.
unsigned char ucCharacterBuffer[]  = {0xF1, 0xF2, 0xF4, 0xF8};                                      // Buffer to hold the mapped letter to displ.

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
  if (ulTimeCurrent - ulTimeLast > 1000) {
    ulTimeLast = ulTimeCurrent;
    uiTimeDisplay++;
    if (uiTimeDisplay > 9999) uiTimeDisplay = 0;
  } 
  ledNumberWrite(uiTimeDisplay);
}

void ledNumberWrite(int uiTimeDisplay)
{
  // Isolate each number as a single decimal digit.
  ledSegmentWrite(0, uiTimeDisplay / 1000);        // Highest order is leftmost digit.
  ledSegmentWrite(1, uiTimeDisplay % 1000 / 100);
  ledSegmentWrite(2, uiTimeDisplay % 100 / 10);
  ledSegmentWrite(3, uiTimeDisplay % 10);          // Lowest order is rightmost digit.
}

void ledSegmentWrite(byte bSegmentNo, byte bDigitValue)
{
  digitalWrite(pinLatch, LOW); 
  shiftOut(pinData, pinClock, MSBFIRST, ucCharacterMap[bDigitValue]);
  shiftOut(pinData, pinClock, MSBFIRST, ucCharacterBuffer[bSegmentNo] );
  digitalWrite(pinLatch, HIGH);    
}
