/* VR618
 * Copyright (C) 2016 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <SerialALLBOT.h>                 // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

SerialALLBOT BOT(18);                     // Create a SerialALLBOT

enum MotorName {                          
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
  kneeMiddleLeft,
  ankleFrontRight,
  ankleFrontLeft,
  ankleMiddleRight,
  ankleMiddleLeft,
  ankleRearRight,
  ankleRearLeft
};

int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

///
// Setup
///
void setup()
{
  
  // Attach limbs
  BOT.attach(hipFrontRight,      31,   45,     1,      0);     //SV10
  BOT.attach(hipFrontLeft,       24,   45,     0,      0);     //SV03
  BOT.attach(hipMiddleRight,     48,   90,     1,      0);     //SV27
  BOT.attach(hipMiddleLeft,      38,   90,     0,      0);     //SV17
  BOT.attach(hipRearRight,       49,   45,     0,      0);     //SV28
  BOT.attach(hipRearLeft,        36,   45,     1,      0);     //SV15
  BOT.attach(kneeFrontRight,     32,   135,    1,      0);     //SV11
  BOT.attach(kneeFrontLeft,      23,   135,    0,      0);     //SV02
  BOT.attach(kneeMiddleRight,    47,   135,    1,      0);     //SV26
  BOT.attach(kneeMiddleLeft,     37,   135,    0,      0);     //SV16
  BOT.attach(kneeRearRight,      50,   135,    1,      0);     //SV29
  BOT.attach(kneeRearLeft,       35,   135,    0,      0);     //SV14 
  BOT.attach(ankleFrontRight,    33,   0,      0,      0);     //SV12
  BOT.attach(ankleFrontLeft,     22,   0,      1,      0);     //SV01
  BOT.attach(ankleMiddleRight,   46,   0,      0,      0);     //SV25
  BOT.attach(ankleMiddleLeft,    39,   0,      1,      0);     //SV18
  BOT.attach(ankleRearRight,     51,   0,      0,      0);     //SV30
  BOT.attach(ankleRearLeft,      34,   0,      1,      0);     //SV13

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

void turnright (int argc, int argv[])
{
  int steps   = argv[0];  
  int speedms = argv[1]; 
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontRight, 130);    //Lift 3a
    BOT.move(kneeRearRight, 130);
    BOT.move(kneeMiddleLeft, 130);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 70);    //turn 3a right
    BOT.move(hipRearRight, 20);
    BOT.move(hipMiddleLeft, 65);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 100);  //Drop 3a
    BOT.move(kneeRearRight, 100);
    BOT.move(kneeMiddleLeft, 100);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 130);    //Lift 3b
    BOT.move(kneeRearLeft, 130);
    BOT.move(kneeMiddleRight, 130);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 20);    //turn 3b right
    BOT.move(hipRearLeft, 70);
    BOT.move(hipMiddleRight, 115);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 100);      //Drop 3b + small ankle movement
    BOT.move(kneeRearLeft, 100);
    BOT.move(kneeMiddleRight, 100);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);    //Swivel 3a&b back to center
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);
    BOT.move(hipFrontLeft, 45);    //turn 3b right
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.animate(speedms);
  }
}

void turnleft (int argc, int argv[])
{
  int steps   = argv[0];  
  int speedms = argv[1]; 
  
  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontRight, 130);    //Lift 3a
    BOT.move(kneeRearRight, 130);
    BOT.move(kneeMiddleLeft, 130);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 20);    //turn 3a left
    BOT.move(hipRearRight, 70);
    BOT.move(hipMiddleLeft, 115);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 100);  //Drop 3a
    BOT.move(kneeRearRight, 100);
    BOT.move(kneeMiddleLeft, 100);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 130);    //Lift 3b
    BOT.move(kneeRearLeft, 130);
    BOT.move(kneeMiddleRight, 130);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 70);    //turn 3b left
    BOT.move(hipRearLeft, 20);
    BOT.move(hipMiddleRight, 65);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 100);      //Drop 3b + small ankle movement
    BOT.move(kneeRearLeft, 100);
    BOT.move(kneeMiddleRight, 100);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);    //Swivel 3a&b back to center
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);
    BOT.move(hipFrontLeft, 45);    //turn 3b right
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    BOT.animate(speedms);
  }
}

void leanright (int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeFrontRight, 50);
  BOT.move(kneeMiddleRight, 50);
  BOT.move(kneeRearRight, 50);
  BOT.move(ankleFrontRight, 50);
  BOT.move(ankleMiddleRight, 50);
  BOT.move(ankleRearRight, 50);
  //
  BOT.move(kneeFrontLeft, 150);
  BOT.move(kneeMiddleLeft, 150);
  BOT.move(kneeRearLeft, 150);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(ankleMiddleLeft, 0);
  BOT.move(ankleRearLeft, 0);
  BOT.animate(speedms);
  //
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeRearRight, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleMiddleRight, 0);
  BOT.move(ankleRearRight, 0);
  //
  BOT.move(kneeFrontLeft, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(ankleMiddleLeft, 0);
  BOT.move(ankleRearLeft, 0);
  BOT.animate(speedms);
}

void leanleft (int argc, int argv[])
{
  int speedms = argv[0];
  
  BOT.move(kneeFrontLeft, 50);
  BOT.move(kneeMiddleLeft, 50);
  BOT.move(kneeRearLeft, 50);
  BOT.move(ankleFrontLeft, 50);
  BOT.move(ankleMiddleLeft, 50);
  BOT.move(ankleRearLeft, 50);
  //
  BOT.move(kneeFrontRight, 150);
  BOT.move(kneeMiddleRight, 150);
  BOT.move(kneeRearRight, 150);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleMiddleRight, 0);
  BOT.move(ankleRearRight, 0);
  BOT.animate(speedms);
  //
  BOT.move(kneeFrontLeft, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(ankleMiddleLeft, 0);
  BOT.move(ankleRearLeft, 0);
  //
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeRearRight, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleMiddleRight, 0);
  BOT.move(ankleRearRight, 0);
  BOT.animate(speedms);
}

void wavefrontright(int argc, int argv[])
{
  int waves   = argv[0];  
  int speedms = argv[1]; 

  BOT.move(kneeFrontRight, 150);
  BOT.move(ankleFrontRight, 75);
  BOT.move(hipFrontRight, 10);
  BOT.animate(speedms);
  
  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleFrontRight, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontRight, 90);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontRight, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontRight, 90);
    BOT.animate(speedms/1.5);
    BOT.move(ankleFrontRight, 45);
    BOT.animate(speedms/1.5);
  }
  
  BOT.move(kneeFrontRight, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(hipFrontRight, 45);
  BOT.animate(speedms);
}

void wavefrontleft(int argc, int argv[])
{
  int waves   = argv[0];  
  int speedms = argv[1]; 
  
  BOT.move(kneeFrontLeft, 150);
  BOT.move(ankleFrontLeft, 75);
  BOT.move(hipFrontLeft, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleFrontLeft, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontLeft, 90);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontLeft, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleFrontLeft, 90);
    BOT.animate(speedms/1.5);
    BOT.move(ankleFrontLeft, 45);
    BOT.animate(speedms/1.5);
 }
  
  BOT.move(kneeFrontLeft, 100);
  BOT.move(ankleFrontLeft, 0);
  BOT.move(hipFrontLeft, 45);
  BOT.animate(speedms);
}

void waverearleft(int argc, int argv[])
{
  int waves   = argv[0];  
  int speedms = argv[1]; 
  
  BOT.move(kneeRearLeft, 150);
  BOT.move(ankleRearLeft, 75);
  BOT.move(hipRearLeft, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleRearLeft, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleRearLeft, 90);
    BOT.animate(speedms/2);
    BOT.move(ankleRearLeft, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleRearLeft, 90);
    BOT.animate(speedms/1.5);
    BOT.move(ankleRearLeft, 45);
    BOT.animate(speedms/1.5);
  }
  
  BOT.move(kneeRearLeft, 100);
  BOT.move(ankleRearLeft, 0);
  BOT.move(hipRearLeft, 45);
  BOT.animate(speedms);
}

void waverearright(int argc, int argv[])
{
  int waves   = argv[0];  
  int speedms = argv[1]; 

  BOT.move(kneeRearRight, 150);
  BOT.move(ankleRearRight, 75);
  BOT.move(hipRearRight, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++)
  {
    BOT.move(ankleRearRight, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleRearRight, 90);
    BOT.animate(speedms/2);
    BOT.move(ankleRearRight, 45);
    BOT.animate(speedms/2);
    BOT.move(ankleRearRight, 90);
    BOT.animate(speedms/1.5);
    BOT.move(ankleRearRight, 45);
    BOT.animate(speedms/1.5);
  }
  
  BOT.move(kneeRearRight, 100);
  BOT.move(ankleRearRight, 0);
  BOT.move(hipRearRight, 45);
  BOT.animate(speedms);
}

void standup()
{
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeFrontLeft, 100);
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.animate(200);
}

void walkforward(int argc, int argv[])
{
  int steps   = argv[0];  
  int speedms = argv[1]; 
  
  BOT.move(kneeFrontRight, 130);    //Lift 3a
  BOT.move(kneeRearRight, 130);
  BOT.move(kneeMiddleLeft, 130);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 20);    //Swivel 3a forward
  BOT.move(hipRearRight, 70);
  BOT.move(hipMiddleLeft, 65);
  BOT.animate(speedms);

  for (int i = 0; i < steps; i++)
  {
    BOT.move(kneeFrontRight, 70);  //Drop 3a + small ankle movement
    BOT.move(kneeRearRight, 70);
    BOT.move(kneeMiddleLeft, 70);
    BOT.move(ankleFrontRight, 20);
    BOT.move(ankleRearRight, 20);
    BOT.move(ankleMiddleLeft, 20);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);    //Swivel 3a back to center
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);
    //
    BOT.move(kneeFrontLeft, 130);    //Lift 3b
    BOT.move(kneeRearLeft, 130);
    BOT.move(kneeMiddleRight, 130);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 100);    //Normalize 3a
    BOT.move(kneeRearRight, 100);
    BOT.move(kneeMiddleLeft, 100);
    BOT.move(ankleFrontRight, 0);
    BOT.move(ankleRearRight, 0);
    BOT.move(ankleMiddleLeft, 0);
    //
    BOT.move(hipFrontLeft, 20);    //Swivel 3b forward
    BOT.move(hipRearLeft, 70);
    BOT.move(hipMiddleRight, 65);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 70);      //Drop 3b + small ankle movement
    BOT.move(kneeRearLeft, 70);
    BOT.move(kneeMiddleRight, 70);
    BOT.move(ankleFrontLeft, 20);
    BOT.move(ankleRearLeft, 20);
    BOT.move(ankleMiddleRight, 20);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 45);    //Swivel 3b back to center
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    //
    BOT.move(kneeFrontRight, 130);    //Lift 3a
    BOT.move(kneeRearRight, 130);
    BOT.move(kneeMiddleLeft, 130);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 100);    //Normalize 3b
    BOT.move(kneeRearLeft, 100);
    BOT.move(kneeMiddleRight, 100);
    BOT.move(ankleFrontLeft, 0);
    BOT.move(ankleRearLeft, 0);
    BOT.move(ankleMiddleRight, 0);
    //
    BOT.move(hipFrontRight, 20);    //Swivel 3a forward
    BOT.move(hipRearRight, 70);
    BOT.move(hipMiddleLeft, 65);
    BOT.animate(speedms);
  }
  
  BOT.move(kneeFrontRight, 70);  //Drop 3a + small ankle movement
  BOT.move(kneeRearRight, 70);
  BOT.move(kneeMiddleLeft, 70);
  BOT.move(ankleFrontRight, 20);
  BOT.move(ankleRearRight, 20);
  BOT.move(ankleMiddleLeft, 20);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 45);    //Swivel 3a back to center
  BOT.move(hipRearRight, 45);
  BOT.move(hipMiddleLeft, 90);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 100);    //Normalize 3a
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleRearRight, 0);
  BOT.move(ankleMiddleLeft, 0);
  BOT.animate(speedms);
}

void walkbackward(int argc, int argv[])
{
  int steps   = argv[0];  
  int speedms = argv[1]; 
  
  BOT.move(kneeFrontRight, 130);    //Lift 3a
  BOT.move(kneeRearRight, 130);
  BOT.move(kneeMiddleLeft, 130);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 70);    //Swivel 3a backward
  BOT.move(hipRearRight, 20);
  BOT.move(hipMiddleLeft, 115);
  BOT.animate(speedms);
  
  for (int i = 0; i < steps; i++)
  {
  
    BOT.move(kneeFrontRight, 70);  //Drop 3a + small ankle movement
    BOT.move(kneeRearRight, 70);
    BOT.move(kneeMiddleLeft, 70);
    BOT.move(ankleFrontRight, 20);
    BOT.move(ankleRearRight, 20);
    BOT.move(ankleMiddleLeft, 20);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);    //Swivel 3a back to center
    BOT.move(hipRearRight, 45);
    BOT.move(hipMiddleLeft, 90);
    //
    BOT.move(kneeFrontLeft, 130);    //Lift 3b
    BOT.move(kneeRearLeft, 130);
    BOT.move(kneeMiddleRight, 130);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontRight, 100);    //Normalize 3a
    BOT.move(kneeRearRight, 100);
    BOT.move(kneeMiddleLeft, 100);
    BOT.move(ankleFrontRight, 0);
    BOT.move(ankleRearRight, 0);
    BOT.move(ankleMiddleLeft, 0);
    //
    BOT.move(hipFrontLeft, 70);    //Swivel 3b backward
    BOT.move(hipRearLeft, 20);
    BOT.move(hipMiddleRight, 115);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 70);      //Drop 3b + small ankle movement
    BOT.move(kneeRearLeft, 70);
    BOT.move(kneeMiddleRight, 70);
    BOT.move(ankleFrontLeft, 20);
    BOT.move(ankleRearLeft, 20);
    BOT.move(ankleMiddleRight, 20);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 45);    //Swivel 3b back to center
    BOT.move(hipRearLeft, 45);
    BOT.move(hipMiddleRight, 90);
    //
    BOT.move(kneeFrontRight, 130);    //Lift 3a
    BOT.move(kneeRearRight, 130);
    BOT.move(kneeMiddleLeft, 130);
    BOT.animate(speedms);
    
    BOT.move(kneeFrontLeft, 100);    //Normalize 3b
    BOT.move(kneeRearLeft, 100);
    BOT.move(kneeMiddleRight, 100);
    BOT.move(ankleFrontLeft, 0);
    BOT.move(ankleRearLeft, 0);
    BOT.move(ankleMiddleRight, 0);
    //
    BOT.move(hipFrontRight, 70);    //Swivel 3a backward
    BOT.move(hipRearRight, 20);
    BOT.move(hipMiddleLeft, 115);
    BOT.animate(speedms);
  }
  
  BOT.move(kneeFrontRight, 70);  //Drop 3a + small ankle movement
  BOT.move(kneeRearRight, 70);
  BOT.move(kneeMiddleLeft, 70);
  BOT.move(ankleFrontRight, 20);
  BOT.move(ankleRearRight, 20);
  BOT.move(ankleMiddleLeft, 20);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 45);    //Swivel 3a back to center
  BOT.move(hipRearRight, 45);
  BOT.move(hipMiddleLeft, 90);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 100);    //Normalize 3a
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleRearRight, 0);
  BOT.move(ankleMiddleLeft, 0);
  BOT.animate(speedms);
}
//********************************************************************************************************************
void leanbackward(int argc, int argv[])
{  
  int speedms = argv[0];
  
  BOT.move(kneeFrontRight, 50);
  BOT.move(kneeFrontLeft, 50);
  BOT.move(ankleFrontRight, 80);
  BOT.move(ankleFrontLeft, 80);
  //
  BOT.move(kneeRearRight, 135);
  BOT.move(kneeRearLeft, 135);
  BOT.move(ankleRearRight, 0);
  BOT.move(ankleRearLeft, 0);
  //
  BOT.move(kneeMiddleRight, 110);
  BOT.move(kneeMiddleLeft, 110);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeFrontLeft, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleFrontLeft, 0);
  //
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.move(ankleRearRight, 0);
  BOT.move(ankleRearLeft, 0);
  //
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.animate(speedms);
}

void leanforward(int argc, int argv[])
{  
  int speedms = argv[0];
  
  BOT.move(kneeRearRight, 50);
  BOT.move(kneeRearLeft, 50);
  BOT.move(ankleRearRight, 80);
  BOT.move(ankleRearLeft, 80);
  //
  BOT.move(kneeFrontRight, 135);
  BOT.move(kneeFrontLeft, 135);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleFrontLeft, 0);
  //
  BOT.move(kneeMiddleRight, 110);
  BOT.move(kneeMiddleLeft, 110);
  BOT.animate(speedms);
  
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeFrontLeft, 100);
  BOT.move(ankleFrontRight, 0);
  BOT.move(ankleFrontLeft, 0);
  //
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.move(ankleRearRight, 0);
  BOT.move(ankleRearLeft, 0);
  //
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeMiddleLeft, 100);
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
