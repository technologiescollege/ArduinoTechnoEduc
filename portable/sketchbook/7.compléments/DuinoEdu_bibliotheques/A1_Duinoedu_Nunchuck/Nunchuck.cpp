/*
Cette librairie a été inspirée du travail de 2007-11 Tod E. Kurt, http://todbot.com/blog/

L'encapsulation à été réalisée par David Souder pour DuinoEDU.com - 10/2014

  
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Nunchuck.h"

#include "Wire.h"
#include <avr/pgmspace.h>


//========== << CONSTRUCTOR >>
	Nunchuck::Nunchuck(){

}
	

	
//========== INITIALISATION
/*EDU FR*/	void 	Nunchuck::brancher(){branch();}
/*EDU US*/	void 	Nunchuck::branch(){init();}
/*ARD US*/	void 	Nunchuck::init(){ 
// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
			Wire.begin();                // join i2c bus as master
			Wire.beginTransmission(0x52);// transmit to device 0x52
		#if (ARDUINO >= 100)
			Wire.write((uint8_t)0x40);// sends memory address
			Wire.write((uint8_t)0x00);// sends sent a zero.
			
		#else
			Wire.send((uint8_t)0x40);// sends memory address
			Wire.send((uint8_t)0x00);// sends sent a zero.  
		
		#endif
			Wire.endTransmission();// stop transmitting
}

/*ARD US*/	void 	Nunchuck::init2(){ 
// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
			Wire.begin();                // join i2c bus as master
			Wire.beginTransmission(0x52);// transmit to device 0x52
		#if (ARDUINO >= 100)
			Wire.write (0xF0);
			Wire.write (0x55);
			Wire.write (0x00); 			
		#else
			Wire.send (0xF0);
			Wire.send (0x55);
			Wire.send (0x00); 
		
		#endif
			Wire.endTransmission();// stop transmitting
}

/*ARD US*/	void 	Nunchuck::init3(){ 
// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
			Wire.begin();                // join i2c bus as master
			// transmit to device 0x52
		#if (ARDUINO >= 100)
			Wire.beginTransmission(0x52);	
			Wire.write (0xF0);
			Wire.write (0x55);
			Wire.endTransmission();
			Wire.write (0xFB);
			Wire.write (0x00);
			Wire.endTransmission();
 			
		#else
			Wire.beginTransmission(0x52);	
			Wire.send (0xF0);
			Wire.send (0x55);
			Wire.endTransmission();
			Wire.send(0xFB);
			Wire.send (0x00);
			Wire.endTransmission();
		
		#endif
		
}

	
//========== METHODES OPERATIVES
/*EDU FR*/	void 	Nunchuck::ecrire(){send_request();}
/*ARD US*/	void 	Nunchuck::send_request(){
// Send a request for data to the nunchuck
// was "send_zero()"
		Wire.beginTransmission(0x52);// transmit to device 0x52
	#if (ARDUINO >= 100)
		Wire.write((uint8_t)0x00);// sends one byte
	#else
		Wire.send((uint8_t)0x00);// sends one byte
	#endif
		Wire.endTransmission();// stop transmitting
}

/*ARD US*/	char 	Nunchuck::decode_byte(char x){
// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
		x = (x ^ 0x17) + 0x17;
		return x;
}

/*ARD US*/	int 	Nunchuck::lireTout(){
		return get_data();
}
/*ARD US*/	int 	Nunchuck::get_data(){
// Receive data back from the nunchuck, 
// returns 1 on successful read. returns 0 on failure
		int cnt=0;
		Wire.requestFrom (0x52, 6);// request data from nunchuck
		while (Wire.available ()) {
        // receive byte as an integer
			#if (ARDUINO >= 100)
				nunchuck_buf[cnt] = decode_byte( Wire.read() );
			#else
				nunchuck_buf[cnt] = decode_byte( Wire.receive() );
			#endif
				cnt++;
		}
    
		send_request();  // send request for next data payload
		// If we recieved the 6 bytes, then go print them
		
	
		
		if (cnt >= 5) {
			return 1;   // success
		}	
    
			return 0; //failure

}

