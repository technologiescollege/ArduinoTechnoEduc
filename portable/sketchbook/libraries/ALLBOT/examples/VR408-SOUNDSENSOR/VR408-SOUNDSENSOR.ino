/*VR408
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * This program will make your ALLBOT wave when it hears a loud noise like a clap
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

int soundsensor = A5;                     // The soundsensor needs to be connected to the A5 servo port or A0 -> A5
int soundsensorvalue;                     // The measured value will be stored in this variable

int soundsensortreshold = 450;            // The threshold value must be between 0 an 650 (lower and the ALLBOT will react to less loud noises, higher and it will react to louder noises.) 


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
soundsensorvalue = analogRead(soundsensor);       // Measure the temperature 
  if (soundsensorvalue > soundsensortreshold)     // Check if bigger than threshold value (you may need to change this value)
  {
    wavefrontright(1, 100);                                // Put your action here that needs to be executed when the robot "feels hot"
  }
  else
  {
                                                         // Put your action here that needs to be executed when the robot "feels cold"
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
void wavefrontright(int waves, int speedms){

    BOT.move(kneeFrontRight, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 65);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
  }    
  
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
