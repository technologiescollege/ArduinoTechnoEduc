/********************************************************************************************
* 	 	File: 		uStepperSLite.cpp														*
*		Version:    1.1.0                                           						*
*      	Date: 		June 14, 2020 	                                    					*
*      	Author: 	Thomas Hørring Olsen                                   					*
*                                                   										*	
*********************************************************************************************
*			           			 uStepper S-lite class 					   					*
* 																							*
*	This file contains the implementation of the class methods, incorporated in the  		*
*	uStepper S-lite arduino library. The library is used by instantiating an uStepper 		*
*	S-lite object by calling either of the two overloaded constructors: 					*
*																							*
*		example:																			*
*																							*
*		uStepperSLite stepper; 																*
*																							*
*		OR 																					*
*																							*
*		uStepperSLite stepper(500, 2000);													*
*																							*
*	The first instantiation above creates a uStepper S-lite object with default 			*
*	acceleration and maximum speed (1000 steps/s^2 and 1000steps/s respectively).			*
*	The second instantiation overwrites the default settings of acceleration and 			*
*	maximum speed (in this case 500 steps/s^2 and 2000 steps/s, respectively);				*
*																							*
*	After instantiation of the object, the object setup function should be called within 	*
*	Arduino's setup function:																*
*																							*
*		example:																			*
*																							*
*		uStepperSLite stepper;																*
*																							*
*		void setup()																		*
*		{																					*
*			stepper.setup();																*
*		} 																					*
*																							*
*		void loop()																			*
*		{																					*
*																							*
*		}																					*
*																							*
*	After this, the library is ready to control the motor!									*
*																							*
*********************************************************************************************
*	(C) 2018																				*
*																							*
*	uStepper ApS																			*
*	www.ustepper.com 																		*
*	administration@ustepper.com 															*
*																							*
*	The code contained in this file is released under the following open source license:	*
*																							*
*			Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International			*
* 																							*
* 	The code in this file is provided without warranty of any kind - use at own risk!		*
* 	neither uStepper ApS nor the author, can be held responsible for any damage				*
* 	caused by the use of the code contained in this file ! 									*
*                                                                                           *
********************************************************************************************/
/**
 * @file uStepperSLite.cpp
 * @brief      Class implementations for the uStepper S-lite library
 *
 *             This file contains the implementations of the classes defined in
 *             uStepperSLite.h
 *
 * @author     Thomas Hørring Olsen (thomas@ustepper.com)
 */