/*ARD US*/	void 	Nunchuck::print_data(){ 
// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
		static int i=0;
		int joy_x_axis = nunchuck_buf[0];
		int joy_y_axis = nunchuck_buf[1];
		int accel_x_axis = nunchuck_buf[2]; // * 2 * 2; 
		int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
		int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;

		int z_button = 0;
		int c_button = 0;

    // byte nunchuck_buf[5] contains bits for z and c buttons
    // it also contains the least significant bits for the accelerometer data
    // so we have to check each bit of byte outbuf[5]
		if ((nunchuck_buf[5] >> 0) & 1) 
			z_button = 1;
		if ((nunchuck_buf[5] >> 1) & 1)
			c_button = 1;

		if ((nunchuck_buf[5] >> 2) & 1) 
			accel_x_axis += 1;
		if ((nunchuck_buf[5] >> 3) & 1)
			accel_x_axis += 2;

		if ((nunchuck_buf[5] >> 4) & 1)
			accel_y_axis += 1;
		if ((nunchuck_buf[5] >> 5) & 1)
			accel_y_axis += 2;

		if ((nunchuck_buf[5] >> 6) & 1)
			accel_z_axis += 1;
		if ((nunchuck_buf[5] >> 7) & 1)
			accel_z_axis += 2;
		
		/*
		Serial.print(i,DEC);
		Serial.print("\t");

		Serial.print("joy:");
		Serial.print(joy_x_axis,DEC);
		Serial.print(",");
		Serial.print(joy_y_axis, DEC);
		Serial.print("  \t");

		Serial.print("acc:");
		Serial.print(accel_x_axis, DEC);
		Serial.print(",");
		Serial.print(accel_y_axis, DEC);
		Serial.print(",");
		Serial.print(accel_z_axis, DEC);
		Serial.print("\t");

		Serial.print("but:");
		Serial.print(z_button, DEC);
		Serial.print(",");
		Serial.print(c_button, DEC);

		Serial.print("\r\n");  // newline
		*/
		i++;
}//*/

/*EDU FR*/	int 	Nunchuck::lireBoutonZ(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return zbutton();
}
/*ARD US*/	int 	Nunchuck::zbutton(){
// returns zbutton state: 1=pressed, 0=notpressed
		return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}

/*EDU FR*/	int 	Nunchuck::lireBoutonC(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return cbutton();
}
/*ARD US*/	int 	Nunchuck::cbutton(){
// returns zbutton state: 1=pressed, 0=notpressed
		return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}

/*EDU FR*/	int 	Nunchuck::lireJoystickX(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return joyx();
}
/*ARD US*/	int 	Nunchuck::joyx(){
// returns value of x-axis joystick
		return nunchuck_buf[0]; 
}

/*EDU FR*/	int 	Nunchuck::lireJoystickY(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return joyy();
}
/*ARD US*/	int 	Nunchuck::joyy(){
// returns value of y-axis joystick
		return nunchuck_buf[1];
}

/*EDU FR*/	int 	Nunchuck::lireAccelerationX(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return accelx();
}
/*ARD US*/	int 	Nunchuck::accelx(){
// returns value of x-axis accelerometer
		return nunchuck_buf[2];   // FIXME: this leaves out 2-bits of the data
}

/*EDU FR*/	int 	Nunchuck::lireAccelerationY(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return accely();
}
/*ARD US*/	int 	Nunchuck::accely(){
// returns value of y-axis accelerometer
    return nunchuck_buf[3];   // FIXME: this leaves out 2-bits of the data
}

/*EDU FR*/	int 	Nunchuck::lireAccelerationZ(){
	//---- Lecture de tous les boutons
	get_data();
	//---- Délais minimum
	delay(20);
	return accelz();
}
/*ARD US*/	int 	Nunchuck::accelz(){
// returns value of z-axis accelerometer
    return nunchuck_buf[4];   // FIXME: this leaves out 2-bits of the data
}


//========== METHODES DE MODIFICATION D'OPTIONS




//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                               METHODES PRIVATE
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


