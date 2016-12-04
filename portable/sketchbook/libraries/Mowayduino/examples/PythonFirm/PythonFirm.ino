//********************************************************************************************
//*                                    Python RC                                             *
//********************************************************************************************
//*    Python RC                                                                            *
//*    version: 1.0                                                                          *
//*    Cliente para Radio Controlar el robot mOwayduino por medio de Scratch                 *
//*    Copyright (C) 2013  Bizintek Innova S.L.                                              *
//********************************************************************************************
//*    This program is free software; you can redistribute it and/or modify                  *
//*    it under the terms of the GNU General Public License as published by                  *
//*    the Free Software Foundation; either version 2 of the License, or                     *
//*    (at your option) any later version.                                                   *
//*                                                                                          *
//*    This program is distributed in the hope that it will be useful,                       *
//*    but WITHOUT ANY WARRANTY; without even the implied warranty of                        *
//*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                         *
//*    GNU General Public License for more details.                                          *
//*                                                                                          *
//*    You should have received a copy of the GNU General Public License along               *
//*    with this program; if not, write to the Free Software Foundation, Inc.,               *
//*    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                           *
//********************************************************************************************
// Fecha: 23/09/2013
//********************************************************************************************
#include "MowayduinoRobot.h"
#include <SPI.h>
#include <EEPROM.h>

//********************************************************************************************
//	DEFINITIONS
//********************************************************************************************
// Motor
#define DISTANCE  0
#define TIME      1
#define DEFAULT_SPEED  100

// Sensors
#define DIST_MIN  500

#define DISABLE 0
#define ENABLE 1

// Melodies
#define NO_MELODY		0

// Programs
#define NO_PROGRAM		0
#define LINE_FOLLOW_L	1
#define LINE_FOLLOW_R	2
#define ENCLOSED		3
#define DEFENDER		4
#define PUSH			5
//#define PROGRAM_STOP	5

//Canal y direcci�n de mOway
#define moway_address		0x02
#define pc_address			0x01
//Comandos movimiento
#define cmd_go				0xE1
#define cmd_back			0xE2
#define cmd_go_left			0xE3
#define cmd_go_right		0xE4
#define cmd_back_left		0xE5
#define cmd_back_right		0xE6
#define cmd_stop			0xE7
#define cmd_rotate_left		0xE8
#define cmd_rotate_right	0xE9   	
#define cmd_go_simple		0xEA
#define cmd_back_simple		0xEB
#define cmd_left_simple		0xEC
#define cmd_right_simple	0xED
#define cmd_turn_around		0xEE
#define cmd_reset_dist		0xEF

//Comandos LED
#define cmd_led_front_on	0xA0
#define cmd_led_brake_on	0xA1
#define cmd_led_green_on	0xA2
#define cmd_led_red_on		0xA3
#define cmd_led_front_off	0xA4
#define cmd_led_brake_off	0xA5
#define cmd_led_green_off	0xA6
#define cmd_led_red_off		0xA7

#define cmd_led_front_blink	0xA8
#define cmd_led_brake_blink	0xA9
#define cmd_led_green_blink	0xAA
#define cmd_led_red_blink	0xAB
#define cmd_leds_on			0xAC
#define cmd_leds_off		0xAD
#define cmd_leds_blink		0xAE

//Comandos varios
#define cmd_var				0xB0
#define cmd_rst				0xB1
#define cmd_frame_01		0xB5
#define cmd_frame_02		0xB6

// Sonido
#define cmd_buzzer_on		0xC0
#define cmd_buzzer_off		0xC1
#define cmd_melody_charge	0xC2
#define cmd_melody_fail		0xC3

// Funciones
#define cmd_line_follow_l	0x91
#define cmd_line_follow_r	0x92
#define cmd_enclosed		0x93
#define cmd_defender		0x94
#define cmd_push			0x95
	
//**************************************
//  Objects
//**************************************
mowayduinorobot Mowayduino;