#include <uStepperSLite.h>
#include <math.h>
uStepperSLite *pointer;
volatile int32_t *p __attribute__((used));
i2cMaster I2C(1);
extern "C" {

void INT0_vect(void)
{
	if(PIND & 0x04)
	{
		PORTD |= (1 << 4);
	}
	else
	{
		PORTD &= ~(1 << 4);
	}
	if((PINB & (0x08)))			//CCW
	{
		if(!pointer->invertPidDropinDirection)
		{
			pointer->stepCnt--;				//DIR is set to CCW, therefore we subtract 1 step from step count (negative values = number of steps in CCW direction from initial postion)
		}
		else
		{
			pointer->stepCnt++;			//DIR is set to CW, therefore we add 1 step to step count (positive values = number of steps in CW direction from initial postion)
		}
		
	}
	else						//CW
	{
		if(!pointer->invertPidDropinDirection)
		{
			pointer->stepCnt++;			//DIR is set to CW, therefore we add 1 step to step count (positive values = number of steps in CW direction from initial postion)
		}
		else
		{
			pointer->stepCnt--;				//DIR is set to CCW, therefore we subtract 1 step from step count (negative values = number of steps in CCW direction from initial postion)
		}
	}
}

void INT1_vect(void)
{
	if(PIND & 0x04)
	{
		PORTD |= (1 << 4);
	}
	else
	{
		PORTD &= ~(1 << 4);
	}
}

void TIMER3_COMPA_vect(void)
{
	asm volatile("push r16 \n\t");
	asm volatile("in r16,0x3F \n\t");
	asm volatile("push r16 \n\t");
	asm volatile("push r30 \n\t");
	asm volatile("push r31 \n\t");
	asm volatile("lds r30,p \n\t");
	asm volatile("lds r31,p+1 \n\t");

	asm volatile("jmp _stepGenerator \n\t");	//Execute the acceleration profile algorithm

}

void TIMER1_COMPA_vect(void)
	{
		uint8_t data[2];
		uint16_t curAngle;
		int16_t deltaAngle;
		float posError = 0.0;
		static float posEst = 0.0;
		static float velIntegrator = 0.0;
		static float velEst = 0.0;
		uint32_t temp;
		int32_t stepCntTemp;
		int32_t pidTargetPositionTruncated;
		int32_t *stepsSinceResetPointer;
		float tempFloat;
		sei();

		if(I2C.getStatus() != I2CFREE)
		{
			return;
		}

		I2C.read(ENCODERADDR, ANGLE, 2, data);

		curAngle = (((uint16_t)data[0]) << 8 ) | (uint16_t)data[1];
		pointer->encoder.angle = curAngle;
		curAngle -= pointer->encoder.encoderOffset;

		if(curAngle > 4095)
		{
			curAngle -= 61440;
		}

		deltaAngle = (int16_t)pointer->encoder.oldAngle - (int16_t)curAngle;

		if(deltaAngle < -2047)
		{
			deltaAngle += 4096;
		}

		else if(deltaAngle > 2047)
		{
			deltaAngle -= 4096;
		}

		pointer->encoder.angleMoved -= deltaAngle;		//The AS5600 encoder counts in the opposite direction
														//as the AEAT8800 (the encoder on uStepper S).
														//Therefore it is subtracted here, so the two boards
														//behave similar

		pointer->encoder.oldAngle = curAngle;

		if(pointer->mode == DROPIN)
		{
			cli();
				stepCntTemp = pointer->stepCnt;
			sei();

			//		Speed filter
			posEst += velEst * ENCODERINTSAMPLETIME;
			posError = (float)stepCntTemp - posEst;
			velIntegrator += posError * PULSEFILTERKI;
			velEst = (posError * PULSEFILTERKP) + velIntegrator;
			pointer->currentPidSpeed = velIntegrator;

			posError = (float)stepCntTemp - ((float)pointer->encoder.angleMoved * pointer->stepConversion);

			pointer->pidDropin(posError);
			return;
		}
		else
		{
			/*
			//		Speed filter
			posEst += velEst * ENCODERINTSAMPLETIME;
			posError = (float)pointer->encoder.angleMoved - posEst;
			velIntegrator += posError * PULSEFILTERKI;
			velEst = (posError * PULSEFILTERKP) + velIntegrator;
			pointer->encoder.curSpeed = velIntegrator * pointer->stepConversion;
			*/

			//stepGenerator speed integrator
			pointer->currentPidSpeed += pointer->currentPidAcceleration;
			if(pointer->direction == CW)
			{
				if(pointer->currentPidSpeed >= pointer->velocity)
				{
					pointer->currentPidSpeed = pointer->velocity;
				}
				else if(pointer->currentPidSpeed < 0.0)
				{
					pointer->currentPidSpeed = 0.0;
				}
			}
			else
			{
				if(pointer->currentPidSpeed <= -pointer->velocity)
				{
					pointer->currentPidSpeed = -pointer->velocity;
				}
				else if(pointer->currentPidSpeed > 0.0)
				{
					pointer->currentPidSpeed = 0.0;
				}
			}


			//stepgenerator targetposition integrator
			pointer->pidTargetPosition += pointer->currentPidSpeed * ENCODERINTSAMPLETIME;

			if(pointer->mode == PID)
			{
				pidTargetPositionTruncated = (int32_t)pointer->pidTargetPosition;
				stepsSinceResetPointer = &pidTargetPositionTruncated;
			}
			else
			{
				stepsSinceResetPointer = &pointer->stepsSinceReset;
			}

			if(pointer->continous == 1)
			{
				pointer->targetPosition = (float)pointer->pidTargetPosition;
			}
			else
			{
				if(pointer->targetPosition < 0)
				{
					if(pointer->pidTargetPosition < (float)pointer->targetPosition)
					{
						pointer->pidTargetPosition = (float)pointer->targetPosition;
					}
				}
				else
				{
					if(pointer->pidTargetPosition > (float)pointer->targetPosition)
					{
						pointer->pidTargetPosition = (float)pointer->targetPosition;
					}
				}
			}

			//acceleration profile generator
			if(pointer->state == INITDECEL)
			{
				if(pointer->direction == CCW)
				{
					pointer->currentPidAcceleration = -(pointer->acceleration * ENCODERINTSAMPLETIME);
					if(*stepsSinceResetPointer >= pointer->decelToAccelThreshold)
					{
						pointer->state = ACCEL;
					}
				}
				else
				{
					pointer->currentPidAcceleration = pointer->acceleration * ENCODERINTSAMPLETIME;
					if(*stepsSinceResetPointer <= pointer->decelToAccelThreshold)
					{
						pointer->state = ACCEL;
					}
				}

			}
			else if(pointer->state == ACCEL)
			{
				if(pointer->direction == CCW)
				{
					if(*stepsSinceResetPointer <= pointer->accelToCruiseThreshold)
					{
						pointer->state = CRUISE;
					}
					pointer->currentPidAcceleration = -(pointer->acceleration * ENCODERINTSAMPLETIME);
				}
				else
				{
					if(*stepsSinceResetPointer >= pointer->accelToCruiseThreshold)
					{
						pointer->state = CRUISE;
					}
					pointer->currentPidAcceleration = pointer->acceleration * ENCODERINTSAMPLETIME;
				}
			}
			else if(pointer->state == CRUISE)
			{
				if(pointer->continous == 1)
				{
					pointer->state = CRUISE;
				}
				else
				{
					if(pointer->direction == CCW)
					{
						if(*stepsSinceResetPointer <= pointer->cruiseToDecelThreshold)
						{
							pointer->state = DECEL;
						}
					}
					else
					{
						if(*stepsSinceResetPointer >= pointer->cruiseToDecelThreshold)
						{
							pointer->state = DECEL;
						}
					}
				}

				pointer->currentPidAcceleration = 0;

			}
			else if(pointer->state == DECEL)
			{
				if(pointer->direction == CW)
				{
					if(*stepsSinceResetPointer >= pointer->decelToStopThreshold)
					{
						pointer->state = STOP;
					}
					pointer->currentPidAcceleration = -(pointer->acceleration * ENCODERINTSAMPLETIME);
				}
				else
				{
					if(*stepsSinceResetPointer <= pointer->decelToStopThreshold)
					{
						pointer->state = STOP;
					}
					pointer->currentPidAcceleration = pointer->acceleration * ENCODERINTSAMPLETIME;
				}
			}
			else if(pointer->state == STOP)
			{
				pointer->currentPidAcceleration = 0.0;
				pointer->currentPidSpeed = 0.0;
				TCCR3B &= ~(1 << CS30);
				if(pointer->mode == NORMAL)
				{
					if(pointer->brake == BRAKEON)
					{
						PORTD &= ~(1 << 4);
					}
					else
					{
						PORTD |= (1 << 4);
					}
				}
			}

			if(pointer->mode == PID && !pointer->pidDisabled)
			{
				tempFloat = (float)pointer->encoder.angleMoved * pointer->stepConversion;
				pointer->stepsSinceReset = (int32_t)(tempFloat);
				pointer->pid((float)pointer->pidTargetPosition - tempFloat);
			}
			if(pointer->mode == NORMAL || pointer->pidDisabled)
			{
				if(pointer->currentPidSpeed > 5.0)
				{
					temp = (uint32_t)((STEPGENERATORFREQUENCY/(pointer->currentPidSpeed)) + 0.5);
					cli();
						pointer->stepDelay = temp;
					sei();
				}
				else if(pointer->currentPidSpeed < -5.0)
				{
					temp = (uint32_t)((STEPGENERATORFREQUENCY/(-pointer->currentPidSpeed)) + 0.5);
					cli();
						pointer->stepDelay = temp;
					sei();
				}
				else
				{
					cli();
						pointer->stepDelay = 20000;
					sei();
				}
			}

			pointer->detectStall();
		}
	}
}

uStepperEncoder::uStepperEncoder(void)
{
	I2C.begin();
}

float uStepperEncoder::getAngleMoved(void)
{
	return (float)this->angleMoved*0.087890625;
}

float uStepperEncoder::getSpeed(bool unit)
{
	if(unit == RPM)
	{
		return this->curSpeed * pointer->stepsPerSecondToRPM;
	}
	else	//StepsPerSecond
	{
		return this->curSpeed;
	}
}

void uStepperEncoder::setup()
{
	TCNT1 = 0;
	ICR1 = 32000;
	TIFR1 = 0;
	TIMSK1 = (1 << OCIE1A);
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);
}

void uStepperEncoder::setHome(void)
{
	cli();
        uint8_t data[2];
        TIMSK1 &= ~(1 << OCIE1A);
        I2C.read(ENCODERADDR, ANGLE, 2, data);
        TIMSK1 |= (1 << OCIE1A);
        this->encoderOffset = (((uint16_t)data[0]) << 8 ) | (uint16_t)data[1];
        pointer->stepsSinceReset = 0;
        this->angle = 0;
        this->oldAngle = 0;
        this->angleMoved = 0;
        pointer->pidTargetPosition = 0.0;
        pointer->targetPosition = 0;
        pointer->pidError = 0;
	sei();
}

float uStepperEncoder::getAngle()
{
	return (float)this->angle*0.087890625;
}

uint16_t uStepperEncoder::getStrength()
{
	uint8_t data[2];

	TIMSK1 &= ~(1 << OCIE1A);
	I2C.read(ENCODERADDR, MAGNITUDE, 2, data);
	TIMSK1 |= (1 << OCIE1A);

	return (((uint16_t)data[0]) << 8 )| (uint16_t)data[1];
}

