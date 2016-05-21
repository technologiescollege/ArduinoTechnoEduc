/*
  Created to support the release of the Sheildbot from SeeedStudios
  http://www.seeedstudio.com/wiki/Shield_Bot

  Created by Jacob Rosenthal and Colin Ho, December 30, 2012.
  Released into the public domain.
*/

// include core Wiring API
#include "Arduino.h"

// include this library's description file
#include "Shieldbot.h"

#define SHIELDBOTDEBUG 0
// for shieldbot Version 0.9 and 1.0
 /*
#define right1 5				//define I1 interface
#define speedPinRight 6  	//enable right motor (bridge A)
#define right2 7      			//define I2 interface 
#define left1 8      			//define I3 interface 
#define speedPinLeft 9  	//enable motor B
#define left2 10     			//define I4 interface 
*/
 
 
// for shieldbot Version 1.1, we changed to the driver pins ,
//if you shieldbot is ver1.0 or 0.9, please using the above pin mapping;
#define right1 8				//define I1 interface
#define speedPinRight 9 		//enable right motor (bridge A)
#define right2 11      			//define I2 interface 
#define left1 12      			//define I3 interface 
#define speedPinLeft 10  		//enable motor B
#define left2 13    			//define I4 interface 

#define finder1 A0   			//define line finder S1
#define finder2 A1   			//define line finder S2
#define finder3 A2   			//define line finder S3
#define finder4 A3   			//define line finder S4
#define finder5 4    			//define line finder S5

int speedmotorA = 255; 			//define the speed of motorA
int speedmotorB = 255; 			//define the speed of motorB

  //========== << CONSTRUCTOR >>
Shieldbot::Shieldbot(){
  pinMode(right1,OUTPUT);
  pinMode(right2,OUTPUT);
  pinMode(speedPinRight,OUTPUT);
  pinMode(left1,OUTPUT);
  pinMode(left2,OUTPUT);
  pinMode(speedPinLeft,OUTPUT);
  pinMode(finder1,INPUT);
  pinMode(finder2,INPUT);
  pinMode(finder3,INPUT);
  pinMode(finder4,INPUT);
  pinMode(finder5,INPUT);
}

/*EDU FR*/	void Shieldbot::brancher(void){
	//----On fixe les valeurs par défaut
		m_defaultSpeed = 75;
	//----On fixe une vitesse par défaut
		setMaxSpeed(m_defaultSpeed, m_defaultSpeed);		// 0 à 100
	//----On stoppe les moteurs au cas où
		stop();
}



//sets same max speed to both motors
void Shieldbot::setMaxSpeed(int both){
  setMaxLeftSpeed(both);
  setMaxRightSpeed(both);
}


/*EDU FR*/	void Shieldbot::ecrireVitesse(long SL, long SR){
	setSpeedMax(SL, SR);
}//*/
/*EDU FR*/	void Shieldbot::reglerVitesse(long SL, long SR){
	setSpeedMax(SL, SR);
}//*/

/*EDU US*/	void Shieldbot::setSpeedMax(long SL, long SR){
	//---- On récupère les vitesses souhaitées avec ajustement
		if(SL>1000){SL=1000;} if(SL<0){SL=0;}
		if(SR>1000){SR=1000;} if(SR<0){SR=0;}
		m_curentSpeedL = SL;
		m_curentSpeedR = SR;
	//---- On actualise les vitesses
		setMaxSpeed(m_curentSpeedL/10, m_curentSpeedR/10);

}//*/

/*ARD US*/	void Shieldbot::setMaxSpeed(int left, int right){
  setMaxLeftSpeed(left);
  setMaxRightSpeed(right);
}

void Shieldbot::setMaxLeftSpeed(int left){
  speedmotorB=left;
}

void Shieldbot::setMaxRightSpeed(int right){
  speedmotorA=right;
}

