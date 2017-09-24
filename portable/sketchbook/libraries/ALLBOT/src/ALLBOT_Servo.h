#ifndef ALLBOT_SERVO_H
#define ALLBOT_SERVO_H

#include <Servo.h>

class ALLBOT_Servo {
public:
	ALLBOT_Servo();
	void attach(int pin);
	void flipped(bool flipped);
	void offset(int offset);
	void write(int angle);
	ALLBOT_Servo& operator=(int angle);
protected:
	int _angle;
	bool _flipped;
	int _offset;
	Servo _servo;
};

#endif