uint8_t uStepperEncoder::getAgc()
{
	uint8_t data;
	TIMSK1 &= ~(1 << OCIE1A);
	I2C.read(ENCODERADDR, AGC, 1, &data);
	TIMSK1 |= (1 << OCIE1A);
	return data;
}

uint8_t uStepperEncoder::detectMagnet()
{
	uint8_t data;
	TIMSK1 &= ~(1 << OCIE1A);
	I2C.read(ENCODERADDR, STATUS, 1, &data);
	TIMSK1 |= (1 << OCIE1A);
	data &= 0x38;					//For some reason the encoder returns random values on reserved bits. Therefore we make sure reserved bits are cleared before checking the reply !

	if(data == 0x08)
	{
		return 1;					//magnet too strong
	}

	else if(data == 0x10)
	{
		return 2;					//magnet too weak
	}

	else if(data == 0x20)
	{
		return 0;					//magnet detected and within limits
	}

	return 3;						//Something went horribly wrong !
}

uStepperSLite::uStepperSLite(float accel, float vel)
{
	this->state = STOP;

	this->setMaxVelocity(vel);
	this->setMaxAcceleration(accel);

	pointer = this;

	DDRB |= (1 << 2);		//set direction pin to output
	DDRD |= (1 << 7);		//set step pin to output
	DDRD |= (1 << 4);		//set enable pin to output
}

void uStepperSLite::setMaxAcceleration(float accel)
{
	this->acceleration = accel;

	if(this->state != STOP)
	{
		if(this->continous == 1)	//If motor was running continously
		{
			this->runContinous(this->direction);	//We should make it run continously again
		}
		else						//If motor still needs to perform some steps
		{
			this->moveSteps(abs(this->targetPosition - this->stepsSinceReset + 1), this->direction, this->brake);	//we should make sure the motor gets to execute the remaining steps
		}
	}
}

void uStepperSLite::setMaxVelocity(float vel)
{

	if(vel < 0.5005)
	{
		this->velocity = 0.5005;			//Limit velocity in order to not overflow delay variable

	}

	else if(vel > 100000.0)
	{
		this->velocity = 100000.0;			//limit velocity in order to not underflow delay variable
	}

	else
	{
		this->velocity = vel;
	}

	if(this->state != STOP)		//If motor was running, we should make sure it runs again
	{
		
		if(this->continous == 1)	//If motor was running continously
		{
			this->runContinous(this->direction);	//We should make it run continously again
		}
		else					//If motor still needs to perform some steps
		{
			this->moveSteps(abs(this->targetPosition - this->stepsSinceReset + 1), this->direction, this->brake);	//we should make sure the motor gets to execute the remaining steps
		}
	}
}

void uStepperSLite::runContinous(bool dir)
{
	float curVel, startVelocity = 0;
	uint8_t tempState;
	uint32_t accelSteps;
	uint32_t initialDecelSteps;

	if(this->mode == DROPIN)
	{
		return;		//Drop in feature is activated. just return since this function makes no sense with drop in activated!
	}

	curVel = this->currentPidSpeed;

	if(this->state == STOP)											//If motor is currently running at desired speed
	{
		initialDecelSteps = 0;
		tempState = ACCEL;						//We should just run at cruise speed
		startVelocity = 0.0;//sqrt(2.0*this->acceleration);	//number of interrupts before the first step should be performed.
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));	//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))

	}
	else if((dir == CW && curVel < 0) || (dir == CCW && curVel > 0))									//If motor turns CCW and should turn CW, or if motor turns CW and shoúld turn CCW
	{
		tempState = INITDECEL;									//We should decelerate the motor to full stop
		initialDecelSteps = (uint32_t)((curVel*curVel)/(2.0*this->acceleration));		//the amount of steps needed to bring the motor to full stop. (S = (V^2 - V0^2)/(2*-a)))
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));									//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))

		startVelocity = curVel;//sqrt((curVel*curVel) + 2.0*this->acceleration);	//number of interrupts before the first step should be performed.
	}
	else if((dir == CW && curVel > 0) || (dir == CCW && curVel < 0))												//If the motor is currently rotating the same direction as the desired direction
	{
		if(abs(curVel) > this->velocity)						//If current velocity is greater than desired velocity
		{
			tempState = INITDECEL;						//We need to decelerate the motor to desired velocity
			initialDecelSteps = (uint32_t)(((this->velocity*this->velocity) - (curVel*curVel))/(-2.0*this->acceleration));		//Number of steps to bring the motor down from current speed to max speed (S = (V^2 - V0^2)/(2*-a)))
			accelSteps = 0;						//No acceleration phase is needed
		}

		else if(abs(curVel) < this->velocity)					//If the current velocity is less than the desired velocity
		{
			tempState = ACCEL;							//Start accelerating
			accelSteps = (uint32_t)(((this->velocity*this->velocity) - (curVel*curVel))/(2.0*this->acceleration));	//Number of Steps needed to accelerate from current velocity to full speed
			initialDecelSteps = 0;
		}

		else 											//If motor is currently running at desired speed
		{
			initialDecelSteps = 0;
			tempState = CRUISE;						//We should just run at cruise speed
			accelSteps = 0;						//No acceleration phase is needed
		}
	}
	else
	{
		initialDecelSteps = 0;
		tempState = ACCEL;						//We should just run at cruise speed
		startVelocity = 0.0;//sqrt(2.0*this->acceleration);	//number of interrupts before the first step should be performed.
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));	//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))
	}
	cli();
		this->direction = dir;
		this->stepGeneratorDirection = dir;

		this->continous = 1;

		if(dir == CW)
		{
			this->decelToAccelThreshold = this->stepsSinceReset + initialDecelSteps;
			this->accelToCruiseThreshold = this->decelToAccelThreshold + accelSteps;
			PORTB |= (1 << 2);
		}
		else
		{
			this->decelToAccelThreshold = this->stepsSinceReset - initialDecelSteps;
			this->accelToCruiseThreshold = this->decelToAccelThreshold - accelSteps;
			PORTB &= ~(1 << 2);
		}
		this->currentPidSpeed = startVelocity;
		if(pointer->currentPidSpeed > 5.0)
		{
			pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(pointer->currentPidSpeed)) + 0.5);
		}
		else if(pointer->currentPidSpeed < -5.0)
		{
			pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(-pointer->currentPidSpeed)) + 0.5);
		}
		else
		{
			pointer->stepDelay = 20000;
		}
		this->state = tempState;
	sei();

	PORTD &= ~(1 << 4);
	TCCR3B |= (1 << CS30);
}