//*******************************************************************
//	Variables
//*******************************************************************
volatile unsigned char program;
volatile unsigned char melody;
volatile unsigned char melodyNote;

// RF variables
unsigned char data_in[8];
unsigned char data_in_dir[1];

//Velocidad y radio inicial
volatile unsigned char RC_VEL = 50;
unsigned char RC_RADIO = 20;
unsigned char RC_ROT = 20;
unsigned char RC_TIME = 100;
unsigned char RC_DISTANCE = 20;
unsigned char RC_WHEEL_CENTER = 0;
//unsigned char RC_TIME_DISTANCE = 0;
//unsigned char RC_DISTANCE = 0;
unsigned char RC_TIME_DISTANCE_VAR = 0;
unsigned char RC_FREQ = 250;
unsigned char channel  = 7;

// Interrupts variable
volatile uint8_t oldSREG;
	
//*******************************************************************
//	Function definitions
//*******************************************************************
void Config();
void ReadCommand();

void LineFollow();
void Enclosed();
void Defender();
void Push();

//**************************************
//  RF ISR (necessary for a correct compilation)
//**************************************
void RfDataReady();
void RfDataReady()
{
  while(Mowayduino.Receive(data_in_dir, data_in) == 2);
  ReadCommand();													
}

void Config()
{
  Mowayduino.beginMowayduino();
  Mowayduino.beginRF(channel, moway_address);
  Mowayduino.InterruptsRF(ENABLE);
  Mowayduino.On();
}	

