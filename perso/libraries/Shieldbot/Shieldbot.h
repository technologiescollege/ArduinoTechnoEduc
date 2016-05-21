/*
  Created to support the release of the Sheildbot from SeeedStudios
  http://www.seeedstudio.com/wiki/Shield_Bot

  Created by Jacob Rosenthal and Colin Ho, December 30, 2012.
  Released into the public domain.
  
  EDU : augmenté par David Souder pour www.duinoedu.com
  souder.d@gmail.com 
  
  V0.06 du 14/11/2014
  
*/

// ensure this library description is only included once
#ifndef Shieldbot_h
#define Shieldbot_h


// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class Shieldbot
{
  // user-accessible "public" interface
  public:

	Shieldbot();
	/*EDU FR*/	void brancher(void);
	/*EDU FR*/	bool lire(String S);
	int readS1();
	int readS2();
	int readS3();
	int readS4();
	int readS5();
	void setMaxSpeed(int);
	/*EDU FR*/	void ecrireVitesse(long SL, long SR);
	/*EDU FR*/	void reglerVitesse(long SL, long SR);
	/*EDU US*/	void setSpeedMax(long SL, long SR);
	/*ARD US*/	void setMaxSpeed(int, int);
	void setMaxLeftSpeed(int);
	void setMaxRightSpeed(int);
	void rightMotor(char);
	void leftMotor(char);
	
	
	/*EDU FR*/  void avancer(String direction="");
	/*EDU US*/	void forward(String dir="straight");
	/*ARD US*/	void forward(long L, long R);
	
	/*EDU FR*/	void reculer(String direction="");
	/*EDU US*/	void back(String dir="straight");
	/*ARD US*/ 	void backward(long L, long R);
	
	void drive(char, char);
	
	/*EDU FR*/  void pivoter(String direction="");
	/*EDU US*/  void rotate(String dir="");
	
	/*EDU FR*/  void stopper();
	/*ARD US*/	void stop();
	void stopRight();
	void stopLeft();
	void fastStopLeft();
	void fastStopRight();
	void fastStop();
	

	
	
	
	private:
	//Méthodes 
		/*EDU US*/	uint16_t    m_defaultSpeed;
		/*EDU US*/	uint16_t	m_curentSpeedR;
		/*EDU US*/	uint16_t	m_curentSpeedL;
		
	
	//Mémoire

};

#endif