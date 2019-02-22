/**
  ******************************************************************************
  * @file    mrduino.h
  * @author  Mace Robotics
  * @version 0.21
  * @date    08/06/2017
  * @brief   lib for MRduino robot an MRduino Wireless
  *
 *******************************************************************************/

#ifndef _MRduino_H
#define _MRduino_H

void initRobot();

void led(int led, int state);
void ledToggle(int led);

float firmwareVersion();


void forward(int speed);
void back(int speed);
void turnLeft(int speed);
void turnRight(int speed);

int proxSensor(int number);
int ambiantLight(int number);
int groundSensor(int number);

void controlEnable();
void controlDisable();

int readSwitch();

int irReceiver();

float battery();
float temperature();

// motor
void motorRight(int speed, int direction);
void motorLeft(int speed, int direction);

// move
void forwardC(int speed, int distance);
void forward_mm(int speed, int distance);
void backC(int speed, int distance);
void back_mm(int speed, int distance);
void turnRightC(int speed, int distance);
void turnLeftC(int speed, int distance);
void stop();
void robotGo(int speed, int coord_X, int coord_Y);

// turn degree
void turnRight_degree(int speed, unsigned int angle);
void turnLeft_degree(int speed, unsigned int angle);
void turn_degree(int speed, unsigned int angle);

// position
float robotPositionX();
float robotPositionY();
float robotPositionOrientation();


// encoder
int encoderLeft();
int encoderRight();

int readData();
float readFloatData();

// accelerometer
float acceleroX();
float acceleroY();
float acceleroZ();

//speaker
void speakerEnable();

#endif