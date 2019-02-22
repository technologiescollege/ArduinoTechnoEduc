/**
  ******************************************************************************
  * @file    mrduino2.cpp
  * @author  Mace Robotics (www.macerobotics.com)
  * @Licence MIT Licence
  * @version 0.52
  * @date    13/02/2019
  * @brief   lib for MRduino2 robot
  *
 *******************************************************************************/


#include <Arduino.h>
#include <math.h>
#include <assert.h>
#include "mrduino2.h"



#define M_2PI M_PI*2
#define ERR_DIS_SAT 40

// coefficient PID distance
#define K_PID_R 2
#define K_PID_L 2

// coefficient PID orientation
#define KO_PID_R 600
#define KO_PID_L 600

// maximum speed
#define MAX_SPEED 50
#define DIST_MINI 5

static void forwardControl(int speed, int distance);
static void backControl(int speed, int distance);
static void turnRightControl(int speed, int angle);
static void turnLeftControl(int speed, int angle);
static boolean  state_control = false;
static void controlEnable();
static void controlDisable();
static void forwardC(int speed, int distance);
static void backC(int speed, int distance);
static void turnRightC(int speed, int distance);
static void turnLeftC(int speed, int distance);
static float readFloatData();
static int readData();
static int saturationCmd(int cmd, int maxSpeed);

static bool s_ledRight = false;
static bool s_ledLeft = false;
unsigned int timeout=0;


void initRobot()
{
  Serial.begin(115200);

  delay(0.5);
}


/**********************************************************
 * @brief  firmwareVersion
 * @param  None
 * @retval None
**********************************************************/
float firmwareVersion()
{
String  commande;
 
  commande = "#FV!";
  Serial.println(commande); 
  
  return(readFloatData());

}


/**********************************************************
 * @brief  battery
 * @param  None
 * @retval None
**********************************************************/
float battery()
{
String  commande;
 
  commande = "#BAT!";
  Serial.println(commande); 
  
  return(readFloatData());
}



/**********************************************************
 * @brief  readSwitch
 * @param  None
 * @retval None
**********************************************************/
int readSwitch()
{
String  commande;
 
  commande = "#SW!";
  Serial.println(commande); 
  
  return(readData());

}



/**********************************************************
 * @brief  motorRight
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void motorRight(int speed, int direction)
{
String  commande;

  controlDisable();
  commande = "#MOTR," + String(direction) + "," + String(speed) + "!";
  Serial.println(commande); 
}


/**********************************************************
 * @brief  motorRight
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void motorLeft(int speed, int direction)
{
String  commande;

  controlDisable();
  commande = "#MOTL," + String(direction) + "," + String(speed) + "!";
  Serial.println(commande); 
}


/**********************************************************
 * @brief  motorsDisable
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void motorsDisable(void)
{
String  commande;

  commande = "#MDI!";
  Serial.println(commande);
}


/**********************************************************
 * @brief  motorsEnable
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void motorsEnable(void)
{
String  commande;

  commande = "#MEN!";
  Serial.println(commande);
}


/**********************************************************
 * @brief  motorsEnable
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void motorsBrake(void)
{
String  commande;

    
  commande = "#BRK!";
  Serial.println(commande);
}


/**********************************************************
 * @brief  pinMode
 * @param  
 * @retval None
**********************************************************/
void MRpinMode(int pin, uint8_t mode)
{
String  commande;
 
  if(mode == OUTPUT)
  {
	 commande = "#PMOD," + String(pin) + "," + String(0) + "!"; 
  }
  else
  {
	 commande = "#PMOD," + String(pin) + "," + String(1) + "!";  
  }
  
  Serial.println(commande);
}


/**********************************************************
 * @brief  MRpinWrite
 * @param  
 * @retval None
**********************************************************/
void MRpinWrite(int pin, int state)
{
String  commande;

  commande = "#PINW," + String(pin) + "," + String(state) + "!";
  Serial.println(commande);
}


/**********************************************************
 * @brief  MRpinRead
 * @param  
 * @retval None
**********************************************************/
bool MRpinRead(int pin)
{
String  commande;

  commande = "#PINR," + String(pin) + "!";
  Serial.println(commande);
  
  return(readData());
}


/**********************************************************
 * @brief  resetUc
 * @param  
 * @retval None
**********************************************************/
void resetUc()
{
String  commande;

  commande = "#RST!";
  Serial.println(commande);
  delay(1000);
}


