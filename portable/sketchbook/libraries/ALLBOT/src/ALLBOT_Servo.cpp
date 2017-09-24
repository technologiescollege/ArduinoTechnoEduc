#include "ALLBOT_Servo.h"

ALLBOT_Servo::ALLBOT_Servo()
	:_flipped(false)
{

}

void ALLBOT_Servo::flipped(bool flipped)
{
	_flipped = flipped;
}

void ALLBOT_Servo::offset(int offset)
{
	_offset = offset;
}

void ALLBOT_Servo::attach(int pin)
{
	_servo.attach(pin);
}

ALLBOT_Servo& ALLBOT_Servo::operator=(int angle)
{
	write(angle);
}

void ALLBOT_Servo::write(int angle)
{
	_angle = angle;

	if (_flipped) angle = 180 - angle;

	angle += (_flipped ? -_offset : _offset);

	_servo.write(angle);
}