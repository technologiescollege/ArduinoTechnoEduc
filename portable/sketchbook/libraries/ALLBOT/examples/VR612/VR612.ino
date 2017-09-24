/*VR612
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
 
#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <ALLBOT.h>                       // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

ALLBOT BOT(12);                            // Number of motors

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

long randNumber0;                         // This variable is used for random function
long randNumber1;                         // This variable is used for random function
int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

String rawcommand;                        // Global variable that stores the raw received IR command
String command;                           // Global variable that stores part of the decoded IR command
int times;                                // Global variable that stores part the received IR command
int speedms;                              // Global variable that stores part the received IR command

boolean IRreceive = false;                 // Set this to true if you want to use the IR remote
boolean receivelog = false;               // Set this to true if you want to see the serial messages for debugging the IR commands

void setup()
{
  //BOT.attach(motorname, pin, init. angle, flipped, offset);
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
  
  //INIT sounder
  pinMode(sounderPin, OUTPUT);

  // Wait for joints to be initialized
  delay(500);

  // Starting the hardware UART, necessary for receiving IR
  if (IRreceive == true)                  // Check if required (when Serial is started servo1 connector will not work!)
    {
      Serial.begin(2400);
      Serial.setTimeout(100);
      Serial.println("serial communication started");
    }
  
  // INIT the random seed, this is used to create random actions
  randomSeed(analogRead(5));

  // Chirp for ready
  chirp(1, 50);
  chirp(1, 255);
  chirp(3, 0);

}

void loop()                               // Main program loop
{
  if (IRreceive == true)                  // Choose between IR commands or random action
    {
      getcommand();                       // Listen for IR command
      executecommand();                   // Execute any receveid commands
    }
    else
    {
      randNumber0 = random(0, 18);
      randNumber1 = random(0, 20);
      delay(randNumber1*100);
      
      switch (randNumber0) {
        case 0:
          randNumber0 = random(25, 255);
          leanforward(randNumber0);
        break;
        case 1:
          randNumber0 = random(25, 255);
          leanbackward(randNumber0);
        break;
        case 2:
          randNumber0 = random(25, 255);
          leanleft(randNumber0);
        break;
        case 3:
          randNumber0 = random(25, 255);
          leanright(randNumber0);
        break;
        case 4:
          randNumber0 = random(25, 255);
          lookleft(randNumber0);
        break;
        case 5:
          randNumber0 = random(25, 255);
          lookright(randNumber0);
        break;
        case 6:
          randNumber0 = random(25, 255);
          randNumber1 = random(1, 5);
          turnleft(randNumber1, randNumber0);
        break;
        case 7:
          randNumber0 = random(25, 255);
          randNumber1 = random(1, 5);
          turnright(randNumber1, randNumber0);
        break;
        case 8:
          randNumber0 = random(25, 150);
          randNumber1 = random(2, 10);
          walkforward(randNumber1, randNumber0);
        break;
        case 9:
          randNumber0 = random(25, 150);
          randNumber1 = random(2, 10);
          walkbackward(randNumber1, randNumber0);
        break;
        case 10:
          randNumber0 = random(25, 150);
          randNumber1 = random(2, 10);
          walkleft(randNumber1, randNumber0);
        break;
        case 11:
          randNumber0 = random(25, 150);
          randNumber1 = random(2, 10);
          walkright(randNumber1, randNumber0);
        break;
        case 12:
          randNumber0 = random(5, 30);
          randNumber1 = random(2, 10);
          scared(randNumber1, randNumber0);
        break;
        case 13:
          randNumber0 = random(0, 255);
          randNumber1 = random(1, 30);
          chirp(randNumber1, randNumber0);
        break;
        case 14:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          wavefrontright(randNumber1, randNumber0);
        break;
        case 15:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          wavefrontleft(randNumber1, randNumber0);
        break;
        case 16:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          waverearright(randNumber1, randNumber0);
        break;
        case 17:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          waverearleft(randNumber1, randNumber0);
        break;
      }
   }   
}
//--------------------------------------------------------------
void getcommand (void)                    // This is the routine that listens and decodes any IR commands. Decodes commands end up in the global vars.
{ 
  int space1 = 0;
  int space2 = 0;
  
  if (Serial.available()) {
     rawcommand = Serial.readString();
     if (receivelog){
        Serial.println("START " + rawcommand + " END" + "\r\n" + "Received string length = " + rawcommand.length() + "\r\n" + "End character > at index = " + rawcommand.indexOf('>'));
     }

     //checking and deleting rubbish data at start of received command
     if ((rawcommand.indexOf('<') != 0) && (rawcommand.indexOf('<') != -1))
     {
        rawcommand.remove(0, rawcommand.indexOf('<'));
     }
     
     //check if received command is correct
     if ((rawcommand.charAt(0) == '<') && (rawcommand.indexOf('>') <= 12) && (rawcommand.indexOf('>') != -1) && (rawcommand.length() > 7))
     {
       if (receivelog){
         Serial.println("Command is VALID"); 
       }      
       //breakdown into chunks
       //command
       command = rawcommand.substring(1, 3);
       
       //finding the spaces to find the times and speedms
       for (int i=0; i <= rawcommand.length(); i ++)
       {
         if ((rawcommand.charAt(i) == ' ') && (space1 == 0))
         {
            space1 = i;
         }
         else if ((rawcommand.charAt(i) == ' ') && (space2 == 0))
         {
            space2 = i;
         }
       }

       //Setting the command variables and checking if they are indeed a number (toInt()).
       
       //times
       times = rawcommand.substring(space1+1, space2).toInt();
       
       //speedms
       speedms = rawcommand.substring(space2+1, rawcommand.indexOf('>')).toInt();

       if (receivelog){
         Serial.println("decoded commands are:");
         Serial.flush();
         Serial.println("command = " + command);
         Serial.flush();
         Serial.println("times = " + times);
         Serial.flush();
         Serial.println("speedms = " + speedms);
       }
       
     }
     else
     {
       if (receivelog){
          Serial.println("Command is NOT valid");
       }
       resetserial();  
     }
  }
}
//--------------------------------------------------------------
void resetserial (void)                  // This clears any received IR commands that where received in the serial buffer while the robot was execution a command.
{
  //resetting all variables
  rawcommand = "";
  command = "";
  times = 0;
  speedms = 0;
  
  //flushing the serial buffer (64 byte) so there are no stored movements that need to be handled (annoying)...
  while (Serial.available()) {
    Serial.read();
  }
}
//--------------------------------------------------------------
void executecommand (void)                // Execute the commands that are stored in the global vars.)
{ 
  if (command == "WF")
  {
    walkforward(times, (speedms*5));
    resetserial();
  }
  else if (command == "WB")
  {
    walkbackward(times, (speedms*5));
    resetserial();
  }
  else if (command == "TR")
  {
    turnright(times, (speedms*5));
    resetserial();
  }
  else if (command == "TL")
  {
    turnleft(times, (speedms*5));
    resetserial();
  }
  else if (command == "LF")
  {
    leanforward(speedms*5);
    resetserial();
  }
  else if (command == "LB")
  {
    leanbackward(speedms*5);
    resetserial();
  }
  else if (command == "LL")
  {
    leanleft(speedms*5);
    resetserial();
  }
  else if (command == "LR")
  {
    leanright(speedms*5);
    resetserial();
  }
  else if (command == "FR")
  {
    wavefrontright(times, speedms*5);
    resetserial();
  }
  else if (command == "FL")
  {
    wavefrontleft(times, speedms*5);
    resetserial();
  }
  else if (command == "RR")
  {
    waverearright(times, speedms*5);
    resetserial();
  }
  else if (command == "RL")
  {
    waverearleft(times, speedms*5);
    resetserial();
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
void scared(int shakes, int beeps){
    BOT.move(kneeFrontRight, 0);
    BOT.move(kneeMiddleRight, 0);
    BOT.move(kneeRearRight, 0);
    BOT.move(kneeFrontLeft, 0);
    BOT.move(kneeMiddleLeft, 0);
    BOT.move(kneeRearLeft, 0); 
    BOT.animate(50);
    
    for (int i = 0; i < shakes; i++){
 
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
//--------------------------------------------------------------
void turnleft(int steps, int speedms){

  for (int i = 0; i < steps; i++){

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
//--------------------------------------------------------------
void turnright(int steps, int speedms){

  for (int i = 0; i < steps; i++){

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
//--------------------------------------------------------------
void waverearleft(int waves, int speedms){

    BOT.move(kneeRearLeft, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
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
//--------------------------------------------------------------
void waverearright(int waves, int speedms){

    BOT.move(kneeRearRight, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
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
  
    BOT.move(kneeFrontRight, 20);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void wavefrontleft(int waves, int speedms){

    BOT.move(kneeFrontLeft, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
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
//--------------------------------------------------------------
void walkright(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    
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
//--------------------------------------------------------------
void walkleft(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    
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
//--------------------------------------------------------------
void walkbackward(int steps, int speedms){
  
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeRearLeft, 50);
    BOT.move(hipFrontLeft, 15);
    BOT.move(hipFrontRight, 75);
    
    BOT.move(hipFrontLeft, 80);
    BOT.move(hipRearLeft, 5);
    BOT.move(hipMiddleRight, 115);
    BOT.animate(speedms);//
  
  for (int i = 0; i < steps; i++){
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
//--------------------------------------------------------------
void walkforward(int steps, int speedms){
  
    BOT.move(kneeRearLeft, 50);
    BOT.move(kneeFrontLeft, 50);
    BOT.move(kneeMiddleRight, 50);
    BOT.animate(speedms); //
    
    BOT.move(hipRearLeft, 80);
    BOT.move(hipFrontLeft, 5);
    BOT.move(hipMiddleRight, 65);
    BOT.animate(speedms);//
  
  for (int i = 0; i < steps; i++){
    
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
//--------------------------------------------------------------
void leanforward(int speedms){
  
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
//--------------------------------------------------------------
void leanbackward(int speedms){
  
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
//--------------------------------------------------------------
void leanright(int speedms){
  
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
//--------------------------------------------------------------
void leanleft(int speedms){
  
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
//--------------------------------------------------------------
void lookleft(int speedms){
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
//--------------------------------------------------------------
void lookright(int speedms){
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