void uStepperSLite::moveSteps(int32_t steps, bool dir, bool holdMode)
{
	float curVel, startVelocity = 0;
	uint8_t state;
	uint32_t totalSteps;
	uint32_t accelSteps;
	uint32_t decelSteps;
	uint32_t initialDecelSteps;
	uint32_t cruiseSteps = 0;

	if(this->mode == DROPIN)
	{
		return;		//Drop in feature is activated. just return since this function makes no sense with drop in activated!
	}
	
	if(steps < 1)
	{
		return;
	}
	//steps--;
	totalSteps = steps;
	cli();
		curVel = this->currentPidSpeed;
	sei();
	
	
	initialDecelSteps = 0;

	if(this->state == STOP)								//If motor is currently at full stop (state = STOP)
	{
		state = ACCEL;
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));	//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))
				//No initial deceleration phase needed

		if(accelSteps > (totalSteps >> 1))	//If we need to accelerate for longer than half of the total steps, we need to start decelerating before we reach max speed
		{
			cruiseSteps = 0; 		//No cruise phase needed
			accelSteps = decelSteps = (totalSteps >> 1);				//Accelerate and decelerate for the same amount of steps (half the total steps)
			accelSteps += totalSteps - accelSteps - decelSteps;	//if there are still a step left to perform, due to rounding errors, do this step as an acceleration step
		}

		else
		{
			decelSteps = accelSteps;	//If top speed is reached before half the total steps are performed, deceleration period should be same length as acceleration period
			cruiseSteps = totalSteps - accelSteps - decelSteps;	//Perform remaining steps as cruise steps
		}
		startVelocity = 0.0;//sqrt(2.0*this->acceleration);	//number of interrupts before the first step should be performed.
	}
	else if((dir == CW && curVel < 0) || (dir == CCW && curVel > 0))									//If motor turns CCW and should turn CW, or if motor turns CW and shoúld turn CCW
	{
		state = INITDECEL;									//We should decelerate the motor to full stop
		initialDecelSteps = (uint32_t)((curVel*curVel)/(2.0*this->acceleration));		//the amount of steps needed to bring the motor to full stop. (S = (V^2 - V0^2)/(2*-a)))
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));									//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))
		totalSteps += initialDecelSteps;				//Add the steps used for initial deceleration to the totalSteps variable, since we moved this number of steps, passed the initial position, and therefore need to move this amount of steps extra, in the desired direction

		if(accelSteps > (totalSteps >> 1))			//If we need to accelerate for longer than half of the total steps, we need to start decelerating before we reach max speed
		{
			accelSteps = decelSteps = (totalSteps >> 1);	//Accelerate and decelerate for the same amount of steps (half the total steps)
			accelSteps += totalSteps - accelSteps - decelSteps;				//If there are still a step left to perform, due to rounding errors, do this step as an acceleration step
			cruiseSteps = 0;
		}
		else
		{
			decelSteps = accelSteps;					//If top speed is reached before half the total steps are performed, deceleration period should be same length as acceleration period
			cruiseSteps = totalSteps - accelSteps - decelSteps; 			//Perform remaining steps, as cruise steps
		}
		startVelocity = curVel; //sqrt((curVel*curVel) + 2.0*this->acceleration);	//number of interrupts before the first step should be performed.
	}
	else if((dir == CW && curVel > 0) || (dir == CCW && curVel < 0))							//If the motor is currently rotating the same direction as desired, we dont necessarily need to decelerate
	{
		if(abs(curVel) > this->velocity)	//If current velocity is greater than desired velocity
		{
			state = INITDECEL;	//We need to decelerate the motor to desired velocity
			initialDecelSteps = (uint32_t)(((this->velocity*this->velocity) - (curVel*curVel))/(-2.0*this->acceleration));		//Number of steps to bring the motor down from current speed to max speed (S = (V^2 - V0^2)/(2*-a)))
			accelSteps = 0;	//No acceleration phase is needed
			decelSteps = (uint32_t)((this->velocity*this->velocity)/(2.0*this->acceleration));	//Number of steps needed to decelerate the motor from top speed to full stop
			startVelocity = curVel;//sqrt((curVel*curVel) + (2.0*this->acceleration));
			if(totalSteps <= (initialDecelSteps + decelSteps))
			{
				cruiseSteps = 0;
			}
			else
			{
				cruiseSteps = steps - initialDecelSteps - decelSteps;					//Perform remaining steps as cruise steps
			}
		}

		else if(abs(curVel) < this->velocity)	//If current velocity is less than desired velocity
		{
			state = ACCEL;			//Start accelerating
			accelSteps = (int32_t)((((this->velocity*this->velocity) - curVel*curVel))/(2.0*this->acceleration));	//Number of Steps needed to accelerate from current velocity to full speed

			if(accelSteps > (totalSteps >> 1))			//If we need to accelerate for longer than half of the total steps, we need to start decelerating before we reach max speed
			{
				accelSteps = decelSteps = (totalSteps >> 1);	//Accelerate and decelerate for the same amount of steps (half the total steps)
				accelSteps += totalSteps - accelSteps - decelSteps;				//If there are still a step left to perform, due to rounding errors, do this step as an acceleration step
				cruiseSteps = 0;
			}
			else
			{
				decelSteps = accelSteps;					//If top speed is reached before half the total steps are performed, deceleration period should be same length as acceleration period
				cruiseSteps = totalSteps - accelSteps - decelSteps; 			//Perform remaining steps, as cruise steps
			}

			cruiseSteps = totalSteps - accelSteps - decelSteps;	//Perform remaining steps as cruise steps
			initialDecelSteps = 0;								//No initial deceleration phase needed
		}

		else						//If current velocity is equal to desired velocity
		{
			state = CRUISE;	//We are already at desired speed, therefore we start at cruise phase
			decelSteps = (int32_t)((this->velocity*this->velocity)/(2.0*this->acceleration));	//Number of steps needed to decelerate the motor from top speed to full stop
			accelSteps = 0;	//No acceleration phase needed
			initialDecelSteps = 0;		//No initial deceleration phase needed

			if(decelSteps >= totalSteps)
			{
				cruiseSteps = 0;
				decelSteps = totalSteps;
			}
			else
			{
				cruiseSteps = totalSteps - decelSteps;	//Perform remaining steps as cruise steps
			}
		}
		startVelocity = curVel;
	}
	else
	{
		state = ACCEL;
		accelSteps = (uint32_t)((this->velocity * this->velocity)/(2.0*this->acceleration));	//Number of steps to bring the motor to max speed (S = (V^2 - V0^2)/(2*a)))
				//No initial deceleration phase needed

		if(accelSteps > (totalSteps >> 1))	//If we need to accelerate for longer than half of the total steps, we need to start decelerating before we reach max speed
		{
			cruiseSteps = 0; 		//No cruise phase needed
			accelSteps = decelSteps = (totalSteps >> 1);				//Accelerate and decelerate for the same amount of steps (half the total steps)
			accelSteps += totalSteps - accelSteps - decelSteps;	//if there are still a step left to perform, due to rounding errors, do this step as an acceleration step
		}

		else
		{
			decelSteps = accelSteps;	//If top speed is reached before half the total steps are performed, deceleration period should be same length as acceleration period
			cruiseSteps = totalSteps - accelSteps - decelSteps;	//Perform remaining steps as cruise steps
		}
		startVelocity = 0.0;
	}
	cli();
		this->direction = dir;
		this->stepGeneratorDirection = dir;

		this->continous = 0;

		if(dir == CW)
		{
			this->decelToAccelThreshold = this->stepsSinceReset + initialDecelSteps;
			this->accelToCruiseThreshold = this->decelToAccelThreshold + accelSteps;
			this->cruiseToDecelThreshold = this->accelToCruiseThreshold + cruiseSteps;
			this->decelToStopThreshold = this->cruiseToDecelThreshold + decelSteps;
			PORTB |= (1 << 2);
		}
		else
		{
			this->decelToAccelThreshold = this->stepsSinceReset - initialDecelSteps;
			this->accelToCruiseThreshold = this->decelToAccelThreshold - accelSteps;
			this->cruiseToDecelThreshold = this->accelToCruiseThreshold - cruiseSteps;
			this->decelToStopThreshold = this->cruiseToDecelThreshold - decelSteps;
			PORTB &= ~(1 << 2);
		}
		this->currentPidSpeed = startVelocity;
		if(pointer->currentPidSpeed > 5.0)
		{
			pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(pointer->currentPidSpeed)) + 0.5);
		}
		else if(pointer->currentPidSpeed < -5.0)
		{
			pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(-pointer->currentPidSpeed)) + 0.5);
		}
		else
		{
			pointer->stepDelay = 20000;
		}
		this->state = state;
		this->targetPosition = this->decelToStopThreshold;
		this->brake = holdMode;
	sei();

	PORTD &= ~(1 << 4);
	TCCR3B |= (1 << CS30);
}