/**********************************************************
 * @brief  forward
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void forward(int speed)
{
String  commande;

  controlDisable();
  
  commande = "#MF," + String(speed) + "!";
  Serial.println(commande); 
  
 }


/**********************************************************
 * @brief  forward_mm (forward with control, pulling function)
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void forward_mm(int speed, int distance)
{
  controlEnable();
  forwardC(speed, distance*4);
}


/**********************************************************
 * @brief  forwardmm (forward with control, no pulling function
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void forwardmm(int speed, int distance)
{
String  commande;
int  state = 0;
static bool fmm = false;
 

    controlEnable();
    commande = "#MFC," + String(distance*4) + "," + String(speed) + "!";
    Serial.println(commande);



  
}

/**********************************************************
 * @brief  back_mm (forward with control) - pulling function
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void back_mm(int speed, int distance)
{
  controlEnable();
  backC(speed, distance*4);
  //controlDisable();
}


/**********************************************************
 * @brief  backmm (back with control, no pulling function
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void backmm(int speed, int distance)
{
String  commande;
int  state = 0;
 
  controlEnable();
  commande = "#MBC," + String(distance*4) + "," + String(speed) + "!";
  Serial.println(commande); 

  
}


/**********************************************************
 * @brief  back
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void back(int speed)
{
String  commande;

  controlDisable();
  commande = "#MB," + String(speed) + "!";
  Serial.println(commande); 
  
}

/**********************************************************
 * @brief  turnLeft
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnLeft(int speed)
{  
String  commande;

  controlDisable();
  commande = "#TL," + String(speed) + "!";
  Serial.println(commande); 
 }


/**********************************************************
 * @brief  turnRight_degree
 * @param  speed ( 0 to 100 ), angle (0 to 360)
 * @retval None
**********************************************************/
void turnRight_degree(int speed, unsigned int angle)
{
float angle_degree = 0;
int angle_turn;

  controlEnable();

  if((angle >= 0)&&(angle <= 360))
  {
    angle_degree = (float)(angle*546.0);
    angle_degree = (float)(angle_degree/90.0);
    angle_turn = (int)(angle_degree);
 
    turnRightC(speed, angle_turn); 
  }
  else
  {
    // error angle value
  }
  
  controlDisable();
}


/**********************************************************
 * @brief  turnLeft_degree
 * @param  speed ( 0 to 100 ), angle (0 to 360)
 * @retval None
**********************************************************/
void turnLeft_degree(int speed, unsigned int angle)
{
float angle_degree = 0;
int angle_turn;
  
  controlEnable();
  
  if((angle >= 0)&&(angle <= 360))
  {
    angle_degree = (float)(angle*546.0);
    angle_degree = (float)(angle_degree/90.0);
    angle_turn = (int)(angle_degree);
 
    turnLeftC(speed, angle_turn);
  }
  else
  {
    // error angle value
  }
  
  controlDisable();
}


/**********************************************************
 * @brief  turnRight
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnRight(int speed)
{
String  commande;
 
  controlDisable();
  commande = "#TR," + String(speed) + "!";
  Serial.println(commande); 

}


/**********************************************************
 * @brief  proxSensor
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
int proxSensor()
{
String  commande;
int data;
 
  commande = "#PROX,3!";
  Serial.println(commande); 

  data = readData();
  
  // pour resoudre bug au demarrage du capteur, il envoie des valeurs de 0 ou -1.
  if(data < 5)
  {
    data  = 255;
  }
  
  delay(100);
  
  return data;
}


/**********************************************************
 * @brief  stop
 * @param  
 * @retval None
**********************************************************/
void stop()
{
  
  if(state_control == true)
  {
    controlDisable();  
  }
  Serial.println("#STP!"); 
}


/**********************************************************
 * @brief read encoder left
 * @param  
 * @retval None
**********************************************************/
int encoderLeft()
{
String  commande;
int data;
 
  commande = "#EDL!";
  Serial.println(commande); 

  data = readData();
  
  return data;
}


/**********************************************************
 * @brief read encoder right
 * @param  
 * @retval None
**********************************************************/
int encoderRight()
{
String  commande;
int data;
 
  commande = "#EDR!";
  Serial.println(commande); 

  data = readData();
  
  return data;
}










/**********************************************************
 * @brief  turn_degree
 *                   90°
 *                 |
 * @param  180°<--- ----> 0° (0 to 360°)
 * @retval None
**********************************************************/
void turn_degree(int speed, unsigned int angle)
{
float angle_Robot;
float angle_Consigne;

  // read actual robot position
  angle_Robot = robotPositionOrientation();
  
  // conversion en degree
  angle_Robot = (angle_Robot*180.0)/(3.14);
  
  angle_Consigne = angle - angle_Robot;
  
  if(angle >= angle_Robot)
  {
	   turnLeft_degree(10,abs(angle_Consigne)); 
  }
  else
  {
	   turnRight_degree(10,abs(angle_Consigne));  
  }
  

}


/**********************************************************
 * @brief  robotPositionX
 * @param  return millimeter 
 * @retval None
**********************************************************/
float robotPositionX()
{
String  commande;
 
  commande = "#POX!";
  Serial.println(commande); 
  
  return(readFloatData()/4);

}

