#include "ALLBOT.h"

#include <Arduino.h>

ALLBOT::ALLBOT(int count)
	:_count(count)
{
	_servo = new ALLBOT_AsyncServo[count];
}

ALLBOT::~ALLBOT()
{
	delete[] _servo;
}

ALLBOT_AsyncServo& ALLBOT::operator[](int i)
{
	return _servo[i];
}

void ALLBOT::attach(int servo, int pin, int angle, bool flipped, int offset)
{
	ALLBOT_AsyncServo &s = _servo[servo];

	s.flipped(flipped);
	s.offset(offset);
	
	s.attach(pin);
	s.write(angle);
	s.reset();
}

void ALLBOT::move(int servo, int angle)
{
	_servo[servo].move(angle);
}

void ALLBOT::animate(int speed)
{
	for (int i=0; i<_count; i++)
	{
		_servo[i].prepare(speed);
	}

	bool done;

	do {
		done = true;

		for (int i=0; i<_count; i++)
		{
			done &= _servo[i].tick();
		}

		delay(1);
	}
	while(!done);
}