void uStepperSLite::hardStop(bool holdMode)
{
	if(this->mode == DROPIN)
	{
		return;		//Drop in feature is activated. just return since this function makes no sense with drop in activated!
	}

	cli();
	
	this->stepsSinceReset = (int32_t)((float)this->encoder.angleMoved * this->stepConversion);
	this->targetPosition = this->stepsSinceReset;
	this->pidTargetPosition = this->targetPosition;
	this->decelToStopThreshold = this->targetPosition;

	this->state = DECEL;
	this->brake = holdMode;
	this->continous = 0;
	this->stepDelay = 2;
	
	sei();

	PORTD &= ~(1 << 4);
	TCCR3B |= (1 << CS30);
}

void uStepperSLite::stop(bool brake)
{
	this->softStop(brake);
}

void uStepperSLite::softStop(bool holdMode)
{
	uint32_t decelSteps = 0;
	float curVel = 0.0;

	if(this->mode == DROPIN)
	{
		return;		//Drop in feature is activated. just return since this function makes no sense with drop in activated!
	}

	cli();
	curVel = this->currentPidSpeed;
	decelSteps = (uint32_t)((curVel*curVel)/(2.0*this->acceleration));
	if(this->direction == CW)
	{
		this->targetPosition = this->stepsSinceReset + decelSteps;
		pointer->decelToStopThreshold = this->targetPosition;
	}
	else
	{
		this->targetPosition = this->stepsSinceReset - decelSteps;
		pointer->decelToStopThreshold = this->targetPosition;
	}

	this->state = DECEL;
	this->brake = holdMode;
	this->continous = 0;
	this->currentPidSpeed = curVel;
	if(pointer->currentPidSpeed > 5.0)
	{
		pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(pointer->currentPidSpeed)) + 0.5);
	}
	else if(pointer->currentPidSpeed < -5.0)
	{
		pointer->stepDelay = (uint32_t)((STEPGENERATORFREQUENCY/(-pointer->currentPidSpeed)) + 0.5);
	}
	else
	{
		pointer->stepDelay = 20000;
	}
	sei();

	PORTD &= ~(1 << 4);
	TCCR3B |= (1 << CS30);
}

void uStepperSLite::checkConnectorOrientation(uint8_t mode)
{
	uint8_t data[2], i;
	uint16_t angle;
	int16_t angleDiff[3];

	I2C.read(ENCODERADDR, ANGLE, 2, data);
	angle = (((uint16_t)data[0]) << 8 ) | (uint16_t)data[1];

	PORTB &= ~(1 << 2);

	for(i = 0; i < 50; i++)
	{
		PORTD |= (1 << 7);
		delayMicroseconds(1);
		PORTD &= ~(1 << 7);
		_delay_ms(10);
	}

	angleDiff[0] = (int16_t)angle;

	I2C.read(ENCODERADDR, ANGLE, 2, data);
	angle = (((uint16_t)data[0]) << 8 ) | (uint16_t)data[1];

	angleDiff[0] -= (int16_t)angle;

	PORTB |= (1 << 2);

	for(i = 0; i < 50; i++)
	{
		PORTD |= (1 << 7);
		delayMicroseconds(1);
		PORTD &= ~(1 << 7);
		_delay_ms(10);
	}

	angleDiff[1] = (int16_t)angle;

	I2C.read(ENCODERADDR, ANGLE, 2, data);
	angle = (((uint16_t)data[0]) << 8 ) | (uint16_t)data[1];

	angleDiff[1] -= (int16_t)angle;

	for(i = 0; i < 2; i++)
	{
		if(angleDiff[i] > 2048)
		{
			angleDiff[i] -= 4096;
		}
		else if(angleDiff[i] < -2048)
		{
			angleDiff[i] += 4096;
		}
	}

	if((angleDiff[0] > 2 && angleDiff[1] < -2))
	{
		if(mode == DROPIN)
		{
			this->driver.invertDirection();
		}
	}
	else
	{
		if(mode != DROPIN)
		{
			this->driver.invertDirection();
		}
	}
}