/**********************************************************
 * @brief  robotPositionY
 * @param  return millimeter 
 * @retval None
**********************************************************/
float robotPositionY()
{
String  commande;
 
  commande = "#POY!";
  Serial.println(commande); 
  
  return(-readFloatData()/4);

}


/**********************************************************
 * @brief  robotPositionOrientation
 * @param  None
 * @retval None
**********************************************************/
float robotPositionOrientation()
{
String  commande;
 
  commande = "#POO!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  motorRightSpeed
 * @param  None
 * @retval None
**********************************************************/
float motorRightSpeed()
{
String  commande;
 
  commande = "#MRS!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  motorLeftSpeed
 * @param  None
 * @retval None
**********************************************************/
float motorLeftSpeed()
{
String  commande;
 
  commande = "#MLS!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  robotGo
 * @param  
   coordonner X and coordonner Y
 * @retval None
**********************************************************/
bool robotGo(int Maxspeed, int coord_X, int coord_Y)
{
float posA, posX, posY;
float consigneDistance=0, consigneOrientation=0;
float erreurDistance=0, erreurOrientation=0;
int cmdMoteurRight=0, cmdMoteurLeft=0;	
bool robotGo_state = false;
int speedRight, speedLeft;
const unsigned int MAX_TIMEOUT = 100;

		posX = robotPositionX();
		posY = robotPositionY();
		posA = robotPositionOrientation();
		
		// calcul de la distance à parcourir
		consigneDistance = sqrt( pow(coord_X-posX,2)+pow(coord_Y-posY,2) );
		
		// calcul de l'orientation de consigne du robot
		consigneOrientation = atan2( coord_Y-posY, coord_X-posX);
		
		// calcul erreur en distance
		erreurDistance = consigneDistance;
		if(erreurDistance > ERR_DIS_SAT) erreurDistance = ERR_DIS_SAT;
		
		// calcul erreur en orientation
		erreurOrientation = consigneOrientation - posA;
		if(erreurOrientation > M_PI) erreurOrientation -= M_2PI;
		if(erreurOrientation < -M_PI) erreurOrientation += M_2PI;
		
		if(erreurDistance > DIST_MINI)
		{
			// calcul des commandes
			cmdMoteurRight = erreurDistance*K_PID_R + erreurOrientation*KO_PID_R;
			cmdMoteurLeft = erreurDistance*K_PID_L - erreurOrientation*KO_PID_L;
			
			// saturation de la commande
			cmdMoteurRight = saturationCmd(cmdMoteurRight, Maxspeed);
			cmdMoteurLeft = saturationCmd(cmdMoteurLeft, Maxspeed);
			
			// gestion du sens de rotation des moteurs
			if(cmdMoteurRight < 0 ) motorRight(abs(cmdMoteurRight), 1);
			else motorRight(abs(cmdMoteurRight), 0);  
			
			if(cmdMoteurLeft < 0 ) motorLeft(abs(cmdMoteurLeft), 1);
			else motorLeft(abs(cmdMoteurLeft), 0);  
			
			
			speedRight = motorRightSpeed();
			speedLeft = motorLeftSpeed();
			
			if((speedRight == 0)and(speedLeft == 0)and(timeout > 2))
			{
				stop();
				robotGo_state = true;
				timeout = 0;
			}
			
			timeout++;

		} 
		else
		{
			stop();
			robotGo_state = true;
			timeout = 0;
			
		}
		
		delay(50);
		
		return(robotGo_state);
	  
  
}


/**********************************************************
 * @brief  ledLeft
 * @param  
 * @retval None
**********************************************************/
void ledLeft(bool on_off)
{
String  commande;

  if(on_off > 0)
  {
    commande = "#LEL,1!";
    Serial.println(commande); 
	s_ledLeft = true;
  }
  else
  {
    commande = "#LEL,0!";
    Serial.println(commande); 
	s_ledLeft = false;
  }
}


/**********************************************************
 * @brief  ledRight
 * @param  
 * @retval None
**********************************************************/
void ledRight(bool on_off)
{
String  commande;

  if(on_off > 0)
  {
    commande = "#LER,1!";
    Serial.println(commande); 
	s_ledRight = true;
  }
  else
  {
    commande = "#LER,0!";
    Serial.println(commande); 
	s_ledRight = false;
  }
}


/**********************************************************
 * @brief  servo_write
 * @param  angle 0 to 200°
 * @retval None
**********************************************************/
void servo_write(int angle)
{
String  commande;	

  commande = "#SER," + String(angle) + "!";
  Serial.println(commande); 
}

//*********************************************************************/
//*********************************************************************/
//*********************************************************************/
//*****************PRIVATE FUNCTIONS***********************************/
//*********************************************************************/
//*********************************************************************/
//*********************************************************************/


/**********************************************************
 * @brief  turnRightControl
 * @param  
 * @retval None
**********************************************************/
static void turnLeftControl(int speed, int angle)
{
String  commande;

  controlEnable();
  
  if(state_control == true)
  {
    commande = "#TLC," + String(angle) + "," + String(speed) + "!";
    Serial.println(commande); 
  }
}


/**********************************************************
 * @brief  turnRightControl
 * @param  
 * @retval None
**********************************************************/
static void turnRightControl(int speed, int angle)
{
String  commande;

  controlEnable();
  
  if(state_control == true)
  {
    commande = "#TRC," + String(angle) + "," + String(speed) + "!";
    Serial.println(commande); 
  }
}


/**********************************************************
 * @brief  forwardC (forward with control)
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void forwardC(int speed, int distance)
{
String  commande;
int  state = 0;
 
  commande = "#MFC," + String(distance) + "," + String(speed) + "!";
  Serial.println(commande); 
  
  while(state != 3)
  {
    Serial.println("#TGS,1!"); 
    Serial.println(state);
    state = readData();
    
  }
  
}


/**********************************************************
 * @brief  backControl
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void backControl(int speed, int distance)
{
String  commande;

  controlEnable();
  
  if(state_control == true)
  {
    commande = "#MBC," + String(distance) + "," + String(speed) + "!";
    Serial.println(commande); 
  }

}


/**********************************************************
 * @brief  forwardControl
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void forwardControl(int speed, int distance)
{
String  commande;

  controlEnable();
  
  if(state_control == true)
  {
    commande = "#MFC," + String(distance) + "," + String(speed) + "!";
    Serial.println(commande); 
  }

}


/**********************************************************
 * @brief  controlEnable
 * @param  
 * @retval None
**********************************************************/
static void controlEnable()
{
  if(state_control == false)
  {
	resetUc();
    Serial.println("#CRE!"); 
    state_control = true;
  }
  else
  {
    // error
  }
  
}


/**********************************************************
 * @brief  controlDisable
 * @param  
 * @retval None
**********************************************************/
static void controlDisable()
{
  if(state_control == true)
  {
    Serial.println("#CRD!"); 
    state_control = false;
	//resetUc();
	
	if(s_ledRight == true)
		ledRight(1);
	
	if(s_ledLeft == true)
		ledLeft(1);
  }
  else
  {
    // error
  }
}


/**********************************************************
 * @brief  backC (back with control)
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void backC(int speed, int distance)
{
String  commande;
int  state = 0;
 
  commande = "#MBC," + String(distance) + "," + String(speed) + "!";
  Serial.println(commande); 
  
  while(state != 3)
  {
    Serial.println("#TGS,1!"); 
    Serial.println(state);
    state = readData();
    
  }
  
}


/**********************************************************
 * @brief  readFloatData
 * @param  
 * @retval None
**********************************************************/
static float readFloatData()
{ 
char c=0;        
String readString;

  while (c != '\n')
  {
    if (Serial.available() >0)
    {
      c = Serial.read();  //gets one byte from serial buffer
      if ( c != '$')
        readString += c; //makes the string readString
    } 
  }    

  return readString.toFloat();
}


/**********************************************************
 * @brief  turnLeftC
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void turnLeftC(int speed, int distance)
{
String  commande;
int  state = 0;
 
  commande = "#TLC," + String(distance) + "," + String(speed) + "!";
  Serial.println(commande); 
  
  while(state != 3)
  {
    Serial.println("#TGS,2!"); 
    Serial.println(state);
    state = readData();
  }
}


/**********************************************************
 * @brief  turnRightC
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
static void turnRightC(int speed, int distance)
{
String  commande;
int  state = 0;
 
  commande = "#TRC," + String(distance) + "," + String(speed) + "!";
  Serial.println(commande); 
  
  while(state != 3)
  {
    Serial.println("#TGS,2!"); 
    Serial.println(state);
    state = readData();
  }
  
}


/**********************************************************
 * @brief  readData
 * @param  
 * @retval None
**********************************************************/
static int readData()
{ 
char c=0;        
String readString;

  while (c != '\n')
  {
    if (Serial.available() >0)
    {
      c = Serial.read();  //gets one byte from serial buffer
      if ( c != '$')
        readString += c; //makes the string readString
    } 
  }    

  return readString.toInt();
}


// fonction de saturation des commandes moteurs
static int saturationCmd(int cmd, int maxSpeed)
{
  if(cmd > maxSpeed)
  {
    cmd = maxSpeed;
  }

  if(cmd < -maxSpeed)
  {
    cmd = -maxSpeed;
  }

  return(cmd);
}

// end file