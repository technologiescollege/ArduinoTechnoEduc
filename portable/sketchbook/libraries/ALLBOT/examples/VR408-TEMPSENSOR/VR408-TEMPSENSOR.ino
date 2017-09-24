/*VR408
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * This program will make your ALLBOT Beep fast when it "feels hot" and slow when it "feels cold"
 *
 */
 
#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <ALLBOT.h>                       // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

ALLBOT BOT(8);                            // Number of motors

enum MotorName {                          
  hipFrontLeft,
  hipFrontRight,
  hipRearLeft,
  hipRearRight,
  kneeFrontLeft,
  kneeFrontRight,
  kneeRearLeft,
  kneeRearRight
};


int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

int tempsensor = A5;                     // The tempsensor needs to be connected to the A5 servo port or A0 -> A5
int tempsensorvalue;                     // The measured value will be stored in this variable

int tempsensortreshold = 450;            // The threshold value must be between 0 an 650 (lower wil be "cold", higher will be "warm") 


void setup() 
{ 
  // NAME.attach(motorname, pin, init-angle, flipped, offset-angle);
  BOT.attach(hipFrontLeft,   A1,  45, 0, 0);
  BOT.attach(hipFrontRight,  A0,  45, 1, 0);
  BOT.attach(hipRearLeft,     9,  45, 1, 0);
  BOT.attach(hipRearRight,    4,  45, 0, 0);
 
  BOT.attach(kneeFrontLeft,  11,  45, 1, 0);
  BOT.attach(kneeFrontRight,  2,  45, 0, 0);
  BOT.attach(kneeRearLeft,   10,  45, 1, 0);
  BOT.attach(kneeRearRight,   3,  45, 0, 0);

  // INIT sounder
  pinMode(sounderPin, OUTPUT);

  // Wait for joints to be initialized
  delay(500);

  // Chirp for ready
  chirp(1, 50);
  chirp(1, 255);
  chirp(3, 0);

}

void loop()                                     // Main program loop
{
tempsensorvalue = analogRead(tempsensor);       // Measure the temperature 
  if (tempsensorvalue > tempsensortreshold)     // Check if bigger than threshold value (you may need to change this value)
  {
    chirp(3, 0);                                // Put your action here that needs to be executed when the robot "feels hot"
  }
  else
  {
    chirp(1, 255);                              // Put your action here that needs to be executed when the robot "feels cold"
  }
}
//--------------------------------------------------------------
void chirp(int beeps, int speedms){

  for (int i = 0; i < beeps; i++){
    for (int i = 0; i < 255; i++){
      digitalWrite(sounderPin, HIGH);
      delayMicroseconds((355-i)+ (speedms*2));
      digitalWrite(sounderPin, LOW);
      delayMicroseconds((355-i)+ (speedms*2));
    }
     delay(30);
  }
}
//--------------------------------------------------------------