void uStepperSLite::setup(	uint8_t mode = NORMAL, 
				float stepsPerRevolution = 3200.0, 
				float pTerm = 0.75, 
				float iTerm = 3.0, 
				float dTerm = 0.0,
				bool setHome = true,
				uint8_t invert,
				uint8_t runCurrent,
				uint8_t holdCurrent)
{
	dropinCliSettings_t tempSettings;

	p = &(this->stepsSinceReset);
	this->pidDisabled = 1;
	this->mode = mode;
	this->encoder.setup();

	this->state = STOP;
	this->driver.setup();
	this->driver.enableDriver();
	_delay_ms(200);

	if((uint16_t)stepsPerRevolution == FULL)
	{
		stepsPerRevolution = 200.0;
	}
	else if((uint16_t)stepsPerRevolution == HALF)
	{
		stepsPerRevolution = 400.0;
	}
	else if((uint16_t)stepsPerRevolution == QUARTER)
	{
		stepsPerRevolution = 800.0;
	}
	else if((uint16_t)stepsPerRevolution == EIGHT)
	{
		stepsPerRevolution = 1600.0;
	}
	else if((uint16_t)stepsPerRevolution == SIXTEEN)
	{
		stepsPerRevolution = 3200.0;
	}

	this->stepConversion = (float)(stepsPerRevolution)/4096.0;	//Calculate conversion coefficient from raw encoder data, to actual moved steps
	this->angleToStep = ((float)(stepsPerRevolution))/360.0;	//Calculate conversion coefficient from angle to corresponding number of steps
	this->stepToAngle = 360.0/((float)(stepsPerRevolution));	//Calculate conversion coefficient from steps to corresponding angle
	this->stepsPerSecondToRPM = 60.0/stepsPerRevolution;
	this->RPMToStepsPerSecond = stepsPerRevolution/60.0;
	this->RPMToStepDelay = STEPGENERATORFREQUENCY/this->RPMToStepsPerSecond;
	this->encoder.setHome();

	if(this->mode)
	{
		if(this->mode == DROPIN)
		{
			//Set Enable, Step and Dir signal pins from 3dPrinter controller as inputs
			pinMode(2,INPUT);
			pinMode(3,INPUT);
			pinMode(4,INPUT);
			//Enable internal pull-up resistors on the above pins
			digitalWrite(2,HIGH);
			digitalWrite(3,HIGH);
			digitalWrite(4,HIGH);
			EICRA = 0x06;
			EIMSK = 0x03;

			Serial.begin(9600);

			tempSettings.P.f = pTerm;
			tempSettings.I.f = iTerm;
			tempSettings.D.f = dTerm;
			tempSettings.invert = invert;
			tempSettings.runCurrent = runCurrent;
			tempSettings.holdCurrent = holdCurrent;
			tempSettings.checksum = this->dropinSettingsCalcChecksum(&tempSettings);

			if(tempSettings.checksum != EEPROM.read(sizeof(dropinCliSettings_t)))
			{
				this->dropinSettings = tempSettings;
				this->saveDropinSettings();
				EEPROM.put(sizeof(dropinCliSettings_t),this->dropinSettings.checksum);
				this->loadDropinSettings();
			}
			else
			{
				if(!this->loadDropinSettings())
				{
					this->dropinSettings = tempSettings;
					this->saveDropinSettings();
					EEPROM.put(sizeof(dropinCliSettings_t),this->dropinSettings.checksum);
					this->loadDropinSettings();
				}
			}
			delay(10000);
  			this->dropinPrintHelp();
		}
		else
		{
			//Scale supplied controller coefficents. This is done to enable the user to use easier to manage numbers for these coefficients.
		    this->pTerm = pTerm;
			this->iTerm = iTerm * ENCODERINTSAMPLETIME;
		    this->dTerm = dTerm * ENCODERINTFREQ;
		}
	}

	this->checkConnectorOrientation(mode);
	this->encoder.setHome();

	this->pidDisabled = 0;
	TCNT3 = 0;
	ICR3 = 159;
	TIFR3 = 0;
	TIMSK3 = (1 << OCIE3A);
	TCCR3A = (1 << WGM31);
	TCCR3B = (1 << WGM32) | (1 << WGM33);
	sei();
}

void uStepperSLite::enableMotor(void)
{
	this->driver.enableDriver();				//Enable motor driver
}

void uStepperSLite::disableMotor(void)
{
	this->driver.disableDriver();			//Disable motor driver
}

bool uStepperSLite::getCurrentDirection(void)
{
	return this->direction;
}

uint8_t uStepperSLite::getMotorState(void)
{
	if(this->state != STOP)
	{
		return this->state;		//Motor running
	}

	return 0;			//Motor not running
}

int32_t uStepperSLite::getStepsSinceReset(void)
{
	return this->stepsSinceReset;
}

void uStepperSLite::setCurrent(uint8_t runCurrent, uint8_t holdCurrent)
{
	this->driver.setCurrent(runCurrent, holdCurrent);
}

void uStepperSLite::setHoldCurrent(uint8_t holdCurrent)
{
	this->driver.setHoldCurrent(holdCurrent);
}

void uStepperSLite::setRunCurrent(uint8_t runCurrent)
{
	this->driver.setRunCurrent(runCurrent);
}

//Skal Gennemgås !
float uStepperSLite::moveToEnd(bool dir, float stallSensitivity)
{
	uint8_t checks = 0;
  	float pos = 0.0;
  	float lengthMoved;

  	if(this->mode == DROPIN)
  	{
  		return 0.0;		//Doesn't make sense in dropin mode
  	}

  	lengthMoved = this->encoder.getAngleMoved();

  	this->stop(HARD);
	_delay_ms(50);
  	this->runContinous(dir);
  	_delay_ms(200);
	while(!this->isStalled(stallSensitivity));
	this->stop(SOFT);//stop motor without brake

	this->moveSteps(20, !dir, SOFT);
	while(this->getMotorState())
	{
		_delay_ms(1);
	}
	_delay_ms(100);
	if(dir == CW)
	{
		lengthMoved = this->encoder.getAngleMoved() - lengthMoved;
	}
	else
	{
		lengthMoved -= this->encoder.getAngleMoved();
	}
  	this->encoder.setHome();//set new home position

  	return lengthMoved;
}

void uStepperSLite::moveToAngle(float angle, bool holdMode)
{
	float diff;
	uint32_t steps;

	diff = angle - this->encoder.getAngleMoved();
	steps = (uint32_t)((abs(diff)*angleToStep) + 0.5);

	if(diff < 0.0)
	{
		this->moveSteps(steps, CCW, holdMode);
	}
	else
	{
		this->moveSteps(steps, CW, holdMode);
	}
}

void uStepperSLite::moveAngle(float angle, bool holdMode)
{
	int32_t steps;

	if(angle < 0.0)
	{
		steps = -(int32_t)((angle*angleToStep) - 0.5);
		this->moveSteps(steps, CCW, holdMode);
	}
	else
	{
		steps = (int32_t)((angle*angleToStep) + 0.5);
		this->moveSteps(steps, CW, holdMode);
	}
}