void ReadCommand()
{	
  RC_VEL=data_in[1];
  RC_RADIO=data_in[6];
  RC_ROT=data_in[2];
  RC_TIME=data_in[3];
  RC_DISTANCE=data_in[4];
  RC_WHEEL_CENTER =data_in[5];
  RC_FREQ=data_in[7];
	
  // Motor
  if((data_in[0] >> 4) == 0x0E)          //Comando de movimiento
  {
    // Detener el programa actual (todos afectan al movimiento)
    program = NO_PROGRAM;
  }

  // Movimiento  
  if(data_in[0] == cmd_go)
  {
    if(RC_DISTANCE != 0)    
      Mowayduino.Straight(RC_VEL, RC_DISTANCE);
    else
      Mowayduino.Straight(RC_VEL);    
  }
  else if(data_in[0] == cmd_back)
  {
    if(RC_DISTANCE != 0)
      Mowayduino.Back(RC_VEL, RC_DISTANCE);
    else
      Mowayduino.Back(RC_VEL);
  }
  
  else if(data_in[0] == cmd_stop)            //Stop
    Mowayduino.Stop();  
  else if(data_in[0] == cmd_rotate_right)	// Rotaci�n  Derecha
  {
    if(RC_ROT != 0)
      Mowayduino.TurnRight(RC_VEL, RC_ROT);
    else
      Mowayduino.TurnRight(RC_VEL);
  }
  else if(data_in[0] == cmd_rotate_left)	        // Rotacion  Izquierda
  {
    if(RC_ROT != 0)
      Mowayduino.TurnRight(RC_VEL, RC_ROT);
    else
      Mowayduino.TurnRight(RC_VEL);
  }
  else if (data_in[0] == cmd_go_simple)	        // Avanzar de forma indefinida con velocidad por defecto
    Mowayduino.Straight(100);
  else if (data_in[0] == cmd_back_simple)
    Mowayduino.Back(100);
  else if (data_in[0] == cmd_left_simple)	// Girar 90 grados a la izquierda
    Mowayduino.TurnLeft(100, 90);
  else if (data_in[0] == cmd_right_simple)	// Girar 90 grados a la derecha
    Mowayduino.TurnRight(100, 90);
  else if (data_in[0] == cmd_turn_around)		// Dar media vuelta
    Mowayduino.TurnRight(100, 180);

  //LED
  else if(data_in[0] == cmd_led_front_on)      	//Cambio Led izquierdo
    Mowayduino.Fronton();
  else if(data_in[0] == cmd_led_brake_on)       	//Cambio Led derecho
    Mowayduino.Brakeon();		
  else if(data_in[0] == cmd_led_green_on)   		//Cambio Led verde
    Mowayduino.Greenon();
  else if(data_in[0] == cmd_led_red_on)     		//Cambio Led rojo
    Mowayduino.Redon();
  else if(data_in[0] == cmd_led_front_off)     	//Cambio Led izquierdo
    Mowayduino.Frontoff();
  else if(data_in[0] == cmd_led_brake_off)     	//Cambio Led derecho
    Mowayduino.Brakeoff();
  else if(data_in[0] == cmd_led_green_off)  		//Cambio Led verde
    Mowayduino.Greenoff();
  else if(data_in[0] == cmd_led_red_off)    		//Cambio Led rojo
    Mowayduino.Redoff();	
	
  else if(data_in[0] == cmd_leds_on)
  {
    Mowayduino.Fronton();
    Mowayduino.Brakeon();
    Mowayduino.Greenon();
    Mowayduino.Redon();
  }
  else if(data_in[0] == cmd_leds_off)
  {
    Mowayduino.Ledsoff();
  }
		
  // Sound	
  else if(data_in[0] == cmd_buzzer_on)
    Mowayduino.Speakeron();
  else if(data_in[0] == cmd_buzzer_off)
    Mowayduino.Speakeroff();

  else if(data_in[0] == cmd_rst)          //Reset
  {
    RC_VEL = 100;
    RC_RADIO = 20;
    RC_ROT = 20;
    RC_TIME = 100;
    RC_DISTANCE = 20;
    RC_WHEEL_CENTER = 0;
    //RC_TIME_DISTANCE = 0;
    Mowayduino.Frontoff();
    Mowayduino.Brakeoff();
    Mowayduino.Greenoff();
    Mowayduino.Redoff();
  }
	
//****************************************************************
//	Programs
//****************************************************************		
  else if(data_in[0] == cmd_line_follow_l)	
    program = LINE_FOLLOW_L;
  else if(data_in[0] == cmd_line_follow_r)	
    program = LINE_FOLLOW_R;		
  else if(data_in[0] == cmd_enclosed)	
    program = ENCLOSED;		
  else if(data_in[0] == cmd_defender)	
    program = DEFENDER;
  else if(data_in[0] == cmd_push)	
    program = PUSH;
}  

          

//************************************[MAIN]**************************************************
void setup()
{
  Serial.begin(57600);
  delay(500);
}

void loop()
{   			
  // Initialize hardware and Timer 0
  Config();    
  
  while(1)
  {        
    SendSensors();
    ExecutePrograms();
    delay(50);
    ExecuteLine();
    delay(50);
    ExecuteLine();
    delay(50);
    ExecuteLine();
    delay(50);
  }
}


//*****************************************************************************
//
//	Programs
//
//*****************************************************************************	

