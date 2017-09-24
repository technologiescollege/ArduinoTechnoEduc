#include "ALLBOT_AsyncServo.h"

ALLBOT_AsyncServo::ALLBOT_AsyncServo()
	:ALLBOT_Servo()
{

}

void ALLBOT_AsyncServo::move(int to_angle)
{
	// save the target angle
	_to_angle = to_angle;
}

void ALLBOT_AsyncServo::reset()
{
	_to_angle = _angle;
}

void ALLBOT_AsyncServo::prepare(int speed)
{
	// calculate the absolute value of the angle difference
	int angle_diff = _to_angle - _angle;
	if (angle_diff < 0)
		angle_diff *= -1;

	if (angle_diff == 0) {
		_step = 0;
		_steps = 0;
		return;
	}

	// number of degrees to move with each 1ms step
	_step_angle = (double)angle_diff / speed;

	_cur_angle = _angle;

	// stepping
	_step = 0;
	_steps = angle_diff / _step_angle;

	// 180°->0° = count downwards
	if (_to_angle < _angle)
		_step_angle *= -1;

}

bool ALLBOT_AsyncServo::tick()
{
	if (_step < _steps)
	{
		_cur_angle += _step_angle;
		write(_cur_angle);
		_step++;
	}

	return (_step >= _steps);
}