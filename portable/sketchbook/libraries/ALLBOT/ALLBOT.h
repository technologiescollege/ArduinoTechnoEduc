#ifndef ALLBOT_H
#define ALLBOT_H

#include "ALLBOT_AsyncServo.h"

class ALLBOT {
public:
	ALLBOT(int count);
	~ALLBOT();
	ALLBOT_AsyncServo& operator[](int i);
	void attach(int servo, int pin, int angle, bool flipped, int offset);
	void move(int servo, int angle);
	void animate(int speed);
protected:
private:
	ALLBOT_AsyncServo *_servo;
	int _count;
};

#endif