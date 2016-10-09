#ifndef ALLBOT_ASYNC_SERVO_H
#define ALLBOT_ASYNC_SERVO_H

#include "ALLBOT_Servo.h"

class ALLBOT_AsyncServo : public ALLBOT_Servo {
public:
	ALLBOT_AsyncServo();
	void reset();
	void move(int angle);
	void prepare(int speed);
	bool tick();
protected:
private:
	int _to_angle;
	double _step_angle;
	double _cur_angle;
	int _step;
	int _steps;
};

#endif