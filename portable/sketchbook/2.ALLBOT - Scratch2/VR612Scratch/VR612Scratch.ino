/*VR612
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
 
#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <SerialALLBOT.h>                 // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

SerialALLBOT BOT(12);                     // Create a SerialALLBOT with 12 motors

enum MotorName {                          // Giving each motor a name
  hipFrontLeft,
  kneeFrontLeft,
  hipFrontRight,
  kneeFrontRight,
  hipMiddleRight,
  kneeMiddleRight,
  hipRearRight,
  kneeRearRight,
  hipRearLeft,
  kneeRearLeft,
  hipMiddleLeft,
  kneeMiddleLeft
};

int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

void setup()
{
  
  // Attach limbs
  BOT.attach(hipFrontLeft,    A1,  45, 0,  0);
  BOT.attach(hipMiddleLeft,   10,  90, 0,  0);
  BOT.attach(hipRearLeft,      9,  45, 1,  0);
  BOT.attach(kneeFrontLeft,   11,  20, 1,  0);
  BOT.attach(kneeMiddleLeft,   8,  20, 1,  0);
  BOT.attach(kneeRearLeft,     7,  20, 1,  0); 
  BOT.attach(hipFrontRight,   A0,  45, 1,  0);
  BOT.attach(hipMiddleRight,   3,  90, 1,  0);
  BOT.attach(hipRearRight,     4,  45, 0,  0);
  BOT.attach(kneeFrontRight,   2,  20, 0,  0);      
  BOT.attach(kneeMiddleRight,  5,  20, 0,  0);
  BOT.attach(kneeRearRight,    6,  20, 0,  0);

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

void scared(int argc, int argv[])
{
  int shakes  = argv[0];
  int beeps   = argv[1];
  
  BOT.move(kneeFrontRight, 0);
  BOT.move(kneeMiddleRight, 0);
  BOT.move(kneeRearRight, 0);
  BOT.move(kneeFrontLeft, 0);
  BOT.move(kneeMiddleLeft, 0);
  BOT.move(kneeRearLeft, 0); 
  BOT.animate(50);
  
  for (int i = 0; i < shakes; i++)
  {
    BOT.move(hipRearRight, 80);
    BOT.move(hipMiddleRight, 65);
    BOT.move(hipRearLeft, 5);
    BOT.move(hipFrontRight, 5);
    BOT.move(hipMiddleLeft, 115);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(100);
    
    BOT.move(hipRearLeft, 80);
    BOT.move(hipMiddleRight, 115);
    BOT.move(hipRearRight, 5);
    BOT.move(hipFrontLeft, 5);
    BOT.move(hipMiddleLeft, 65);
    BOT.move(hipFrontRight, 80);
    BOT.animate(50);
  }
  
  BOT.move(hipRearRight, 45);
  BOT.move(hipMiddleRight, 90);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontRight, 45);
  BOT.move(hipMiddleLeft, 90);
  BOT.move(hipFrontLeft, 45);
  BOT.animate(200);
  
  chirp(beeps, 0);
  
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeMiddleLeft, 20);
  BOT.move(kneeRearLeft, 20); 
  BOT.animate(75);
}

void turnleft(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.move(kneeMiddleRight, 50);    
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 80);
    BOT.move(hipRearLeft, 5);
    BOT.move(hipMiddleRight, 65);    
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.move(kneeMiddleRight, 20);    
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearRight, 50);
    BOT.move(kneeMiddleLeft, 50);    
    BOT.animate(speedms);

    BOT.move(hipFrontRight, 5);
    BOT.move(hipRearRight, 80);
    BOT.move(hipMiddleLeft, 115);   
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearRight, 20);
    BOT.move(kneeMiddleLeft, 20);    
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);    
    BOT.animate(speedms);
  }
}

void turnright(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.move(kneeMiddleRight, 50);    
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 5);
    BOT.move(hipRearLeft, 80);
    BOT.move(hipMiddleRight, 115);    
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.move(kneeMiddleRight, 20);    
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearRight, 50);
    BOT.move(kneeMiddleLeft, 50);    
    BOT.animate(speedms);

    BOT.move(hipFrontRight, 80);
    BOT.move(hipRearRight, 5);
    BOT.move(hipMiddleLeft, 65);    
    BOT.animate(speedms);

    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearRight, 20);
    BOT.move(kneeMiddleLeft, 20);    
    BOT.animate(speedms);

    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);    
    BOT.animate(speedms);
  }
}

void waverearleft(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];
  
  BOT.move(kneeRearLeft, 180);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(hipRearLeft, 0);
    BOT.animate(speedms/2);
    
    BOT.move(hipRearLeft, 65);
    BOT.animate(speedms/2);
    
    BOT.move(hipRearLeft, 0);
    BOT.animate(speedms/2);
    
    BOT.move(hipRearLeft, 45);
    BOT.animate(speedms/2);
  }    
  
  BOT.move(kneeRearLeft, 20);
  BOT.animate(speedms);
}

void waverearright(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];
  
  BOT.move(kneeRearRight, 180);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(hipRearRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 65);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
  }    
  
  BOT.move(kneeRearRight, 20);
  BOT.animate(speedms);
}

void wavefrontright(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeFrontRight, 180);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 65);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
  }    

  BOT.move(kneeFrontRight, 20);
  BOT.animate(speedms);
}

void wavefrontleft(int argc, int argv[])
{
  int waves   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeFrontLeft, 180);
  BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++)
  {
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 65);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
  }    
  
  BOT.move(kneeFrontLeft, 20);
  BOT.animate(speedms);
}

void walkright(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearRight, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontLeft, 10);
    BOT.move(hipRearRight, 80);
    BOT.animate(speedms);
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearRight, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontRight, 80);
    BOT.move(hipRearLeft, 10);
    BOT.animate(speedms);
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeMiddleRight, 50);
    BOT.move(kneeMiddleLeft, 50);
    BOT.animate(speedms/2);
  
    BOT.move(hipFrontLeft, 75);
    BOT.move(hipFrontRight, 15);
    BOT.move(hipRearLeft, 75);
    BOT.move(hipRearRight, 15);
    BOT.animate(speedms); 
    
    BOT.move(kneeMiddleRight, 20);
    BOT.move(kneeMiddleLeft, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearRight, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearRight, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.animate(speedms);
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.animate(speedms/2);
  }
}

void walkleft(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  for (int i = 0; i < steps; i++)
  {
    
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearRight, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontLeft, 80);
    BOT.move(hipRearRight, 10);
    BOT.animate(speedms);
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearRight, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontRight, 10);
    BOT.move(hipRearLeft, 80);
    BOT.animate(speedms);
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeMiddleRight, 50);
    BOT.move(kneeMiddleLeft, 50);
    BOT.animate(speedms/2);
  
    BOT.move(hipFrontLeft, 15);
    BOT.move(hipFrontRight, 75);
    BOT.move(hipRearLeft, 15);
    BOT.move(hipRearRight, 75);
    BOT.animate(speedms); 
    
    BOT.move(kneeMiddleRight, 20);
    BOT.move(kneeMiddleLeft, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearRight, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearRight, 20);
    BOT.animate(speedms/2);
    
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.animate(speedms/2);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.animate(speedms);
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.animate(speedms/2);
  }
}

void walkbackward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];
  
  BOT.move(kneeFrontLeft, 50);
  BOT.move(kneeRearLeft, 50);
  BOT.move(hipFrontLeft, 15);
  BOT.move(hipFrontRight, 75);
  
  BOT.move(hipFrontLeft, 80);
  BOT.move(hipRearLeft, 5);
  BOT.move(hipMiddleRight, 115);
  BOT.animate(speedms);
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontLeft, 10);
    BOT.move(kneeRearLeft, 10);
    BOT.move(kneeMiddleRight, 10);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeRearRight, 50);
    BOT.move(kneeMiddleLeft, 50);
    BOT.animate(speedms);//
      
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeRearLeft, 20);
    BOT.move(kneeMiddleRight, 20);
    BOT.move(hipFrontLeft, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.move(hipFrontRight, 80);
    BOT.move(hipRearRight, 5);
    BOT.move(hipMiddleLeft, 115);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 10);
    BOT.move(kneeRearRight, 10);
    BOT.move(kneeMiddleLeft, 10);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.move(kneeMiddleRight, 50);
    BOT.animate(speedms);//
    
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeRearRight, 20);
    BOT.move(kneeMiddleLeft, 20);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);
    BOT.move(hipFrontLeft, 80);
    BOT.move(hipRearLeft, 5);
    BOT.move(hipMiddleRight, 115);
    BOT.animate(speedms);
  }
  
  BOT.move(kneeFrontLeft, 10);
  BOT.move(kneeRearLeft, 10);
  BOT.move(kneeMiddleRight, 10);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 50);
  BOT.move(kneeRearRight, 50);
  BOT.move(kneeMiddleLeft, 50);
  BOT.animate(speedms);//
  
  BOT.move(hipFrontLeft, 45);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipMiddleRight, 90);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeMiddleLeft, 20);
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeRearLeft, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.animate(speedms);//
}

void walkforward(int argc, int argv[])
{
  int steps   = argv[0];
  int speedms = argv[1];

  BOT.move(kneeRearLeft, 50);
  BOT.move(kneeFrontLeft, 50);
  BOT.move(kneeMiddleRight, 50);
  BOT.animate(speedms); //
  
  BOT.move(hipRearLeft, 80);
  BOT.move(hipFrontLeft, 5);
  BOT.move(hipMiddleRight, 65);
  BOT.animate(speedms);//
  
  for (int i = 0; i < steps; i++)
  {
    
    BOT.move(kneeRearLeft, 10);
    BOT.move(kneeFrontLeft, 10);
    BOT.move(kneeMiddleRight, 10);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 50);
    BOT.move(kneeFrontRight, 50);
    BOT.move(kneeMiddleLeft, 50);
    BOT.animate(speedms);//
      
    BOT.move(kneeRearLeft, 20);
    BOT.move(kneeFrontLeft, 20);
    BOT.move(kneeMiddleRight, 20);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.move(hipRearRight, 80);
    BOT.move(hipFrontRight, 5);
    BOT.move(hipMiddleLeft, 65);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 10);
    BOT.move(kneeFrontRight, 10);
    BOT.move(kneeMiddleLeft, 10);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 50);
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeMiddleRight, 50);
    BOT.animate(speedms);//
    
    BOT.move(kneeRearRight, 20);
    BOT.move(kneeFrontRight, 20);
    BOT.move(kneeMiddleLeft, 20);
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipMiddleLeft, 90);
    BOT.move(hipRearLeft, 80);
    BOT.move(hipFrontLeft, 5);
    BOT.move(hipMiddleRight, 65);
    BOT.animate(speedms);
  }
  
  BOT.move(kneeRearLeft, 10);
  BOT.move(kneeFrontLeft, 10);
  BOT.move(kneeMiddleRight, 10);
  BOT.animate(speedms);
  
  BOT.move(kneeRearRight, 50);
  BOT.move(kneeFrontRight, 50);
  BOT.move(kneeMiddleLeft, 50);
  BOT.animate(speedms);//
  
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontLeft, 45);
  BOT.move(hipMiddleRight, 90);
  BOT.animate(speedms);
  
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeMiddleLeft, 20);
  BOT.move(kneeRearLeft, 20);
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.animate(speedms);//
}

void leanforward(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeFrontLeft, 90);
  BOT.move(kneeFrontRight, 90);
  BOT.move(kneeMiddleRight, 50);
  BOT.move(kneeMiddleLeft, 50);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.move(kneeMiddleLeft, 20);
  
  BOT.animate(speedms);
}

void leanbackward(int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeRearLeft, 90);
  BOT.move(kneeRearRight, 90);
  BOT.move(kneeMiddleRight, 50);
  BOT.move(kneeMiddleLeft, 50);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(kneeRearLeft, 20);
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.move(kneeMiddleLeft, 20);
  
  BOT.animate(speedms);
}

void leanright(int argc, int argv[])
{
  int speedms = argv[0];  
  
  BOT.move(kneeRearRight, 10);
  BOT.move(kneeFrontRight, 10);
  BOT.move(kneeMiddleRight, 0);
  BOT.move(kneeRearLeft, 90);
  BOT.move(kneeFrontLeft, 90);
  BOT.move(kneeMiddleLeft, 90);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.move(kneeRearLeft, 20);
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeMiddleLeft, 20);
  
  BOT.animate(speedms);
}

void leanleft(int argc, int argv[])
{
  int speedms = argv[0];  

  BOT.move(kneeRearRight, 90);
  BOT.move(kneeFrontRight, 90);
  BOT.move(kneeMiddleRight, 90);
  BOT.move(kneeRearLeft, 10);
  BOT.move(kneeFrontLeft, 10);
  BOT.move(kneeMiddleLeft, 0);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(kneeRearRight, 20);
  BOT.move(kneeFrontRight, 20);
  BOT.move(kneeMiddleRight, 20);
  BOT.move(kneeRearLeft, 20);
  BOT.move(kneeFrontLeft, 20);
  BOT.move(kneeMiddleLeft, 20);
  
  BOT.animate(speedms);
}

void lookleft(int argc, int argv[])
{
  int speedms = argv[0]; 
  
  BOT.move(hipRearLeft, 80);
  BOT.move(hipRearRight, 10);
  BOT.move(hipFrontLeft, 10);
  BOT.move(hipFrontRight, 80);
  BOT.move(hipMiddleRight, 125);
  BOT.move(hipMiddleLeft, 65);
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(hipRearRight, 45);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontRight, 45);
  BOT.move(hipFrontLeft, 45);
  BOT.move(hipMiddleRight, 90);
  BOT.move(hipMiddleLeft, 90);
  BOT.animate(speedms);
}

void lookright(int argc, int argv[])
{
  int speedms = argv[0]; 
  
  BOT.move(hipRearRight, 80);
  BOT.move(hipRearLeft, 10);
  BOT.move(hipFrontRight, 10);
  BOT.move(hipFrontLeft, 80);
  BOT.move(hipMiddleRight, 65);
  BOT.move(hipMiddleLeft, 125);    
  BOT.animate(speedms);
  
  delay(speedms/2);
  
  BOT.move(hipRearRight, 45);
  BOT.move(hipRearLeft, 45);
  BOT.move(hipFrontRight, 45);
  BOT.move(hipFrontLeft, 45);
  BOT.move(hipMiddleRight, 90);
  BOT.move(hipMiddleLeft, 90);
  BOT.animate(speedms);
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
    case 6: actualPin = A6;
        break;
    case 7: actualPin = A7;
        break;
    case 8: actualPin = A8;
        break;
    case 9: actualPin = A9;
        break;
    case 10: actualPin = A10;
        break;
    case 11: actualPin = A11;
        break;    
    case 12: actualPin = A12;
        break;    
    case 13: actualPin = A13;
        break;    
    case 14: actualPin = A14;
        break;    
    case 15: actualPin = A15;
        break;
  }

  analogWrite(actualPin, value);
}
