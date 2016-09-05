/*----------------------------------------------------
Name: LedByPotentiometer.

Description: Varies the brightness of the leds based on
             the value read in from the potentiometer.

Hardware Shield： Funduino Multi-Function Board K74544BS.
Tesed Boards:     Arduino UNO CH431
                  BTE UNO Clone
                  Osepp Uno R3 Plus
                  Leonardo Eduino

Author: Bill Brouwer
Date:   15Nov2014

This code is in the public domain.
------------------------------------------------------*/
#define DEBUG 1 // 0 - Turn off serial monitor - 1 turn on.

int pinLedD1 = 13;  // Define the D1 pin as led (on most arduino boards this is NOT a PWM pin.
int pinLedD2 = 12;  // Define the D2 pin as led (on most arduino boards this is NOT a PWM pin.
int pinLedD3 = 11;  // Define the D3 pin as led.
int pinLedD4 = 10;  // Define the D4 pin as led.

int pinPotentiometer = A0;  // Define Analog pin A0 as Potentiometer.

unsigned int uiPotentiometerValue = 0; 

void setup()
{
  #if DEBUG
    Serial.begin(9600); // Set baud rate to 9600.
  #endif
  // Define the led pins to be output.
  pinMode(pinLedD1, OUTPUT); 
  pinMode(pinLedD2, OUTPUT); 
  pinMode(pinLedD3, OUTPUT); 
  pinMode(pinLedD4, OUTPUT); 
}

void loop()
{
  unsigned int uiPWM; // Pulse width modulation value.
  
  uiPotentiometerValue = analogRead(pinPotentiometer);  
  // Sensor resolution is 0 - 1023.  
  // PWM has range of 0 - 255 therefore divide by 4.
  uiPWM = uiPotentiometerValue / 4;
  
  #if DEBUG
    // Output text to serial port.
    Serial.print("Potentiometer = ");
    Serial.print(uiPotentiometerValue); 
    
    Serial.print("  PWN Value = ");
    Serial.println(uiPWM); 
  #endif
  
  // Set the level of brightness.
  analogWrite(pinLedD1, (255 - uiPWM)); // Not a PWM pin - will be on or off.
  analogWrite(pinLedD2, uiPWM);         // Not a PWM pin - will be on or off. 
  analogWrite(pinLedD3, (255 - uiPWM)); // Reverse to show contrast.
  analogWrite(pinLedD4, uiPWM);         
  
  delay(100); // Delay for 10th of a second.
}
