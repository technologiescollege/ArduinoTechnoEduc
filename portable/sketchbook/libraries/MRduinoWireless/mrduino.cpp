/**
  ******************************************************************************
  * @file    mrduino.cpp
  * @author  Mace Robotics
  * @version 0.21
  * @date    08/06/2017
  * @brief   lib for MRduino robot an MRduino Wireless
  *
 *******************************************************************************/


#include <Arduino.h>
#include <math.h>
#include <assert.h>
#include "mrduino.h"

static void forwardControl(int speed, int distance);
static void backControl(int speed, int distance);
static void turnRightControl(int speed, int angle);
static void turnLeftControl(int speed, int angle);
static boolean  state_control = false;

void initRobot()
{
  Serial.begin(115200);
  controlDisable();
  
  delay(0.5);
}


/**********************************************************
 * @brief  led
 * @param  None
 * @retval None
**********************************************************/
void led(int led, int state)
{
String  commande;
 
  commande = "#LED," + String(led) + "," + String(state) + "!";
  Serial.println(commande); 
  
}


/**********************************************************
 * @brief  ledToggle
 * @param  None
 * @retval None
**********************************************************/
void ledToggle(int led)
{
String  commande;
 
  commande = "#LEDT," + String(led) + "!";
  Serial.println(commande); 
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
 * @brief  battery
 * @param  None
 * @retval None
**********************************************************/
float temperature()
{
String  commande;
 
  commande = "#TE!";
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
 * @brief  irReceiver
 * @param  None
 * @retval None
**********************************************************/
int irReceiver()
{
String  commande;
 
  commande = "#RC5!";
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

  commande = "#MOTL," + String(direction) + "," + String(speed) + "!";
  Serial.println(commande); 
}



/**********************************************************
 * @brief  forward
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void forward(int speed)
{
String  commande;

  commande = "#MF," + String(speed) + "!";
  Serial.println(commande); 
  
  //forwardControl(speed, 9999);
 }

/**********************************************************
 * @brief  forwardC (forward with control)
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void forwardC(int speed, int distance)
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
 * @brief  forward_mm (forward with control)
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void forward_mm(int speed, int distance)
{
  forwardC(speed, distance*4);
}


/**********************************************************
 * @brief  back_mm (forward with control)
 * @param  speed ( 0 to 100 ), distance millimeter
 * @retval None
**********************************************************/
void back_mm(int speed, int distance)
{
  backC(speed, distance*4);
}


/**********************************************************
 * @brief  backC (back with control)
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void backC(int speed, int distance)
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
 * @brief  back
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void back(int speed)
{
String  commande;

  commande = "#MB," + String(speed) + "!";
  Serial.println(commande); 
  
    
  //backControl(speed, 9999);
 }

/**********************************************************
 * @brief  turnLeft
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnLeft(int speed)
{  
String  commande;

  commande = "#TL," + String(speed) + "!";
  Serial.println(commande); 
 }

/**********************************************************
 * @brief  turnLeftC
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnLeftC(int speed, int distance)
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
 * @brief  turnRight_degree
 * @param  speed ( 0 to 100 ), angle (0 to 360)
 * @retval None
**********************************************************/
void turnRight_degree(int speed, unsigned int angle)
{
float angle_degree = 0;
int angle_turn;

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
}




/**********************************************************
 * @brief  turnRight
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnRight(int speed)
{
String  commande;
 
  commande = "#TR," + String(speed) + "!";
  Serial.println(commande); 
  //turnRightControl(speed,99999);
}


/**********************************************************
 * @brief  turnRightC
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
void turnRightC(int speed, int distance)
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
 * @brief  proxSensor
 * @param  speed ( 0 to 100 )
 * @retval None
**********************************************************/
int proxSensor(int number)
{
String  commande;
int data;
 
  commande = "#PROX," + String(number) + "!";
  Serial.println(commande); 

  data = readData();
  
  return data;
}


/**********************************************************
 * @brief  ambiantLight
 * @param  
 * @retval None
**********************************************************/
int ambiantLight(int number)
{
String  commande;
int data;
 
  commande = "#AL," + String(number) + "!";
  Serial.println(commande); 

  data = readData();
  
  return data;
}


/**********************************************************
 * @brief  groundSensor
 * @param  
 * @retval None
**********************************************************/
int groundSensor(int number)
{
String  commande;
int data;
 
  commande = "#GR," + String(number) + "!";
  Serial.println(commande); 

  data = readData();
  
  return data;
}


/**********************************************************
 * @brief  controlEnable
 * @param  
 * @retval None
**********************************************************/
void controlEnable()
{
  if(state_control == false)
  {
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
void controlDisable()
{
  Serial.println("#CRD!"); 
  state_control = false;
}


/**********************************************************
 * @brief  stop
 * @param  
 * @retval None
**********************************************************/
void stop()
{
  //Serial.println("#STP!"); 
  
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
 * @brief  acceleroX
 * @param  None
 * @retval None
**********************************************************/
float acceleroX()
{
String  commande;
 
  commande = "#ACCX!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  acceleroY
 * @param  None
 * @retval None
**********************************************************/
float acceleroY()
{
String  commande;
 
  commande = "#ACCY!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  acceleroZ
 * @param  None
 * @retval None
**********************************************************/
float acceleroZ()
{
String  commande;
 
  commande = "#ACCZ!";
  Serial.println(commande); 
  
  return(readFloatData());
}


/**********************************************************
 * @brief  acceleroZ
 * @param  None
 * @retval None
**********************************************************/
void speakerEnable()
{
String  commande;
 
  commande = "#SPE!";
  Serial.println(commande); 

}


/**********************************************************
 * @brief  readData
 * @param  
 * @retval None
**********************************************************/
int readData()
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

/**********************************************************
 * @brief  readFloatData
 * @param  
 * @retval None
**********************************************************/
float readFloatData()
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
 * @param  
 * @retval None
**********************************************************/
float robotPositionX()
{
String  commande;
 
  commande = "#POX!";
  Serial.println(commande); 
  
  return(readFloatData());

}

/**********************************************************
 * @brief  robotPositionY
 * @param  None
 * @retval None
**********************************************************/
float robotPositionY()
{
String  commande;
 
  commande = "#POY!";
  Serial.println(commande); 
  
  return(-readFloatData());

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
 * @brief  robotGo
 * @param coordonner X and coordonner Y
 * @retval None
**********************************************************/
void robotGo(int speed, int coord_X, int coord_Y)
{
int distance;
float temp;
float angle, angle_Robot;
float coord_X_Robot, coord_Y_Robot;
float coord_X_Goal, coord_Y_Goal;

  controlEnable();


  // read actual robot position
  coord_X_Robot = robotPositionX();
  coord_Y_Robot = robotPositionY();
  
  // read actual robot position
  angle_Robot = robotPositionOrientation();
  
  // conversion en degree
  angle_Robot = (angle_Robot*180.0)/(3.14);
  
  // calcul goal coordonner
  coord_X_Goal = (float)(coord_X - coord_X_Robot);
  coord_Y_Goal = (float)(coord_Y - coord_Y_Robot);

  if(coord_Y_Goal > 0)
  {

    distance = sqrt(coord_X_Goal*coord_X_Goal + coord_Y_Goal*coord_Y_Goal);
	temp = (float)((float)coord_X_Goal/(float)distance);
	
	if(temp > 1.0)
	{
	  temp = 1.0;
	}
	   
	if(temp < -1.0)
	{
	  temp = -1.0;
	}
	   
    angle = acos(temp);
  }
  else
  {
    if(coord_X_Goal > 0)
    {

      distance = sqrt(coord_X_Goal*coord_X_Goal + coord_Y_Goal*coord_Y_Goal);
	  temp = (float)((float)coord_Y_Goal/(float)distance);
	  
	  	if(temp > 1.0)
	   {
	     temp = 1.0;
	   }
	   
	   if(temp < -1.0)
	   {
	     temp = -1.0;
	   }
	   
	   
      angle = asin(temp);
    }
    else
    {

      distance = sqrt(coord_X_Goal*coord_X_Goal + coord_Y_Goal*coord_Y_Goal);
	  temp = (float)((float)coord_X_Goal/(float)distance);
	  
	   if(temp > 1.0)
	   {
	     temp = 1.0;
	   }
	   
	   if(temp < -1.0)
	   {
	     temp = -1.0;
	   }
	      
	  temp = asin(temp);

      angle = -(1.5707 - temp);
    }

  }

  // conversion en degree
  angle = (angle*180.0)/(3.14);

  if(angle < 0)
  {
	  angle = angle + 360;
  }

  turn_degree(speed,angle); 
	
  forwardC(speed, distance);
  
  controlDisable();
  
}

// end file