//---------- Lire les capteurs
bool Shieldbot::lire(String S){
	if(S=="S1"){
		int sInt;
		sInt = readS1();
		if(sInt==LOW){
			return true;
		}
		else{
			return false;
		}
	}
	
	if(S=="S2"){
		int sInt;
		sInt = readS2();
		if(sInt==LOW){
			return true;
		}
		else{
			return false;
		}
	}
	
	if(S=="S3"){
		int sInt;
		sInt = readS3();
		if(sInt==LOW){
			return true;
		}
		else{
			return false;
		}
	}

	if(S=="S4"){
		int sInt;
		sInt = readS4();
		if(sInt==LOW){
			return true;
		}
		else{
			return false;
		}
	}
	
	if(S=="S5"){
		int sInt;
		sInt = readS5();
		if(sInt==LOW){
			return true;
		}
		else{
			return false;
		}
	}
	
	
	
	
	
}//*/	
	

int Shieldbot::readS1(){
  return digitalRead(finder1);
}

int Shieldbot::readS2(){
  return digitalRead(finder2);
}

int Shieldbot::readS3(){
  return digitalRead(finder3);
}

int Shieldbot::readS4(){
  return digitalRead(finder4);
}

int Shieldbot::readS5(){
  return digitalRead(finder5);
}

void Shieldbot::drive(char left, char right){
  rightMotor(right);
  leftMotor(left);
}

//char is 128 to 127
//mag is the direction to spin the right motor
//-128 backwards all the way
//0 dont move
//127 forwards all the way
void Shieldbot::rightMotor(char mag){
  int actualSpeed = 0;  
  if(mag >0){ //forward
    float ratio = (float)mag/128;
    actualSpeed = (int)(ratio*speedmotorA); //define your speed based on global speed
	#if SHIELDBOTDEBUG
      Serial.print("forward right: ");
      Serial.println(actualSpeed);
	#endif
    analogWrite(speedPinRight,actualSpeed);
    digitalWrite(right1,HIGH);
    digitalWrite(right2,LOW);//turn right motor clockwise
  }else if(mag == 0){ //neutral
	#if SHIELDBOTDEBUG
      Serial.println("nuetral right");
	#endif
	stopRight();
  }else { //meaning backwards
    float ratio = (float)abs(mag)/128;
    actualSpeed = ratio*speedmotorA;
	#if SHIELDBOTDEBUG
      Serial.print("backward right: ");
      Serial.println(actualSpeed);
	#endif
    analogWrite(speedPinRight,actualSpeed);
    digitalWrite(right1,LOW);
    digitalWrite(right2,HIGH);//turn right motor counterclockwise
  }
}

//TODO shouldnt these share one function and just input the differences?
void Shieldbot::leftMotor(char mag){
  int actualSpeed = 0;  
  if(mag >0){ //forward
    float ratio = (float)(mag)/128;
    actualSpeed = (int)(ratio*speedmotorB); //define your speed based on global speed
	#if SHIELDBOTDEBUG
      Serial.print("forward left: ");
      Serial.println(actualSpeed);
	#endif
    analogWrite(speedPinLeft,actualSpeed);
    digitalWrite(left1,HIGH);
    digitalWrite(left2,LOW);//turn left motor counter-clockwise
  }else if(mag == 0){ //neutral
	#if SHIELDBOTDEBUG
      Serial.println("nuetral left");
	#endif
	stopLeft();
  }else { //meaning backwards
    float ratio = (float)abs(mag)/128;
    actualSpeed = ratio*speedmotorB;
	#if SHIELDBOTDEBUG
      Serial.print("backward left: ");
      Serial.println(actualSpeed);
	#endif
    analogWrite(speedPinLeft,actualSpeed);
    digitalWrite(left1,LOW);
    digitalWrite(left2,HIGH);//turn left motor counterclockwise
  }
}


