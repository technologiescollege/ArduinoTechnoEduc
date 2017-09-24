/*VR618
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
 
#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <ALLBOT.h>                       // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

ALLBOT BOT(18);                            // Number of motors

enum MotorName {                           // Giving each motor a name
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

  // Perform the standup sequence
  standup();

    //INIT sounder
  pinMode(sounderPin, OUTPUT);
  
  //wait for joints to be initialized
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
//--------------------------------------------------------------
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
          randNumber0 = random(50, 255);
          leanforward(randNumber0);
        break;
        case 1:
          randNumber0 = random(50, 255);
          leanbackward(randNumber0);
        break;
        case 2:
          randNumber0 = random(50, 255);
          leanleft(randNumber0);
        break;
        case 3:
          randNumber0 = random(50, 255);
          leanright(randNumber0);
        break;
        case 4:
          randNumber0 = random(50, 255);
          //lookleft(randNumber0);
        break;
        case 5:
          randNumber0 = random(50, 255);
          //lookright(randNumber0);
        break;
        case 6:
          randNumber0 = random(50, 255);
          randNumber1 = random(1, 5);
          turnleft(randNumber1, randNumber0);
        break;
        case 7:
          randNumber0 = random(50, 255);
          randNumber1 = random(1, 5);
          turnright(randNumber1, randNumber0);
        break;
        case 8:
          randNumber0 = random(50, 150);
          randNumber1 = random(2, 10);
          walkforward(randNumber1, randNumber0);
        break;
        case 9:
          randNumber0 = random(50, 150);
          randNumber1 = random(2, 10);
          walkbackward(randNumber1, randNumber0);
        break;
        case 10:
          randNumber0 = random(50, 150);
          randNumber1 = random(2, 10);
          //walkleft(randNumber1, randNumber0);
        break;
        case 11:
          randNumber0 = random(50, 150);
          randNumber1 = random(2, 10);
          //walkright(randNumber1, randNumber0);
        break;
        case 12:
          randNumber0 = random(5, 30);
          randNumber1 = random(2, 10);
          //scared(randNumber1, randNumber0);
        break;
        case 13:
          randNumber0 = random(0, 255);
          randNumber1 = random(1, 30);
          chirp(randNumber1, randNumber0);
        break;
        case 14:
          randNumber0 = random(50, 100);
          randNumber1 = random(1, 5);
          wavefrontright(randNumber1, randNumber0);
        break;
        case 15:
          randNumber0 = random(50, 100);
          randNumber1 = random(1, 5);
          wavefrontleft(randNumber1, randNumber0);
        break;
        case 16:
          randNumber0 = random(50, 100);
          randNumber1 = random(1, 5);
          waverearright(randNumber1, randNumber0);
        break;
        case 17:
          randNumber0 = random(50, 100);
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
void turnright (int steps, int speedms)
{
  for (int i = 0; i < steps; i++){
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
//********************************************************************************************************************
void turnleft (int steps, int speedms)
{
  for (int i = 0; i < steps; i++){
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
//********************************************************************************************************************
void leanright (int speedms)
{
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
//********************************************************************************************************************
void leanleft (int speedms)
{
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
//********************************************************************************************************************
void wavefrontright (int waves, int speedms){

  BOT.move(kneeFrontRight, 150);
  BOT.move(ankleFrontRight, 75);
  BOT.move(hipFrontRight, 10);
  BOT.animate(speedms);
  
  for (int i = 0; i < waves; i++){
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
//********************************************************************************************************************
void wavefrontleft (int waves, int speedms){

  BOT.move(kneeFrontLeft, 150);
  BOT.move(ankleFrontLeft, 75);
  BOT.move(hipFrontLeft, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++){
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
//********************************************************************************************************************
void waverearleft (int waves, int speedms){

  BOT.move(kneeRearLeft, 150);
  BOT.move(ankleRearLeft, 75);
  BOT.move(hipRearLeft, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++){
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
//********************************************************************************************************************
void waverearright (int waves, int speedms){

  BOT.move(kneeRearRight, 150);
  BOT.move(ankleRearRight, 75);
  BOT.move(hipRearRight, 10);
  BOT.animate(speedms);

  for (int i = 0; i < waves; i++){
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
//********************************************************************************************************************
void standup ()
{
  BOT.move(kneeFrontRight, 100);
  BOT.move(kneeFrontLeft, 100);
  BOT.move(kneeMiddleRight, 100);
  BOT.move(kneeMiddleLeft, 100);
  BOT.move(kneeRearRight, 100);
  BOT.move(kneeRearLeft, 100);
  BOT.animate(200);
}
//********************************************************************************************************************
void walkforward(int steps, int speedms)
{
  BOT.move(kneeFrontRight, 130);    //Lift 3a
  BOT.move(kneeRearRight, 130);
  BOT.move(kneeMiddleLeft, 130);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 20);    //Swivel 3a forward
  BOT.move(hipRearRight, 70);
  BOT.move(hipMiddleLeft, 65);
  BOT.animate(speedms);
  
  for (int i = 0; i < steps; i++){
  
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
//********************************************************************************************************************
void walkbackward(int steps, int speedms)
{
  BOT.move(kneeFrontRight, 130);    //Lift 3a
  BOT.move(kneeRearRight, 130);
  BOT.move(kneeMiddleLeft, 130);
  BOT.animate(speedms);
  
  BOT.move(hipFrontRight, 70);    //Swivel 3a backward
  BOT.move(hipRearRight, 20);
  BOT.move(hipMiddleLeft, 115);
  BOT.animate(speedms);
  
  for (int i = 0; i < steps; i++){
  
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
void leanbackward(int speedms)
{  
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
//********************************************************************************************************************
void leanforward(int speedms)
{  
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
