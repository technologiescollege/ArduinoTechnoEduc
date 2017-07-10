/*
PROJET INITIAL
	Nunchuck functions  -- Talk to a Wii Nunchuck
	This library is from the Bionic Arduino course : 
    http://todbot.com/blog/bionicarduino/
    2007-11 Tod E. Kurt, http://todbot.com/blog/
    The Wii Nunchuck reading code originally from Windmeadow Labs
    http://www.windmeadow.com/node/42

ENCAPSULATION ET AJOUT DE FONCTIONS POUR ARDUBLOCK
	Par David Souder - 10/2014 pour DuinoEDU.com

INTEGRATION DANS ARDUBLOCK
	Par Karl Thomas - 10/2014
 
 */

#ifndef Nunchuck_H
#define Nunchuck_H 

//--- Proposé par le concepteur inital
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
//#define Wire.write(x) Wire.send(x)
//#define Wire.read() Wire.receive()
#endif

//--- Ce que j'utilise en généal
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif
*/

class Nunchuck{
	public:
	/*EDU FR*/	Nunchuck();
	/*EDU FR*/	void 	brancher();
	/*EDU US*/	void 	branch();
	/*ARD US*/	void 	init(); 
	/*EDU US*/	void    init2();
	/*EDU US*/	void    init3();
	
	/*EDU FR*/	void 	ecrire();
	/*ARD US*/	void 	send_request();
	
	/*ARD US*/	char 	decode_byte(char x);
	
	/*EDU FR*/  int		lireTout();
	/*ARD US*/	int 	get_data();
	
	/*ARD US*/	void 	print_data();
	
	/*EDU FR*/	int 	lireBoutonZ();
	/*ARD US*/	int 	zbutton();
	
	/*EDU FR*/	int 	lireBoutonC();
	/*ARD US*/	int 	cbutton();
	
	/*EDU FR*/	int 	lireJoystickX();
	/*ARD US*/	int 	joyx();

	/*EDU FR*/	int 	lireJoystickY();	
	/*ARD US*/	int 	joyy();

	/*EDU FR*/	int 	lireAccelerationX();	
	/*ARD US*/	int 	accelx();

	/*EDU FR*/	int 	lireAccelerationY();	
	/*ARD US*/	int 	accely();
	
	/*EDU FR*/	int 	lireAccelerationZ();		
	/*ARD US*/	int 	accelz();
	


	protected:
	/*ARD US*/	uint8_t nunchuck_buf[6];   // array to store nunchuck data,
};
#endif