//***********************************************************
//	Follow a line (right side or left side)
//***********************************************************
void LineFollow(char side)
{
  if(side == LINE_FOLLOW_L)
  {
    Mowayduino.readLine();
    
    // Over the line    
    if(Mowayduino.Line[LINE_L] == 0 && Mowayduino.Line[LINE_R] == 1)
      Mowayduino.Straight(DEFAULT_SPEED);	

    // Turn right
    else if(Mowayduino.Line[LINE_L] == 0 && Mowayduino.Line[LINE_R] == 0)
      Mowayduino.TurnRight(DEFAULT_SPEED);	

    // Turn left
    else if(Mowayduino.Line[LINE_L] == 1 && Mowayduino.Line[LINE_R] == 1)    
      Mowayduino.TurnLeft(DEFAULT_SPEED);

    // Turn left
    else if(Mowayduino.Line[LINE_L] == 1 && Mowayduino.Line[LINE_R] == 0)    
      Mowayduino.TurnLeft(DEFAULT_SPEED);    
  }
  else if(side == LINE_FOLLOW_R)
  {
    Mowayduino.readLine();
    
    // Over the line    
    if(Mowayduino.Line[LINE_L] == 1 && Mowayduino.Line[LINE_R] == 0)
      Mowayduino.Straight(DEFAULT_SPEED);	

    // Turn right
    else if(Mowayduino.Line[LINE_L] == 1 && Mowayduino.Line[LINE_R] == 1)
      Mowayduino.TurnRight(DEFAULT_SPEED);	

    // Turn left
    else if(Mowayduino.Line[LINE_L] == 0 && Mowayduino.Line[LINE_R] == 0)
      Mowayduino.TurnLeft(DEFAULT_SPEED);

    // Turn right
    else if(Mowayduino.Line[LINE_L] == 0 && Mowayduino.Line[LINE_R] == 1)
      Mowayduino.TurnRight(DEFAULT_SPEED);
  }
}

//***********************************************************
//	Enclosed into a black circle
//***********************************************************
void Enclosed()
{  
  Mowayduino.Straight(DEFAULT_SPEED);
  Mowayduino.readLine();
  if(Mowayduino.Line[LINE_L] == 1 || Mowayduino.Line[LINE_R] == 1)
  {
    // Rotate 144 degrees
    Mowayduino.RotateRight(DEFAULT_SPEED);
    delay(500);
    Mowayduino.Stop();
  }
}

//***********************************************************
//	Detects obstacles and pushes them out of a black circle
//***********************************************************
void Defender()
{
  Mowayduino.Straight(DEFAULT_SPEED);
  Mowayduino.readLine();
  if(Mowayduino.Line[LINE_L] == 1 || Mowayduino.Line[LINE_R] == 1)
  {
    // Rotate 144 degrees
    Mowayduino.RotateRight(DEFAULT_SPEED);
    delay(500);
    Mowayduino.Stop();
    delay(100);
  }
  else
  {
    Mowayduino.readObstacle();
    if(Mowayduino.Obstacle[IR_CL] > DIST_MIN || Mowayduino.Obstacle[IR_CR] > DIST_MIN)    
      return;
    else if(Mowayduino.Obstacle[IR_LR] > DIST_MIN)
    {
      // Rotate 15 degrees
      Mowayduino.RotateRight(DEFAULT_SPEED);
      delay(100);
      Mowayduino.Stop();
    }
    else if(Mowayduino.Obstacle[IR_LL] > DIST_MIN)
    {
      // Rotate 15 degrees
      Mowayduino.RotateLeft(DEFAULT_SPEED);
      delay(100);
      Mowayduino.Stop();
    }	
  }	
}


//**************************************************
//	Detects an obstacle an pushes it
//**************************************************
void Push()
{
  Mowayduino.Straight(DEFAULT_SPEED);
  
  Mowayduino.readObstacle();
  if(Mowayduino.Obstacle[IR_CL] > DIST_MIN || Mowayduino.Obstacle[IR_CR] > DIST_MIN)    
    return;
  else if(Mowayduino.Obstacle[IR_LR] > DIST_MIN)
  {
    // Rotate 15 degrees
    Mowayduino.RotateRight(DEFAULT_SPEED);
    delay(100);
    Mowayduino.Stop();
  }
  else if(Mowayduino.Obstacle[IR_LL] > DIST_MIN)
  {
    // Rotate 15 degrees
    Mowayduino.RotateLeft(DEFAULT_SPEED);
    delay(100);
    Mowayduino.Stop();
  }	
}

//*****************************************************************************
//
//	Auxiliar functions
//
//*****************************************************************************	
void InterruptState(boolean state)
{  
  if(state == ENABLE)
  {
    SREG = oldSREG;
    sei();
  }
  else if(state == DISABLE)
  {
    oldSREG = SREG;
    cli();
  }
}