void uStepperSLite::pid(float error)
{
	float u, uSat, temp;
	float limit = abs(this->currentPidSpeed) + 6000.0;
	static float integral;
	static bool integralReset = 0;
	
	if(this->pidDisabled)
	{
		integral = 0.0;
		integralReset = 0;
		this->currentPidError = 0.0;
		if(this->state == STOP)
		{
			if(this->brake == BRAKEON)
			{
				PORTD &= ~(1 << 4);
			}
			else
			{
				PORTD |= (1 << 4);
			}
		}
		
		return;
	}

	PORTD &= ~(1 << 4);

	this->currentPidError = error;

	u = error*this->pTerm;

	if(u > 0.0)
	{
		if(u > limit)
		{
			u = limit;
		}
	}
	else if(u < 0.0)
	{
		if(u < -limit)
		{
			u = -limit;
		}
	}

	integral += error*this->iTerm;

	if(integral > 10000.0)
	{
		integral = 10000.0;
	}
	else if(integral < -10000.0)
	{
		integral = -10000.0;
	}

	u += integral;

	if(u > 100000 || u < -100000)
	{
		uSat = 100000;
		uSat = ((u > 0) - (u < 0)) * uSat;
	}
	else
	{
		uSat = u;
	}

	if(error < -2.0 || error > 2.0)
	{
		if(error < 0.0)
		{
			if(error < -255.0)
			{
				this->pidError = 255;
			}
			else
			{
				this->pidError = (uint8_t)-error;
			}
		}
		else
		{
			if(error > 255.0)
			{
				this->pidError = 255;
			}
			else
			{
				this->pidError = (uint8_t)error;
			}
		}
		TCCR3B |= (1 << CS30);
	}
	else
	{

		if(this->state == STOP)
		{
			TCCR3B &= ~(1 << CS30);
		}
		this->pidError = 0;
	}

	if(uSat > 5.0)
	{
		temp = (uint32_t)((STEPGENERATORFREQUENCY/uSat) + 0.5);
		this->stepGeneratorDirection = CW;

		cli();
		pointer->stepDelay = temp;
		sei();
	}
	else if(uSat < -5.0)
	{
		this->stepGeneratorDirection = CCW;

		temp = (uint32_t)((STEPGENERATORFREQUENCY/-uSat) + 0.5);
		cli();
		pointer->stepDelay = temp;
		sei();
	}
	else if(uSat > 0.0)
	{
		this->stepGeneratorDirection = CW;
		cli();
		pointer->stepDelay = 20000;
		sei();
	}
	else if(uSat < 0.0)
	{
		this->stepGeneratorDirection = CCW;

		cli();
		pointer->stepDelay = 20000;
		sei();
	}
	else
	{
		this->stepGeneratorDirection = this->direction;
		cli();
		pointer->stepDelay = 20000;
		sei();
	}
}

void uStepperSLite::disablePid(void)
{
	this->pidDisabled = 1;
}

void uStepperSLite::enablePid(void)
{
	cli();
		this->pidDisabled = 0;
		this->pidTargetPosition = this->encoder.angleMoved * this->stepConversion;
		this->targetPosition = this->pidTargetPosition;
		this->state = STOP;
		this->pidError = 0;
	sei();
}

float uStepperSLite::getPidError(void)
{
	return this->currentPidError;
}

void uStepperSLite::pidDropin(float error)
{
	float u;
	float limit = abs(this->currentPidSpeed) + 6000.0;
	static float integral;
	static bool integralReset = 0;

	PORTD &= ~(1 << 4);

	this->currentPidError = error;

	u = error*this->pTerm;

	if(u > 0.0)
	{
		if(u > limit)
		{
			u = limit;
		}
	}
	else if(u < 0.0)
	{
		if(u < -limit)
		{
			u = -limit;
		}
	}

	integral += error*this->iTerm;

	if(integral > 10000.0)
	{
		integral = 10000.0;
	}
	else if(integral < -10000.0)
	{
		integral = -10000.0;
	}

	if(error > -10 && error < 10)
	{
		if(!integralReset)
		{
			integralReset = 1;
			integral = 0;
		}
	}
	else
	{
		integralReset = 0;
	}

	u += integral;
	u *= this->stepsPerSecondToRPM;

	this->driver.setVelocity(u);
}

bool uStepperSLite::detectStall(void)
{
	static float oldTargetPosition;
	static float oldEncoderPosition;
	static float encoderPositionChange;
	static float targetPositionChange;
	float encoderPosition = ((float)this->encoder.angleMoved*this->stepConversion);
	static float internalStall = 0.0;

	encoderPositionChange *= 0.99;
	encoderPositionChange += 0.01*(oldEncoderPosition - encoderPosition);
	oldEncoderPosition = encoderPosition;

	targetPositionChange *= 0.99;
	targetPositionChange += 0.01*(oldTargetPosition - this->pidTargetPosition);
	oldTargetPosition = this->pidTargetPosition;

	if(abs(encoderPositionChange) < abs(targetPositionChange)*0.5)
	{
		internalStall *= this->stallSensitivity;
		internalStall += 1.0-this->stallSensitivity;
	}
	else
	{
		internalStall *= this->stallSensitivity;
	}
	
	if(internalStall >= 0.95)		//3 timeconstants
	{
		this->stall = 1;
	}
	else
	{
		this->stall = 0;
	}
}

bool uStepperSLite::isStalled(float stallSensitivity)
{
  	if(this->stallSensitivity > 1.0)
  	{
  		this->stallSensitivity = 1.0;
  	}
  	else if(this->stallSensitivity < 0.0)
  	{
  		this->stallSensitivity = 0.0;
  	}
  	else{
  		this->stallSensitivity = stallSensitivity;
  	}
  	
	return this->stall;
}

void uStepperSLite::setProportional(float P)
{
	this->pTerm = P;
}

void uStepperSLite::setIntegral(float I)
{
	this->iTerm = I * ENCODERINTSAMPLETIME; 
}

void uStepperSLite::setDifferential(float D)
{
	this->dTerm = D * ENCODERINTFREQ;
}

void uStepperSLite::invertDropinDir(bool invert)
{
	this->invertPidDropinDirection = invert;
}