//========== DEPLACEMENTS
//---------- Avancer
/*EDU FR*/  void Shieldbot::avancer(String direction){
	// Choix de la direction
			if(direction=="" || direction=="devant" || direction=="tout droit" || direction=="tout_droit")   	{forward("straight");} 
	else 	if(direction=="droite")                    															{forward("right");}
	else 	if(direction=="gauche")                    															{forward("left");}
}
/*EDU US*/	void Shieldbot::forward(String dir){
		if(dir=="" 				|| dir=="straight" 		|| dir=="tout droit" || dir=="tout_doit")				{forward(1000,1000);}
		if(dir=="right" )																						{forward(1000,0);}
		if(dir=="left"  )    																					{forward(0,1000);}
}
/*ARD US*/	void Shieldbot::forward(long L, long R){
//Serial.println("Passage ds la fonction");
	if(L>1023){L=1023;}	if(L<0){L=0;}
	if(R>1023){R=1023;}	if(R<0){R=0;}
	leftMotor(L/8);
	rightMotor(R/8); 
}

//---------- Reculer
/*EDU FR*/  void Shieldbot::reculer(String direction){
	     if(direction=="" || direction=="tout droit" || direction=="tout_droit")          						{back("straight");} 
	else if(direction=="droite")                    															{back("right");}
	else if(direction=="gauche")                    															{back("left");}
}
/*EDU US*/	void Shieldbot::back(String dir){
		if(dir=="straight" || dir=="tout droit" || dir=="tout_droit")											{backward(1000,1000);}
		if(dir=="right" || dir=="droie")   																		{backward(1000,0);}
		if(dir=="left"  || dir=="gauche")  																		{backward(0,1000);}
}
/*ARD US*/	void Shieldbot::backward(long L, long R){
//Serial.println("Passage ds la fonction");
	if(L>1023){L=1023;}	if(L<0){L=0;}
	if(R>1023){R=1023;}	if(R<0){R=0;}
	leftMotor(-L/8);
	rightMotor(-R/8);
	//leftMotor(-128);
	//rightMotor(-128); 
}

//---------- Pivoter
/*EDU FR*/  void Shieldbot::pivoter(String direction){
	// Traitement de la chaine en prévision
	// Choix de la direction

		 if(direction=="droite"||direction=="tout_droit")                    				{rotate("right");}
	else if(direction=="gauche")                    										{rotate("left");}
	else {
		//----- Aide utilisateur
		Serial.println("ERREUR : avec la fonction pivoter !");
		Serial.println("pivoter ne comprend que : gauche/droite et ne peut pas rester vide");
	}
}
/*EDU US*/	void Shieldbot::rotate(String dir){
		if(dir=="")        {Serial.println("Choisir une direction pour la rotation");}
		if(dir=="right"){
			//--Pivoter à droite
			leftMotor(127);
			rightMotor(-128);
			
			
		}
		if(dir=="left"){
			//--Pivoter à gauche);
			leftMotor(-128);
			rightMotor(127);
		}
}

//---------- Stopper
/*EDU FR*/  void Shieldbot::stopper(){stop();}
void Shieldbot::stop(){
  stopLeft();
  stopRight();
}

void Shieldbot::stopLeft(){
  analogWrite(speedPinLeft,0);
}

void Shieldbot::stopRight(){
  analogWrite(speedPinRight,0);
}

//may be dangerous to motor if reverse current into hbridge exceeds 2A
void Shieldbot::fastStopLeft(){
  digitalWrite(left1,LOW);
  digitalWrite(left2,LOW);//turn DC Motor B move clockwise
}

//may be dangerous to motor if reverse current into hbridge exceeds 2A
void Shieldbot::fastStopRight(){
  digitalWrite(right1,LOW);
  digitalWrite(right2,LOW);//turn DC Motor A move anticlockwise
}

//may be dangerous to motor if reverse current into hbridge exceeds 2A
void Shieldbot::fastStop(){
	fastStopRight();
	fastStopLeft();
}