void ExecutePrograms()
{
  if(program == LINE_FOLLOW_R)
  {        
    InterruptState(DISABLE);
    LineFollow(LINE_FOLLOW_R);
    InterruptState(ENABLE);
  }
  else if(program == LINE_FOLLOW_L)
  {       
    InterruptState(DISABLE);
    LineFollow(LINE_FOLLOW_L);
    InterruptState(ENABLE);
  }
  else if(program == ENCLOSED)
  {
    //InterruptState(DISABLE);
    Enclosed();
    //InterruptState(ENABLE);
  }
  else if(program == DEFENDER)
  {
    //InterruptState(DISABLE);
    Defender();
    //InterruptState(ENABLE);
  }
  else if(program == PUSH)
  {
    //InterruptState(DISABLE);
    Push();
    //InterruptState(ENABLE);
  }
}

//**************************************************
//	Execute follow line functions
//**************************************************
void ExecuteLine()
{
  if(program == LINE_FOLLOW_R)
  {        
    InterruptState(DISABLE);
    LineFollow(LINE_FOLLOW_R);
    InterruptState(ENABLE);
  }
  else if(program == LINE_FOLLOW_L)
  {
    Mowayduino.Greenon();        
    InterruptState(DISABLE);
    LineFollow(LINE_FOLLOW_L);
    InterruptState(ENABLE);
  }
}

//**************************************************
//	Send sensor values
//**************************************************
void SendSensors()
{
  char iMic;
  char ret;
  unsigned char micValue1 = 0;
  unsigned char micValue2 = 0; 
  static unsigned char data_out[8];
  static unsigned char rf_dir_out; 	
  byte accTemp;
  
  // Media de los valores del microfono
  micValue2 = 0;
  for(iMic=0; iMic<5; iMic++)
  {
    micValue1 = Mowayduino.readMic();
    if(micValue1 > micValue2)
      micValue2 = micValue1;
    delay(10);			// Delay 10ms
  }
  
  ExecuteLine();    // Works better if called frequently
				
  // Obstacle sensors
  Mowayduino.readObstacle();
  data_out[0] = Mowayduino.Obstacle[IR_LL];
  data_out[1] = Mowayduino.Obstacle[IR_CL];
  data_out[2] = Mowayduino.Obstacle[IR_CR];
  data_out[3] = Mowayduino.Obstacle[IR_LR];

  // Line sensors
  Mowayduino.readLine();
  data_out[4] = 3 * Mowayduino.Line[LINE_R];  // multiplied by 0.39 in dll
  data_out[5] = 3 * Mowayduino.Line[LINE_L];

  // Frame number
  data_out[7] = cmd_frame_01;	    
  Mowayduino.Send(pc_address, data_out);

  ExecuteLine();    // Works better if called frequently
  
  // Accelerometer
  Mowayduino.readAcc();                
  accTemp = Mowayduino.accel_data[X_ACC];
  if(accTemp >= 0 && accTemp < 128)
    data_out[0] = accTemp + 128;
  else
    data_out[0] = accTemp - 128;
  
  accTemp = Mowayduino.accel_data[Y_ACC];
  if(accTemp >= 0 && accTemp < 128)
    data_out[1] = accTemp + 128;
  else
    data_out[1] = accTemp - 128;

  accTemp = Mowayduino.accel_data[Z_ACC];
  if(accTemp >= 0 && accTemp < 128)
    data_out[2] = accTemp + 128;
  else
    data_out[2] = accTemp - 128;
                             
  // Light sensor
  data_out[5] = Mowayduino.readLight();
		
  // Microphone
  data_out[6] = micValue2;
		
  // Frame number
  data_out[7] = cmd_frame_02;
	
  // Send less critical sensor values		
  Mowayduino.Send(pc_address, data_out); 
}

