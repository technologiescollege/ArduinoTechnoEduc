/* VR412
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
 
#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <SerialALLBOT.h>                       // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

SerialALLBOT BOT(12);                            // Number of motors

enum MotorName {                          // Giving each motor a name
  hipFrontLeft,
  hipFrontRight,
  hipRearLeft,
  hipRearRight,
  kneeFrontLeft,
  kneeFrontRight,
  kneeRearLeft,
  kneeRearRight,
  ankleFrontLeft,
  ankleFrontRight,
  ankleRearLeft,
  ankleRearRight
};

int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

void setup() 
{ 

  // Attach limbs
  BOT.attach(hipFrontLeft,   A5,  45, 0,  0);
  BOT.attach(hipFrontRight,  A2,  45, 1,  0);
  BOT.attach(hipRearLeft,    10,  45, 1,  0);
  BOT.attach(hipRearRight,   3,   45, 0,  0);
  BOT.attach(kneeFrontLeft,  A4,  10, 0,  0);
  BOT.attach(kneeFrontRight, A1,  10, 1,  0);
  BOT.attach(kneeRearLeft,   9,   10, 0,  0);
  BOT.attach(kneeRearRight,  4,   10, 1,  0);
  BOT.attach(ankleFrontLeft,  A0,  0, 0,  0);
  BOT.attach(ankleFrontRight, A3,  0, 1,  0);
  BOT.attach(ankleRearLeft,   11,  0, 0,  0);
  BOT.attach(ankleRearRight,  2,   0, 1,  0);
  
  // Set up commands
  BOT.registerCommand("WF", walkforward);
  BOT.registerCommand("WB", walkbackward);
  BOT.registerCommand("WL", walkleft);
  BOT.registerCommand("WR", walkright);
  BOT.registerCommand("TR", turnright);
  BOT.registerCommand("TL", turnleft);
  BOT.registerCommand("LF", leanforward);
  BOT.registerCommand("LB", leanbackward);
  BOT.registerCommand("LL", leanleft);
  BOT.registerCommand("LR", leanright);
  BOT.registerCommand("FR", wavefrontright);
  BOT.registerCommand("FL", wavefrontleft);
  BOT.registerCommand("RR", waverearright);
  BOT.registerCommand("RL", waverearleft);
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
  
  // Perform the standup sequence
  standup();

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

void standup()
{
  BOT.move(kneeFrontLeft, 110);
  BOT.move(kneeFrontRight, 110);
  BOT.animate(500);

  BOT.move(kneeRearLeft, 110);
  BOT.move(kneeRearRight, 110);
  BOT.animate(500);

  BOT.move(kneeFrontLeft, 90);
  BOT.animate(100);
  BOT.move(ankleFrontLeft, 20);
  BOT.animate(100);
  BOT.move(kneeFrontLeft, 110);
  BOT.animate(100);

  BOT.move(kneeFrontRight, 90);
  BOT.animate(100);
  BOT.move(ankleFrontRight, 20);
  BOT.animate(100);
  BOT.move(kneeFrontRight, 110);
  BOT.animate(100);

  BOT.move(kneeRearLeft, 90);
  BOT.animate(100);
  BOT.move(ankleRearLeft, 20);
  BOT.animate(100);
  BOT.move(kneeRearLeft, 110);
  BOT.animate(100);

  BOT.move(kneeRearRight, 90);
  BOT.animate(100);
  BOT.move(ankleRearRight, 20);
  BOT.animate(100);
  BOT.move(kneeRearRight, 110);
  BOT.animate(100);
}

void chirp(int beeps, int speedms)
{
  for (int i = 0; i < beeps; i++)
  {
    for (int i = 0; i < 255; i++)
    {
      digitalWrite(sounderPin, HIGH);
      delayMicroseconds((355-i)+ (speedms*2));
      digitalWrite(sounderPin, LOW);
      delayMicroseconds((355-i)+ (speedms*2));
    }
    
    delay(30);
  }
}

void chirp(int argc, int argv[])
{
  chirp(argv[0], argv[1]);
}

void waverearleft(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeRearLeft, 150);
  BOT.move(kneeFrontRight, 60);
  BOT.move(ankleFrontRight, 0);
  BOT.animate(speedms);
  
  BOT.move(kneeRearLeft, 0);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleRearLeft, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearLeft, 60);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearLeft, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearLeft, 60);
    BOT.animate(speedms/2);
  }    
  
  BOT.move(kneeRearLeft, 110);
  BOT.move(kneeFrontRight, 110);  
  BOT.move(ankleRearLeft, 20);
  BOT.move(ankleFrontRight, 20);
  BOT.animate(speedms);
}

void waverearright(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeRearRight, 150);
  BOT.move(kneeFrontLeft, 60);
  BOT.move(ankleFrontLeft, 0);
  BOT.animate(speedms);

  BOT.move(kneeRearRight, 0);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleRearRight, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearRight, 60);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearRight, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleRearRight, 60);
    BOT.animate(speedms/2);
  }    
  
  BOT.move(kneeRearRight, 110);
  BOT.move(kneeFrontLeft, 110);  
  BOT.move(ankleRearRight, 20);
  BOT.move(ankleFrontLeft, 20);
  BOT.animate(speedms);
}

void wavefrontright(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeFrontRight, 150);
  BOT.move(kneeRearLeft, 60);
  BOT.move(ankleRearLeft, 0);
  BOT.animate(speedms);

  BOT.move(kneeFrontRight, 0);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleFrontRight, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontRight, 60);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontRight, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontRight, 60);
    BOT.animate(speedms/2);
  }    
  
  BOT.move(kneeFrontRight, 110);
  BOT.move(kneeRearLeft, 110);  
  BOT.move(ankleFrontRight, 20);
  BOT.move(ankleRearLeft, 20);
  BOT.animate(speedms);
}

void wavefrontleft(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeFrontLeft, 150);
  BOT.move(kneeRearRight, 60);
  BOT.move(ankleRearRight, 0);
  BOT.animate(speedms);

  BOT.move(kneeFrontLeft, 0);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleFrontLeft, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontLeft, 60);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontLeft, 90);
    BOT.animate(speedms/2);
    
    BOT.move(ankleFrontLeft, 60);
    BOT.animate(speedms/2);
  }    
  
  BOT.move(kneeFrontLeft, 110);
  BOT.move(kneeRearRight, 110);  
  BOT.move(ankleFrontLeft, 20);
  BOT.move(ankleRearRight, 20);
  BOT.animate(speedms);
}

void scared(int argc, int argv[])
{
  int shakes  = argv[0];
  int beeps   = argv[1];

  for (int i = 0; i < shakes; i++)
  {
    BOT.move(kneeFrontLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeRearRight, 80);
    BOT.move(ankleFrontLeft, 0);
    BOT.move(ankleFrontRight, 0);
    BOT.move(ankleRearLeft, 0);
    BOT.move(ankleRearRight, 0);
    BOT.animate(30);

    BOT.move(kneeFrontLeft, 110);  
    BOT.move(kneeFrontRight, 110); 
    BOT.move(kneeRearLeft, 110);
    BOT.move(kneeRearRight, 110);
    BOT.move(ankleFrontLeft, 20);
    BOT.move(ankleFrontRight, 20);
    BOT.move(ankleRearLeft, 20);
    BOT.move(ankleRearRight, 20);
    BOT.animate(30);
  }
  
  chirp(beeps, 0);
}

void leanright(int argc, int argv[])
{
    int speedms   = argv[0];
    
    BOT.move(kneeFrontRight, 80);
    BOT.move(kneeRearRight, 80);
    BOT.move(ankleFrontRight, 0);
    BOT.move(ankleRearRight, 0);
    BOT.animate(speedms*2);
    
    delay(speedms*3);

    BOT.move(kneeFrontRight, 110);  
    BOT.move(kneeRearRight, 110);
    BOT.move(ankleFrontRight, 20);
    BOT.move(ankleRearRight, 20);
    BOT.animate(speedms*2);
}

void leanleft(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeFrontLeft, 80);
  BOT.move(kneeRearLeft, 80);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(ankleRearLeft, 0);
  BOT.animate(speedms*2);
  
  delay(speedms*3);

  BOT.move(kneeFrontLeft, 110);  
  BOT.move(kneeRearLeft, 110);
  BOT.move(ankleFrontLeft, 20);
  BOT.move(ankleRearLeft, 20);
  BOT.animate(speedms*2);
}

void leanforward(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeFrontLeft, 80);
  BOT.move(kneeFrontRight, 80);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(ankleFrontRight, 0);
  BOT.animate(speedms*2);
  
  delay(speedms*3);

  BOT.move(kneeFrontLeft, 110);
  BOT.move(kneeFrontRight, 110);
  BOT.move(ankleFrontLeft, 20);
  BOT.move(ankleFrontRight, 20);
  BOT.animate(speedms*2);
}

void leanbackward(int argc, int argv[])
{
  int speedms = argv[0];
  BOT.move(kneeRearLeft, 80);
  BOT.move(kneeRearRight, 80);
  BOT.move(ankleRearLeft, 0);
  BOT.move(ankleRearRight, 0);
  BOT.animate(speedms*2);
  
  delay(speedms*3);

  BOT.move(kneeRearLeft, 110);
  BOT.move(kneeRearRight, 110);
  BOT.move(ankleRearLeft, 20);
  BOT.move(ankleRearRight, 20);
  BOT.animate(speedms*2);
}

void lookleft(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(hipRearLeft, 80);
  BOT.move(hipRearRight, 10);
  BOT.move(hipFrontLeft, 10);
  BOT.move(hipFrontRight, 80);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(hipRearRight, 45);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontRight, 45);
  BOT.move(hipFrontLeft, 45);
  BOT.animate(speedms);
}

void lookright(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(hipRearRight, 80);
  BOT.move(hipRearLeft, 10);
  BOT.move(hipFrontRight, 10);
  BOT.move(hipFrontLeft, 80);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(hipRearRight, 45);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontRight, 45);
  BOT.move(hipFrontLeft, 45);
  BOT.animate(speedms);
}

void walkforward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  { 
    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearLeft, 45);
    BOT.animate(speedms);
  }
}

void walkbackward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);

    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
  }
}

void walkleft(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);

    BOT.move(hipRearLeft, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
  }
}

void walkright(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(hipRearLeft, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
  }
}

void turnleft(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
  }
}

void turnright(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeRearRight, 80);
    BOT.move(hipRearRight, 10);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 110);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 80);
    BOT.move(hipRearLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 110);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 80);
    BOT.move(hipFrontLeft, 10);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 110);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 80);
    BOT.move(hipFrontRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 110);
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipRearRight, 45);
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
