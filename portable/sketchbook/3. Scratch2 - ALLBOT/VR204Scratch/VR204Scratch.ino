/* VR204
   Copyright (C) 2016 Velleman nv

   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.

*/

#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <SerialALLBOT.h>                 // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

SerialALLBOT BOT(4);                      // Create a SerialALLBOT

enum MotorName {
  hipLeft,
  hipRight,
  ankleRight,
  ankleLeft,
};

int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

///
// Setup
///
void setup()
{

  // Attach limbs
  BOT.attach(hipLeft,   11,  90, 1, 0);
  BOT.attach(hipRight,  3,  90, 0, 0);
  BOT.attach(ankleLeft,     12,  90, 1, 0);
  BOT.attach(ankleRight,    2,  95, 0, 0);

  // Set up commands
  BOT.registerCommand("WF", walkforward);
  BOT.registerCommand("WB", walkbackward);
  BOT.registerCommand("WL", walkleft);
  BOT.registerCommand("WR", walkright);
  BOT.registerCommand("SC", scared);
  BOT.registerCommand("DA", digitalWriteHigh);
  BOT.registerCommand("DU", digitalWriteLow);
  BOT.registerCommand("AW", analogWriteV);
  BOT.registerCommand("CH", chirp);
  BOT.registerCommand("KL", lookleft);
  BOT.registerCommand("KR", lookright);

  // INIT sounder
  pinMode(sounderPin, OUTPUT);

  // Wait for joints to be initialized
  delay(500);

  // Start serial communication
  Serial.begin(115200);
  Serial.setTimeout(100);

  // Chirp for ready
  chirp(1, 50);
  chirp(1, 255);
  chirp(3, 0);
}

///
// Main program loop
///
void loop()
{
  BOT.update();
}

///
// Commands
///

void chirp(int beeps, int speedms)
{
  for (int i = 0; i < beeps; i++)
  {
    for (int i = 0; i < 255; i++)
    {
      digitalWrite(sounderPin, HIGH);
      delayMicroseconds((355 - i) + (speedms * 2));
      digitalWrite(sounderPin, LOW);
      delayMicroseconds((355 - i) + (speedms * 2));
    }

    delay(30);
  }
}

void chirp(int argc, int argv[])
{
  chirp(argv[0], argv[1]);
}


void scared(int argc, int argv[])
{
  int shakes  = argv[0];
  int beeps   = argv[1];

  for (int i = 0; i < shakes; i++) {

    BOT.move(ankleLeft, 45);
    BOT.move(ankleRight, 45);
    BOT.animate(100);

    BOT.move(ankleLeft, 135);
    BOT.move(ankleRight, 135);
    BOT.animate(100);
  }
  BOT.move(ankleLeft, 90);
  BOT.move(ankleRight, 90);
  BOT.animate(100);

  chirp(beeps, 0);
}

void lookleft(int argc, int argv[])
{
  int speedms = argv[0];

  BOT.move(hipLeft, 135);
  BOT.move(hipRight, 45);
  BOT.animate(speedms);

  delay(speedms / 2);

  BOT.move(hipLeft, 90);
  BOT.move(hipRight, 90);
  BOT.animate(speedms);
}

void lookright(int argc, int argv[])
{
  int speedms = argv[0];

  BOT.move(hipLeft, 45);
    BOT.move(hipRight, 135);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(hipLeft, 90);
    BOT.move(hipRight, 90);
    BOT.animate(speedms);
}

void walkforward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

  BOT.move(hipLeft, 130);
    BOT.move(hipRight, 40); 
    BOT.animate(speedms);

    for (int i = 0; i < steps; i++){
      BOT.move(ankleLeft, 45);
      BOT.animate(speedms*2);
  
      BOT.move(ankleRight, 135);
      BOT.animate(speedms*2);
  
      BOT.move(ankleLeft, 90);
      BOT.animate(speedms*2);
  
      BOT.move(ankleRight, 90);
      BOT.animate(speedms*2);
    }

    BOT.move(hipLeft, 90);
    BOT.move(hipRight, 90); 
    BOT.animate(speedms);
}

void walkbackward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

   BOT.move(hipLeft, 130);
    BOT.move(hipRight, 40); 
    BOT.animate(speedms);

    for (int i = 0; i < steps; i++){
      BOT.move(ankleLeft, 135);
      BOT.animate(speedms*2);
  
      BOT.move(ankleRight, 45);
      BOT.animate(speedms*2);
  
      BOT.move(ankleLeft, 90);
      BOT.animate(speedms*2);
  
      BOT.move(ankleRight, 90);
      BOT.animate(speedms*2);
    }

    BOT.move(hipLeft, 90);
    BOT.move(hipRight, 90); 
    BOT.animate(speedms);
}

void walkleft(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

  for (int i = 0; i < steps; i++){
    BOT.move(ankleLeft, 45);
    BOT.animate(speedms);

    BOT.move(ankleRight, 135);
    BOT.animate(speedms);

    BOT.move(ankleLeft, 90);
    BOT.animate(speedms);

    BOT.move(ankleRight, 90);
    BOT.animate(speedms);
  }
}

void walkright(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

  for (int i = 0; i < steps; i++){
    BOT.move(ankleRight, 45);
    BOT.animate(speedms);

    BOT.move(ankleLeft, 135);
    BOT.animate(speedms);

    BOT.move(ankleRight, 90);
    BOT.animate(speedms);

    BOT.move(ankleLeft, 90);
    BOT.animate(speedms);
  }
}

void digitalWriteHigh(int argc, int argv[])
{
  int pin = argv[0];

  digitalWrite(pin, HIGH);
}

void digitalWriteLow(int argc, int argv[])
{
  int pin = argv[0];

  digitalWrite(pin, LOW);
}

void analogWriteV(int argc, int argv[])
{
  int pin   = argv[0];
  int value = argv[1];
  
  int actualPin = 0;
  switch (pin)
  {
    case 0: actualPin = A0;
        break;
    case 1: actualPin = A1;
        break;
    case 2: actualPin = A2;
        break;
    case 3: actualPin = A3;
        break;
    case 4: actualPin = A4;
        break;
    case 5: actualPin = A5;
        break;    
    default: return;
  }

  analogWrite(actualPin, value);
}

