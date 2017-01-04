
/*
  Pap1.cpp - Bibliothèque : Controleur de deux moteurs Pas à Pas PAP1.
  _________
  Rév : 1.00 
  Christophe Grosse 26 Juin , 2014.
  Le code est du domaine public.
*/
 
#include <Wire.h>
#include "Pap1.h"

#ifdef __AVR__
  #include <avr/pgmspace.h>
  #define WIRE Wire
#else
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define WIRE Wire1
#endif
 
#if (ARDUINO >= 100)
  #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
   // Macro to deal with the difference in I2C write functions from old and new Arduino versions.
  #define _I2C_WRITE write
  #define _I2C_READ read
#else
  #include <WProgram.h>
  #define _I2C_WRITE send
  #define _I2C_READ receive
#endif
 
struct splitLong {
  union {
    long value;
    char split[4];
  }__attribute__((packed));
};

struct splitLong myLong;

int _add_device;
 
Pap1::Pap1(int add)
{
  //Fixe l'adresse I2C du controleur
  _add_device = add;
}

void Pap1::Begin(int stepping_mode,int relax)
{
  switch (stepping_mode) { 
  case 0 : //Mode Single Stepping
           Set_Registre(29,0);
           break;
  case 1 : //Mode full stepping
           Set_Registre(29,1);
           break;
  default ://Mode half stepping
            Set_Registre(29,3);    
            break;
  } //switch
  switch (relax) {
  case 0 : //Autorelease actif
            Set_Registre(28,0);
            break;
  default : //Autorelease non actif
            Set_Registre(28,1);
            break;          
  }//switch 
}

void Pap1::SetMode(int mde)
{
  switch (mde) {
   case 0 : //Iddle mode
            Set_Registre(4,0);
            break;
   case 1 : //CC Mode
            Set_Registre(4,1);
            break;
   case 2 : //Relative mode
            Set_Registre(4,2);
            break;         
   default : //absolute mode
            Set_Registre(4,3);
            break;         
  }//switch
}

void Pap1::Set_Registre(int reg,int data)
{
  WIRE.beginTransmission(_add_device);
  WIRE._I2C_WRITE(reg);
  WIRE._I2C_WRITE(data);
  WIRE.endTransmission();
  delayMicroseconds(1000);  
} 
 
void Pap1::SpeedA(int speed)
{
  Set_Registre(1,speed); 
}

void Pap1::SpeedB(int speed)
{
  Set_Registre(3,speed);
}

void Pap1::DirA(int dir)
{
 switch (dir) {
  case 0 : //conterclockwise
            Set_Registre(0,255);
            break;
  case 1 : //clockwise
            Set_Registre(0,0);
            break;
 }//switch
}

void Pap1::DirB(int dir)
{
 switch (dir) {
  case 0 : //conterclockwise
            Set_Registre(2,255);
            break;
  case 1 : //clockwise
            Set_Registre(2,0);
            break;
 }//switch
}

void Pap1::StepA(int step)
{
  Set_Registre(6,highByte(step));
  Set_Registre(7,lowByte(step));
}

void Pap1::StepB(int step)
{
  Set_Registre(8,highByte(step));
  Set_Registre(9,lowByte(step));
}

void Pap1::OrigineA()
{
  Set_Registre(10,0);
  Set_Registre(11,0);
  Set_Registre(12,0);
  Set_Registre(13,0);
}

void Pap1::OrigineB()
{
  Set_Registre(14,0);
  Set_Registre(15,0);
  Set_Registre(16,0);
  Set_Registre(17,0);
}

void Pap1::TargetA(long target)
{
  myLong.value = target;
  Set_Registre(18,byte(myLong.split[3]));
  Set_Registre(19,byte(myLong.split[2]));
  Set_Registre(20,byte(myLong.split[1]));
  Set_Registre(21,byte(myLong.split[0]));
}

void Pap1::TargetB(long target)
{
  myLong.value = target;
  Set_Registre(22,byte(myLong.split[3]));
  Set_Registre(23,byte(myLong.split[2]));
  Set_Registre(24,byte(myLong.split[1]));
  Set_Registre(25,byte(myLong.split[0]));
}

bool Pap1::Busy()
{
  byte rep;
  WIRE.beginTransmission(_add_device);
  WIRE._I2C_WRITE(5);
  WIRE.endTransmission();
  delayMicroseconds(500);
  WIRE.requestFrom(_add_device, 1);
  delayMicroseconds(500);
  rep=WIRE._I2C_READ();
  WIRE.endTransmission();
  delayMicroseconds(500);
  if (rep==0) {
    return true;
  } 
  else  {
    return false;
  }  
} 