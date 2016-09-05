/*----------------------------------------------------
Name: LedDisplayLM35.

Description: Displays the value of the LM35 port to 
              the 4 digit led display.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino

Author: Bill Brouwer
Date:   24Nov2014

This code is in the public domain.
------------------------------------------------------*/
#define DEBUG 0 // 0 - Turn off serial monitor - 1 turn on.

// Define the pins for the 4 digit LED display using 2 MC74HC595AD chips
// labeled u2 and u3 on the Funduino Shield.
int pinLatch = 4; // Latch pin (LCHCLK - pin 12 both u2 and u3).
int pinClock = 7; // Clock pin (SFTCLK - pin 11 both u2 and u3).
int pinData  = 8; // Data pin  (SD1 - pin 14 - u2 only - u3 SD1 pin 14 goes to u2 SD0 pin 9).

int pinLM35 = A4; // A4 corresponds to the LM35-A4 port on the Funduino Shield.

unsigned char ucCharacterMap[]     = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};  // Map the led segements into the letters '0'-'9'.
unsigned char ucCharacterBuffer[]  = {0xF1,0xF2,0xF4,0xF8};                                // Buffer to hold the mapped letter to display.

unsigned long ulTimeCurrent;
unsigned long ulTimeLast    = 0;

void setup ()
{
  #if DEBUG
    Serial.begin(9600); // Set baud rate to 9600.
  #endif
  // Define the outprt pins.
  pinMode(pinLatch, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinData,  OUTPUT); 
  analogReference(INTERNAL);
}

void loop()
{ 
  unsigned int uiValue;
  byte bDecimalPos;
  
  ulTimeCurrent = millis();
  if (ulTimeCurrent - ulTimeLast > 100) {
    // Read the analog every 100 milliseconds as per above.
    unsigned int u1AnalogValue = analogRead(pinLM35);
    float fValue = (float) u1AnalogValue * (2.56 / 1023.0);

    // Determine the decimal point position for the 4 digit display.
    if (fValue < 1.0) {
        uiValue = (int) (fValue * 1000);
        bDecimalPos = 1;
       } 
     else if (fValue < 10.0) {
        uiValue = (int) (fValue * 1000);
        bDecimalPos = 1;
       } 
     else if (fValue < 100.0) {
        uiValue = (int) (fValue * 100);
        bDecimalPos = 2;
       } 
     else if (fValue < 1000.0) {
        uiValue = (int) (fValue * 10);
        bDecimalPos = 3;
       } 
     else {
        uiValue = (int) (fValue);
        bDecimalPos = 4;
     }
    
    #if DEBUG
      // Output text to serial port.
      Serial.print("LM 35 value = ");
      Serial.print(u1AnalogValue); 
      
      Serial.print("  Float Value = ");
      Serial.print(fValue, 4); 
       
      Serial.print("  Display Value = ");
      Serial.print(uiValue); 
      
      Serial.print("  Decimal Pos = ");
      Serial.println(bDecimalPos); 
    #endif
  }  
  // Display the value with a decimal on the 3rd digit.
  ledNumberWrite(uiValue, bDecimalPos);
}

void ledNumberWrite(int uiNumber, byte bDecimalPosition)
{
  // Do the lowest order digits first to make the math easier.
  for (byte bDigitPosition = 4; bDigitPosition > 0; bDigitPosition--) { 
    ledSegmentWrite((bDigitPosition - 1),                  // Digit position from left to right (0 - 3).
                    (uiNumber % 10),                       // The modulus of the number.
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