void uStepperSLite::parseCommand(String *cmd)
{
  uint8_t i = 0;
  String value;

  if(cmd->charAt(2) == ';')
  {
    Serial.println("COMMAND NOT ACCEPTED");
    return;
  }

  value.remove(0);
  /****************** SET P Parameter ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  if(cmd->substring(0,2) == String("P="))
  {
    for(i = 2;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == '.')
      {
        value.concat(cmd->charAt(i));
        i++;
        break;
      }
      else if(cmd->charAt(i) == ';')
      {
        break;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
    
    for(;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == ';')
      {
        Serial.print("COMMAND ACCEPTED. P = ");
        Serial.println(value.toFloat(),4);
        this->dropinSettings.P.f = value.toFloat();
    	this->saveDropinSettings();
        this->setProportional(value.toFloat());
        return;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
  }

/****************** SET I Parameter ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,2) == String("I="))
  {
    for(i = 2;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == '.')
      {
        value.concat(cmd->charAt(i));
        i++;
        break;
      }
      else if(cmd->charAt(i) == ';')
      {
        break;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
    
    for(;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == ';')
      {
        Serial.print("COMMAND ACCEPTED. I = ");
        Serial.println(value.toFloat(),4);

        this->dropinSettings.I.f = value.toFloat();
    	this->saveDropinSettings();
        this->setIntegral(value.toFloat());
        return;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
  }

/****************** SET D Parameter ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,2) == String("D="))
  {
    for(i = 2;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == '.')
      {
        value.concat(cmd->charAt(i));
        i++;
        break;
      }
      else if(cmd->charAt(i) == ';')
      {
        break;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
    
    for(;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == ';')
      {
        Serial.print("COMMAND ACCEPTED. D = ");
        Serial.println(value.toFloat(),4);
        this->dropinSettings.D.f = value.toFloat();
    	this->saveDropinSettings();
        this->setDifferential(value.toFloat());
        return;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
  }

/****************** invert Direction ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,6) == String("invert"))
  {
      if(cmd->charAt(6) != ';')
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
      if(this->invertPidDropinDirection)
      {
      	Serial.println(F("Direction normal!"));
      	this->dropinSettings.invert = 0;
    	this->saveDropinSettings();
        this->invertDropinDir(0);
        return;
      }
      else
      {
      	Serial.println(F("Direction inverted!"));
      	this->dropinSettings.invert = 1;
    	this->saveDropinSettings();
        this->invertDropinDir(1);
        return;
      }
  }

  /****************** get Current Pid Error ********************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,5) == String("error"))
  {
      if(cmd->charAt(5) != ';')
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
      Serial.print(F("Current Error: "));
      Serial.print(this->getPidError());
      Serial.println(F(" Steps"));
  }

  /****************** Get run/hold current settings ************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,7) == String("current"))
  {
      if(cmd->charAt(7) != ';')
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
      Serial.print(F("Run Current: "));
      Serial.print(this->driver.getRunCurrent());
      Serial.println(F(" %"));
      Serial.print(F("Hold Current: "));
      Serial.print(this->driver.getHoldCurrent());
      Serial.println(F(" %"));
  }

  /****************** Get PID Parameters ***********************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,10) == String("parameters"))
  {
      if(cmd->charAt(10) != ';')
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
      Serial.print(F("P: "));
      Serial.print(this->dropinSettings.P.f,4);
      Serial.print(F(", "));
      Serial.print(F("I: "));
      Serial.print(this->dropinSettings.I.f,4);
      Serial.print(F(", "));
      Serial.print(F("D: "));
      Serial.println(this->dropinSettings.D.f,4);
  }

  /****************** Help menu ********************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,4) == String("help"))
  {
      if(cmd->charAt(4) != ';')
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
      this->dropinPrintHelp();
  }
  
/****************** SET run current ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,11) == String("runCurrent="))
  {
    for(i = 11;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == '.')
      {
        value.concat(cmd->charAt(i));
        i++;
        break;
      }
      else if(cmd->charAt(i) == ';')
      {
        break;
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }
    
    for(;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == ';')
      {
      	if(value.toFloat() >= 0.0 && value.toFloat() <= 100.0)
      	{
      		i = (uint8_t)value.toFloat();
    		break;	
      	}
      	else
      	{
      		Serial.println("COMMAND NOT ACCEPTED");
        	return;
      	}
      }
      else
      {
        Serial.println("COMMAND NOT ACCEPTED");
        return;
      }
    }

    Serial.print("COMMAND ACCEPTED. runCurrent = ");
    Serial.print(i);
    Serial.println(F(" %"));
    this->dropinSettings.runCurrent = i;
    this->saveDropinSettings();
    this->driver.setRunCurrent(i);
  }

  /****************** SET run current ***************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else if(cmd->substring(0,12) == String("holdCurrent="))
  {
    for(i = 12;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == '.')
      {
        value.concat(cmd->charAt(i));
        i++;
        break;
      }
      else if(cmd->charAt(i) == ';')
      {
        break;
      }
      else
      {
        Serial.println(F("COMMAND NOT ACCEPTED"));
        return;
      }
    }
    
    for(;;i++)
    {
      if(cmd->charAt(i) >= '0' && cmd->charAt(i) <= '9')
      {
        value.concat(cmd->charAt(i));
      }
      else if(cmd->charAt(i) == ';')
      {
      	if(value.toFloat() >= 0.0 && value.toFloat() <= 100.0)
      	{
      		i = (uint8_t)value.toFloat();
    		break;	
      	}
      	else
      	{
      		Serial.println(F("COMMAND NOT ACCEPTED"));
        	return;
      	}
      }
      else
      {
        Serial.println(F("COMMAND NOT ACCEPTED"));
        return;
      }
    }

    Serial.print(F("COMMAND ACCEPTED. holdCurrent = "));
    Serial.print(i);
    Serial.println(F(" %"));
    this->dropinSettings.holdCurrent = i;
    this->saveDropinSettings();
    this->driver.setHoldCurrent(i);
  }

  /****************** DEFAULT (Reject!) ************************
  *                                                            *
  *                                                            *
  **************************************************************/
  else
  {
    Serial.println(F("COMMAND NOT ACCEPTED"));
    return;
  }
  
}

void uStepperSLite::dropinCli()
{
	static String stringInput;
	static uint32_t t = millis();

	while(1)
	{
		while(!Serial.available())
		{
			delay(1);
			if((millis() - t) >= 500)
			{
				stringInput.remove(0);
				t = millis();
			}
		}
		t = millis();
		stringInput += (char)Serial.read();
		if(stringInput.lastIndexOf(';') > -1)
		{
		  this->parseCommand(&stringInput);
		  stringInput.remove(0);
		}
	}
}

void uStepperSLite::dropinPrintHelp()
{
	Serial.println(F("uStepper S-lite Dropin !"));
	Serial.println(F(""));
	Serial.println(F("Usage:"));
	Serial.println(F("Show this command list: 'help;'"));
	Serial.println(F("Get PID Parameters: 'parameters;'"));
	Serial.println(F("Set Proportional constant: 'P=10.002;'"));
	Serial.println(F("Set Integral constant: 'I=10.002;'"));
	Serial.println(F("Set Differential constant: 'D=10.002;'"));
	Serial.println(F("Invert Direction: 'invert;'"));
	Serial.println(F("Get Current PID Error: 'error;'"));
	Serial.println(F("Get Run/Hold Current Settings: 'current;'"));
	Serial.println(F("Set Run Current (percent): 'runCurrent=50.0;'"));
	Serial.println(F("Set Hold Current (percent): 'holdCurrent=50.0;'"));
	Serial.println(F(""));
	Serial.println(F(""));
}

bool uStepperSLite::loadDropinSettings(void)
{
	dropinCliSettings_t tempSettings;

	EEPROM.get(0,tempSettings);

	if(this->dropinSettingsCalcChecksum(&tempSettings) != tempSettings.checksum)
	{
		return 0;
	}

	this->dropinSettings = tempSettings;
	this->setProportional(this->dropinSettings.P.f);
	this->setIntegral(this->dropinSettings.I.f);
	this->setDifferential(this->dropinSettings.D.f);
	this->invertDropinDir((bool)this->dropinSettings.invert);
	this->setCurrent(this->dropinSettings.runCurrent,this->dropinSettings.holdCurrent);	
	return 1;
}

void uStepperSLite::saveDropinSettings(void)
{
	this->dropinSettings.checksum = this->dropinSettingsCalcChecksum(&this->dropinSettings);

	EEPROM.put(0,this->dropinSettings);
}

uint8_t uStepperSLite::dropinSettingsCalcChecksum(dropinCliSettings_t *settings)
{
	uint8_t i;
	uint8_t checksum = 0xAA;
	uint8_t *p = (uint8_t*)settings;

	for(i=0; i < 15; i++)
	{		
		checksum ^= *p++;
	}

	return checksum;
}