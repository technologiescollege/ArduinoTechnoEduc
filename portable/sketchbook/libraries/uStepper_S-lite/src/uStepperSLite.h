/********************************************************************************************
* 	 	File: 		uStepperSLite.h 														*
*		Version:    1.0.0                                           						*
*      	Date: 		April 29th, 2019 	                                    				*
*      	Author: 	Thomas Hørring Olsen                                   					*
*                                                   										*	
*********************************************************************************************
*	(C) 2019																				*
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
* @file uStepperSLite.h
*
* @brief      Function prototypes and definitions for the uStepper S-lite library
*
*             This file contains class and function prototypes for the library,
*             as well as necessary constants and global variables.
*
* @author     Thomas Hørring Olsen (thomas@ustepper.com)
*
*	\mainpage Arduino library for the uStepper S-lite Board
*	
*	This is the uStepper S-lite Arduino library, providing software functions for the different features of the uStepper S-lite board.
*
*	\par Features
*	The uStepper S-lite library contains the following features:
*	
*	- Second order acceleration profile, to provide smooth ramping of the speed and to avoid the motor stalling when demanding high speeds.
*	- Closed loop PID position controller
*	- Control of RC servo motors
*	- Measure the current position of the shaft (absolute, multiple revolutions)
*	- Measure the current speed of the motor 
*	- Stall detection for use in e.g. limit detection functionality 
*	
*	The library uses both timer one and timer three in order to function properly, meaning that unless the user of this library
*	can accept the loss of some functionality, these two timers are unavailable and the registers associated with these timers
*	should not be reconfigured.
*
*	Timer one is used for sampling the encoder in order to provide the ability to keep track of both the current speed and the
*	angle moved since the board was reset (or a new home position was configured). Also the drop-in features missed step detection and 
*	correction is done in this timer. 
*	
*	Timer three is used to calculate the stepper acceleration algorithm.  
* 
*	\par Installation
*	To install the uStepper S-lite library into the Arduino IDE, perform the following steps:
*
*	- Go to Sketch->Include Libraries->Manage Libraries... in the arduino IDE
*	- Search for "uStepper S-lite", in the top right corner of the "Library Manager" window
*	- Install uStepper S-lite library 
*	
*	The library is tested with Arduino IDE 1.8.5
*	
*	\warning MAC users should be aware, that OSX does NOT include FTDI VCP drivers, needed to upload sketches to the uStepper S-lite, by default. This driver should be 
*	downloaded and installed from FTDI's website:
*	\warning http://www.ftdichip.com/Drivers/VCP.htm
*	\warning             The uStepper S-lite should NOT be connected to the USB port while installing this driver !
*	\warning This is not (commonly) a problem for windows/linux users, as these drivers are most often already included in the OS
*
*	\par Theory
*
*	The acceleration profile implemented in this library is a second order profile, meaning that the stepper will accelerate with a constant acceleration, the velocity will follow
*	a linear function and the position will look like an S-curve (it will follow a linear function at top speed and a second order function during acceleration and deceleration).
*	As a result, the position of the stepper motor will have continous and differentiable first and second derivatives.
*
*	The second order acceleration profile can be illustrated as follows (<a rel="license" href="http://picprog.strongedge.net/step_prof/step-profile.html">Source</a>):
*
*	\image html dva.gif
*
*	From the above picture it can be seen how the motor moves with this second order acceleration profile. The postion is depicted as orange, velocity as red and the acceleration as green.	
*
*	\par Copyright
*
*	(C)2019 uStepper ApS	
*																	
*	www.ustepper.com 																	
*
*	administration@ustepper.com 														
*																							
*	<img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" />																
*
*	The code contained in this file is released under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>	
*																							
*	The code in this library is provided without warranty of any kind - use at own risk!		
* 	neither uStepper ApS nor the author, can be held responsible for any damage		
* 	caused by the use of the code contained in this library ! 	
*
*	\par To do list
*	- Clean out in unused variables
*	- Update comments
*
*	\par Known Bugs
*	- No known bugs
*
*	\author Thomas Hørring Olsen (thomas@ustepper.com)
*	\par Change Log
* 	\version 1.0.0:
*	- Fixed dropin and PID
*	- Complete rewrite of step generation algorithm, to increase performance and accuracy
*	- Added CLI interface to adjust dropin parameters and store to EEPROM
*	- Added docs
*	- ALOT of bugfixes 
* 	\version 0.1.1:
*  	- Updated uStepperServo example
*  	- Removed timer1 tampering from uStepperServo.cpp
*	\version 0.1.0:	
*	- Initial release
*	
*/


#ifndef _USTEPPER_S_LITE_H_
#define _USTEPPER_S_LITE_H_

#ifndef __AVR_ATmega328PB__
#error !!This library only supports the ATmega328pb MCU!!
#endif

#include <EEPROM.h>

/**
 * @brief      Union to easily split a float into its binary representation
 * 
 */
typedef union
{
	float f;			/**< normal float value*/
	uint8_t bytes[4];	/**< binary representation, split into an array of 4 bytes*/
}floatBytes_t;

/**
 * @brief      	Struct to store dropin settings
 *
 *				This struct contains the current dropin settings, aswell as a checksum,
 *				which is used upon loading of settings from EEPROM, to determine if the 
 *				settings in the EEPROM are valid.								
 * 
 */
typedef struct 
{
	floatBytes_t P;				/**< Proportional gain of the dropin PID controller	*/
	floatBytes_t I;				/**< Integral gain of the dropin PID controller	*/			
	floatBytes_t D;				/**< Differential gain of the dropin PID controller	*/
	uint8_t invert;				/**< Inversion of the "direction" input in dropin mode. 0 = NOT invert, 1 = invert	*/
	uint8_t holdCurrent;		/**< Current to use when the motor is NOT rotating. 0-100 %	*/
	uint8_t runCurrent;			/**< Current to use when the motor is rotating. 0-100 %	*/
	uint8_t checksum;			/**< Checksum	*/
}dropinCliSettings_t;

/** @name I2C0 defines
 *  Defines necessary to use I2C0 
 */
///@{
#define SDA SDA0
#define SCL SCL0
#define TWSTO TWSTO0
#define TWDR TWDR0
#define TWSTA TWSTA0
#define TWINT TWINT0
#define TWEA TWEA0
#define TWEN TWEN0
#define TWCR TWCR0
#define TWIE TWIE0
#define TWSR TWSR0
#define TWPS0 TWPS00
#define TWPS1 TWPS01
#define TWBR TWBR0
///@}

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <uStepperServo.h>
#include "TMC2208.h"
#include "i2cMaster.h"

/** Step generator frequency set to 100 kHz*/
#define STEPGENERATORFREQUENCY 100000.0
/** Full step definition*/
#define FULL 1							
/** Half step definition*/
#define HALF 2							
/** Quarter step definition*/
#define QUARTER 4						
/** Eighth step definition*/
#define EIGHT 8							
/** Sixteenth step definition*/	
#define SIXTEEN 16						
/** Value defining normal mode*/	
#define NORMAL 	0						
/** Value defining PWM mode of corresponding IO pin. Used to switch pin D3 and D8 between pwm or normal IO mode*/
#define PWM 	1						
/** Value defining dropin mode for 3d printer/CNC controller boards*/
#define DROPIN 	1						
/** Value defining PID mode for normal library functions*/
#define PID 	2						
/** Value to put in state variable in order to indicate that the motor should not be running */
#define STOP 1							
/** Value to put in state variable in order to indicate that the motor should be accelerating */
#define ACCEL 2	
/** Value to put in state variable in order to indicate that the motor should be decelerating */						
#define CRUISE 4						
/** Value to put in state variable in order to indicate that the motor should be cruising at constant speed with no acceleration */
#define DECEL 8							
/** Value to put in state variable in order to indicate that the motor should be decelerating to full stop before changing direction */
#define INITDECEL 16					
/** Value to put in direction variable in order for the stepper to turn clockwise */
#define CW 0						
/** Value to put in direction variable in order for the stepper to turn counterclockwise */	
#define CCW 1						
/** Value to put in hold variable in order for the motor to block when it is not running */
#define HARD 1							
/** Value to put in hold variable in order for the motor to \b not block when it is not running */
#define SOFT 0							
/** Value to convert angle moved between samples to RPM. */
#define DELTAANGLETORPM ENCODERINTFREQ*(60.0/4095.0)
/** Value to convert angle moved between samples to steps per second. */
#define DELTAANGLETOSTEPSPERSECOND ENCODERINTFREQ*(3200.0/4095.0)
/** Value to put in hold variable in order for the motor to block when it is not running */
#define BRAKEON 1
/** Value to put in hold variable in order for the motor to \b not block when it is not running */
#define BRAKEOFF 0
/** Frequency at which the encoder is sampled, for keeping track of angle moved and current speed */
#define ENCODERINTFREQ 500.0	
/** Encoder Sample period, for keeping track of angle moved and current speed */	
#define ENCODERINTSAMPLETIME 1.0/ENCODERINTFREQ	
/** I2C address of the encoder chip */
#define ENCODERADDR 0x36				
/** Address of the register, in the encoder chip, containing the 8 least significant bits of the stepper shaft angle */
#define ANGLE 0x0E						
/** Address of the register, in the encoder chip, containing information about whether a magnet has been detected or not */
#define STATUS 0x0B						
/** Address of the register, in the encoder chip, containing information about the current gain value used in the encoder chip. This value should preferably be around 127 (Ideal case!) */
#define AGC 0x1A						
/** Address of the register, in the encoder chip, containing the 8 least significant bits of magnetic field strength measured by the encoder chip */
#define MAGNITUDE 0x1B	
/**	P term in the PI filter estimating the step rate of incomming pulsetrain in DROPIN mode*/
#define PULSEFILTERKP 60.0
/**	I term in the PI filter estimating the step rate of incomming pulsetrain in DROPIN mode*/
#define PULSEFILTERKI 500.0*ENCODERINTSAMPLETIME
/** Value defining return of speed in Steps Per Second */
#define SPS 0
/** Value defining return of speed in Revolutions Per Minute */
#define RPM 1

/**
 * @brief      Measures angle and speed of motor.
 *
 *             This interrupt routine is in charge of sampling the encoder and
 *             measure the current speed of the motor. 
 */
extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal,used));

/**
 * @brief      Handles accelerations.
 *
 *             This interrupt routine is in charge of acceleration and deceleration.
 */
extern "C" void TIMER3_COMPA_vect(void) __attribute__ ((signal,used,naked));

/**
 * @brief      Used by dropin feature to take in step pulses
 *
 *             This interrupt routine is used by the dropin feature to keep
 *             track of step and direction pulses from main controller
 */
extern "C" void INT0_vect(void) __attribute__ ((signal,used));

/**
 * @brief      Used by dropin feature to take in enable signal
 *
 *             This interrupt routine is used by the dropin feature to keep
 *             track of enable signal from main controller
 */
extern "C" void INT1_vect(void) __attribute__ ((signal,used));

/**
 * @brief      Prototype of class for the AS5600 encoder
 *
 *             This class enables the user of the library to access the AS5600
 *             encoder on the uStepper S-lite board. This class can be instantiated as
 *             a standalone object if all the features of the uStepper S-lite is not
 *             needed by the programmers specific application.
 */


class uStepperEncoder
{
public:
	/** Variable used to store the angle moved from the
	* reference position */
	volatile int32_t angleMoved;	
	
	/** Angle of the shaft at the reference position. */
	uint16_t encoderOffset;				

	/** Used to stored the previous measured angle for the speed
	 * measurement, and the calculation of angle moved from reference
	 * position */
	volatile uint16_t oldAngle;			

	/** This variable always contain the current rotor angle, relative
	* to a single revolution */
	volatile uint16_t angle;			

	/** Variable used to store the current rotational speed of
	* the motor shaft */
	volatile float curSpeed;			 	
	/**
	 * @brief      Constructor
	 *
	 *             This is the constructor of the uStepperEncoder class.
	 */
	uStepperEncoder(void);
	
	/**
	 * @brief      Measure the current shaft angle
	 *
	 *             This function reads the current angle of the motor shaft. The
	 *             resolution of the angle returned by this function is
	 *             0.087890625 degrees (12 bits) The Angle is read by means of
	 *             the I2C interface, using the I2C interface implemented in
	 *             this library.
	 *
	 * @return     Floating point representation of the current motor shaft
	 *             angle
	 */
	float getAngle(void);
	
	/**
	 * @brief      Measure the current speed of the motor
	 *
	 *             This function returns the current speed of the motor. The
	 *             speed is not calculated in this function, it is merely
	 *             returning a variable. The speed is calculated in the
	 *             interrupt routine associated with timer1.
	 * @param      unit  - Return unit for speed, set to either "SPS" Steps 
	 *			   Per Second (default) or "RPM" (without the quotes).
	 * @return     Current speed in revolutions per minute (RPM)
	 */
	float getSpeed(bool unit = SPS);
	
	/**
	 * @brief      Measure the strength of the magnet
	 *
	 *             This function returns the strength of the magnet.
	 *
	 * @return     Strength of magnet
	 */
	uint16_t getStrength(void);
	
	/**
	 * @brief      Read the current AGC value of the encoder chip
	 *
	 *             This function returns the current value of the AGC register
	 *             in the encoder chip (AS5600). This value ranges between 0 and
	 *             255, and should preferably be as close to 128 as possible.
	 *
	 * @return     current AGC value
	 */
	uint8_t getAgc(void);
	
	/**
	 * @brief      Detect if magnet is present and within range
	 *
	 *             This function detects whether the magnet is present, too
	 *             strong or too weak.
	 *
	 * @return     0 - Magnet detected and within limits
	 * @return     1 - Magnet too strong
	 * @return     2 - Magnet too weak
	 */
	uint8_t detectMagnet(void);
	
	/**
	 * @brief      Measure the angle moved from reference position
	 *
	 *             This function measures the angle moved from the shaft
	 *             reference position. When the uStepper S-lite is first powered on,
	 *             the reference position is reset to the current shaft
	 *             position, meaning that this function will return the angle
	 *             rotated with respect to the angle the motor initially had. It
	 *             should be noted that this function is absolute to an
	 *             arbitrary number of revolutions !
	 *
	 *             The reference position can be reset at any point in time, by
	 *             use of the setHome() function.
	 *
	 * @return     The angle moved.
	 */
	float getAngleMoved(void);
	
	/**
	 * @brief      Setup the encoder
	 *
	 *             This function initializes all the encoder features.
	 */
	void setup(void);
	
	/**
	 * @brief      Define new reference(home) position
	 *
	 *             This function redefines the reference position to the current
	 *             angle of the shaft
	 */
	void setHome(void);

private:

	friend void TIMER1_COMPA_vect(void) __attribute__ ((signal,used));
};

/**
 * @brief      Prototype of class for accessing all features of the uStepper S-lite in
 *             a single object.
 *
 *             This class enables the user of the library to access all features
 *             of the uStepper S-lite board, by use of a single object.
 */

class uStepperSLite
{
public:
	
	/**This variable contains an open-loop number of steps moved from
	 * the position the motor had when powered on (or reset). A negative
	 * value represents a rotation in the counter clock wise direction
	 * and a positive value corresponds to a rotation in the clock wise
	 * direction. */
	volatile int32_t stepsSinceReset;		//offset 0
	/**	Counter used by the stepgeneration algorithm to check how many
	*	interrupt ticks has passed since last step was issued
	*/
	volatile uint32_t cntSinceLastStep;		//offset 4
	/**	This variable holds the delay needed between each step pulse,
	*	in interrupt ticks.	*/
	volatile uint32_t stepDelay;			//offset 8
	/** This variable tells the algorithm the direction of rotation for
	 * the commanded move. */
	volatile uint8_t stepGeneratorDirection;//offset 12
	/**	This variable holds the number of steps to issue during the
	*	deceleration phase.
	*/
	volatile int32_t decelToStopThreshold;	//offset 13

	/** This variable tells the algorithm whether the motor should
	 * rotated continuous or only a limited number of steps. If set to
	 * 1, the motor will rotate continous. */
	bool continous;							//offset 17

	/** This variable contains the current PID error. */
	volatile uint8_t pidError = 0;			//offset 18

	/** This variable is used by the stepper algorithm to keep track of
	 * which part of the acceleration profile the motor is currently
	 * operating at. */
	volatile uint8_t state;	

	/** This variable is used to indicate which mode the uStepper S-lite is
	* running in (Normal, Drop-in or PID)*/
	uint8_t mode;

	/** This variable contains the number of steps commanded by
	* external controller, in case of dropin feature */
	volatile int32_t stepCnt;	

	/** This variable contains the direction commanded by the last issued move */					
	volatile uint8_t direction;		

	/** This variable contains the maximum velocity, the motor is
	 * allowed to reach at any given point. The user of the library can
	 * set this by use of the setMaxVelocity() function */
	float velocity;					

	/** This variable contains the maximum acceleration to be used. The
	 * can be set and read by the user of the library using the
	 * functions setMaxAcceleration() and getMaxAcceleration()
	 * respectively. Since this library uses a second order acceleration
	 * curve, the acceleration applied will always be either +/- this
	 * value (acceleration/deceleration)or zero (cruise). */
	float acceleration;				
	
	/** This variable contains the conversion coefficient from raw
	* encoder data to number of steps */					
	volatile float stepConversion;	
	
	/** This variable contains the proportional coefficient used by the
	* PID */
	float pTerm;	

	/** This variable contains the value for converting steps per second to RPM */			
	float stepsPerSecondToRPM;

	/** This variable contains the value for converting RPM to steps per second */	
	float RPMToStepsPerSecond;

	/** This variable contains the integral coefficient used by the PID */
	float iTerm;	

	/** This variable contains the differential coefficient used by the PID */
	float dTerm;								

	/** This variable contains the sensitivity of the stall function, and is set to a value between 0.0 and 1.0*/
	float stallSensitivity = 0.992;

	/** This variable converts an angle in degrees into a corresponding
	 * number of steps*/
	float angleToStep;	

	/** This variable converts a number of steps into a corresponding
	 * angle in degrees*/	
	float stepToAngle;

	/** This variable holds information on wether the motor is stalled or not.
	0 = OK, 1 = stalled */
	volatile bool stall;
	
	/** This variable is used to convert a desired velocity in RPM to the corresponding
	*	delay between step pulses in interrupt ticks*/
	volatile float RPMToStepDelay;
	
	/**	This Variable holds the number of steps to issue during the initial deceleration phase,
	*	which is issued if the motor velocity is higher than the desired velocity of a new move*/
	volatile int32_t decelToAccelThreshold;
	
	/**	This Variable holds the number of steps to issue during the acceleration phase*/
	volatile int32_t accelToCruiseThreshold;

	/**	This Variable holds the number of steps to issue during the cruise phase*/
	volatile int32_t cruiseToDecelThreshold;

	/** This variable holds the current PID algorithm speed*/	
	volatile float currentPidSpeed;

	/** This variable holds the current PID algorithm acceleration*/	
	volatile float currentPidAcceleration;

	/** This variable holds the steps done in the PID since last reset*/	
	volatile float pidStepsSinceReset;

	/** This variable holds the target position*/	
	volatile int32_t targetPosition;
	
	/** This variable holds the bool telling if the PID is enabled or not*/	
	volatile bool pidDisabled;

	/** This variable holds the bool telling if the motor should brake or not*/	
	bool brake;

	friend void TIMER1_COMPA_vect(void) __attribute__ ((signal,used));
	friend void TIMER3_COMPA_vect(void) __attribute__ ((signal,used,naked));
	friend void INT0_vect(void) __attribute__ ((signal,used));
	friend void uStepperEncoder::setHome(void);	


	/**
	 * @brief      Enables the stepper driver output stage.
	 *
	 *             This function enables the output stage of the stepper driver.
	 *             If no step pulses is applied to the stepper driver, either
	 *             manually or by means of the stepper algorithm of this
	 *             library, this will make the force the motor to brake in the
	 *             current position.
	 */
	void enableMotor(void);
	

	/**
	 * @brief      Disables the stepper driver output stage.
	 *
	 *             This function disables the stepper driver output stage. If
	 *             this function is used, the motor will not brake, and it will
	 *             be possible to turn the motor shaft by hand
	 */
	void disableMotor(void);

	/**
	 * @brief      This method handles the actual PID controller calculations,
	 *             if enabled.
	 */
	void pid(float error);

	/**
	 * @brief      This method handles the actual PID Drop-in controller calculations,
	 *             if enabled.
	 */
	void pidDropin(float error);

	/**
	 * @brief      This method handles the connector orientation check in order to
	 *			   automatically compensate the PID for reversed direction
	 */	
	void checkConnectorOrientation(uint8_t mode);

	/**
	 * @brief      This method returns the current PID error
	 * @return     PID error (float)
	 */	
	float getPidError(void);

	/** This variable contains the current PID errror.
	*
	*/	
	volatile float currentPidError;

	/** This variable contains the current PID target position.
	*
	*/				
	volatile float pidTargetPosition;

	/** Instantiate object for the encoder */
	uStepperEncoder encoder;		

	/** Instantiate object for the Stepper Driver */
	Tmc2208 driver;

	/** This variable is used to invert the drop-in motor direction.
	*
	*/	
	bool invertPidDropinDirection;

	/**
	 * @brief      Constructor of uStepper S-lite class
	 *
	 *             This is the constructor of the uStepper S-lite class. This version
	 *             of the constructor takes in two arguments, "accel" and "vel".
	 *             These two arguments lets the programmer set the maximum
	 *             acceleration and velocity, respectively, during instantiation
	 *             of the uStepper S-lite object.
	 *
	 * @param      accel  - Floating point representation of the maximum
	 *                    acceleration allowed in steps/s^2.
	 * @param      vel    - Floating point representation of the maximum
	 *                    velocity allowed in steps/s.
	 */
	uStepperSLite(float accel = 1000.0, float vel = 1000.0);
	

	/**
	 * @brief      Set the maximum acceleration of the stepper motor.
	 *
	 *             This function lets the user of this library set the
	 *             acceleration used by the stepper algorithm. The algorithm is
	 *             a second order acceleration profile, meaning that the
	 *             acceleration only assumes three values; -a, 0 and a, with a
	 *             being the acceleration set by this function.
	 *
	 * @param      accel  - Maximum acceleration in steps/s^2
	 */
	void setMaxAcceleration(float accel);

	/**
	 * @brief      Sets the maximum rotational velocity of the motor
	 *
	 *             This function sets the maximum velocity at which the motor is
	 *             allowed to run. The rotational velocity of the motor will
	 *             gradually be ramped up to the value set by this function, by
	 *             the stepper acceleration profile implemented in this library.
	 *
	 * @param      vel   - Maximum rotational velocity of the motor in steps/s
	 */
	void setMaxVelocity(float vel);

	/**
	 * @brief      Make the motor rotate continuously
	 *
	 *             This function makes the motor rotate continuously, using the
	 *             acceleration profile implemented in this library. The motor
	 *             will accelerate at the rate set by setMaximumAcceleration(),
	 *             and eventually reach the speed set by setMaximumVelocity()
	 *             function. The direction of rotation is set by the argument
	 *             "dir".
	 *
	 * @param      dir   - Can be set to "CCW" or "CW" (without the quotes)
	 */
	void runContinous(bool dir);
	

	/**
	 * @brief      Make the motor perform a predefined number of steps
	 *
	 *             This function makes the motor perform a predefined number of
	 *             steps, using the acceleration profile implemented in this
	 *             library. The motor will accelerate at the rate set by
	 *             setMaximumAcceleration(), and eventually reach the speed set
	 *             by setMaximumVelocity() function. The direction of rotation
	 *             is set by the argument "dir". The argument "holdMode",
	 *             defines whether the motor should brake or let the motor
	 *             freewheel after the steps has been performed.
	 *
	 * @param      steps     -	Number of steps to be performed.
	 * @param      dir       -	Can be set to "CCW" or "CW" (without the quotes).
	 * @param      holdMode  -	can be set to "HARD" for brake mode or "SOFT" for
	 *                       freewheel mode (without the quotes).
	 */
	void moveSteps(int32_t steps, bool dir, bool holdMode = BRAKEON);
	

	/**
	 * @brief      Stop the motor without deceleration !!! DEPRECATED !!!
	 *
	 *             This function will stop any ongoing motor movement, without
	 *             any deceleration phase. If the motor is rotation at a
	 *             significant speed, the motor might not stop instantaneously,
	 *             due to build up inertia. The argument "holdMode" can be used
	 *             to define whether the motor should brake or freewheel after
	 *             the function has been called.
	 *
	 * @param      holdMode  -	can be set to "HARD" for brake mode or "SOFT" for
	 *                       freewheel mode (without the quotes).
	 */
	void hardStop(bool holdMode) __attribute__ ((deprecated("Function only here for compatibility with old code. this function is replaced by 'stop(bool brake)'")));
	

	/**
	 * @brief      Stop the motor with deceleration !!! DEPRECATED !!!
	 *
	 *             This function stops any ongoing motor movement, with a
	 *             deceleration phase. This will take longer for the motor to
	 *             stop, however the mechanical vibrations related to the
	 *             stopping of the motor can be significantly reduced compared
	 *             to the hardStop() function. The argument "holdMode" can be
	 *             used to define whether the motor should brake or freewheel
	 *             after the function has been called.
	 *
	 * @param      holdMode  -	can be set to "HARD" for brake mode or "SOFT" for
	 *                       freewheel mode (without the quotes).
	 */
	void softStop(bool holdMode) __attribute__ ((deprecated("Function only here for compatibility with old code. this function is replaced by 'stop(bool brake)'")));
	

	/**
	 * @brief      Stop the motor with deceleration
	 *
	 *             This function stops any ongoing motor movement, with a
	 *             deceleration phase. This will take longer for the motor to
	 *             stop, however the mechanical vibrations related to the
	 *             stopping of the motor can be significantly reduced compared
	 *             to the hardStop() function. The argument "holdMode" can be
	 *             used to define whether the motor should brake or freewheel
	 *             after the function has been called.
	 *
	 * @param      brake  -	can be set to "HARD" for brake mode or "SOFT" for
	 *                       freewheel mode (without the quotes).
	 */
	void stop(bool brake = BRAKEON);

	/**
	 * @brief      Initializes the different parts of the uStepper S-lite object
	 *
	 *             This function initializes the different parts of the uStepper S-lite
	 *             object, and should be called in the setup() function of the
	 *             arduino sketch. This function is needed as some things, like
	 *             the timer can not be setup in the constructor, since arduino
	 *             for some strange reason, resets a lot of the AVR registers
	 *             just before entering the setup() function.
	 *
	 * @param[in]  mode             	Default is normal mode. Pass the constant
	 *                              	"DROPIN" to configure the uStepper S-lite to act as
	 *                              	dropin compatible to the stepstick. Pass the
	 *                              	constant "PID", to enable PID feature for
	 *                              	regular movement functions, such as
	 *                              	moveSteps()
	 * @param[in]  stepsPerRevolution  	This parameter should be set to
	 *                              	the steps/revolution which is 16 * steppper steps/revolution.
	 *									For a 1.8deg stepper this is 16 * 200 = 3200.
	 *									In Drop-in this number should be set to the microstep setting
	 *									of the controller (e.g. 16).
	 * @param[in]  pTerm           		The proportional coefficent of the PID
	 *                              	controller
	 * @param[in]  iTerm            	The integral coefficent of the PID
	 *                              	controller
	 * @param[in]  dTerm            	The differential coefficent of the PID
	 *                              	controller
	 * @param[in]  setHome          	When set to true, the encoder position is
	 *									Reset. When set to false, the encoder
	 *									position is not reset.
	 * @param[in]  invert           	Inverts the motor direction for dropin
	 *									feature. 0 = NOT invert, 1 = invert.
	 *									this has no effect for other modes than dropin
	 * @param[in]  runCurrent       	Sets the current (in percent) to use while motor is running.
	 * @param[in]  holdCurrent      	Sets the current (in percent) to use while motor is NOT running
	 */
	void setup(	uint8_t mode = NORMAL, 
				float stepsPerRevolution = 3200.0, 
				float pTerm = 0.75, 
				float iTerm = 3.0, 
				float dTerm = 0.0,
				bool setHome = true,
				uint8_t invert = 0,
				uint8_t runCurrent = 50,
				uint8_t holdCurrent = 30);	

	/**
	 * @brief      Returns the direction the motor is currently configured to
	 *             rotate
	 *
	 *             This function checks the last configured direction of
	 *             rotation and returns this.
	 *
	 * @return     0 - Clockwise
	 * @return     1 - Counter Clockwise
	 */
	bool getCurrentDirection(void);
	

	/**
	 * @brief      Get the current state of the motor
	 *
	 *             This function returns the current state of the motor, i.e. if
	 *             the motor is rotating or not.
	 *
	 * @return     0 - Motor not rotating
	 * @return     1 - Motor rotating
	 */
	uint8_t getMotorState(void);
	

	/**
	 * @brief      Get the number of steps applied since reset.
	 *
	 *             This function returns the number of steps applied to the
	 *             motor driver since reset. This function doesn't care if the
	 *             step has actually been performed or not. Steps applied in the
	 *             clockwise direction is added and steps applied in the counter
	 *             clockwise direction is subtracted. This means that a negative
	 *             return value indicates that the motor should have rotated the
	 *             returned amount of steps in the counter clockwise direction,
	 *             with respect to the initial position. Likewise a positive
	 *             value indicate the number of steps the motor should have
	 *             rotated in the clockwise direction, with respect to the
	 *             initial position.
	 *
	 * @return     negative value - number of steps the motor should have
	 *             rotated in the counter clockwise direction, with respect to
	 *             the initial position.
	 * @return     positive value - number of steps the motor should have rotated in
	 *             the clockwise direction, with respect to the initial position.
	 */
	int32_t getStepsSinceReset(void);

	/**
	 * @brief      Set motor run and hold current
	 *
	 *             This function allows the user to change the run and hold current setting of the motor
	 *             driver.
	 *
	 * @param[in]  runCurrent Desired run current setting in percent (0% - 100%)
	 *
	 * @param[in]  holdCurrent Desired brake current setting in percent (0% - 100%)
	 */
	void setCurrent(uint8_t runCurrent, uint8_t holdCurrent = 25);

	/**
	 * @brief      Set motor hold current
	 *
	 *             This function allows the user to change the hold current setting of the motor
	 *             driver.
	 *
	 * @param[in]  holdCurrent Desired brake current setting in percent (0% - 100%)
	 */
	void setHoldCurrent(uint8_t holdCurrent);

	/**
	 * @brief      Set motor run current
	 *
	 *             This function allows the user to change the run current setting of the motor
	 *             driver.
	 *
	 * @param[in]  runCurrent Desired run current setting in percent (0% - 100%)
	 */
	void setRunCurrent(uint8_t runCurrent);
	
	/**
	 * @brief      	Moves the motor to its physical limit, without limit switch
	 *
	 *              This function, makes the motor run continously, untill the
	 *				encoder detects a stall, at which point the motor is assumed
	 *				to be at it's limit.
	 *
	 * @param[in]  	dir  Direction to search for limit
	 *
	 * @param[in]  	stallSensitivity  Sensitivity of stall detection (0.0 - 1.0), low is more sensitive
	 *
	 * @return 		Degrees turned from calling the function, till end was reached
	 */
	float moveToEnd(bool dir, float stallSensitivity = 0.992);

	/**
	 * @brief      	Moves the motor to an absolute angle
	 *
	 * @param[in]  	angle  Absolute angle. A positive angle makes
	 *				the motor turn clockwise, and a negative angle, counterclockwise.
	 *
	 * @param[in]  	holdMode can be set to "HARD" for brake mode or "SOFT" for
	 *              freewheel mode (without the quotes).
	 */

	void moveToAngle(float angle, bool holdMode = BRAKEON);

	/**
	 * @brief      	Moves the motor to a relative angle
	 *
	 * @param[in]  	angle  Relative angle from current position. A positive angle makes
	 *				the motor turn clockwise, and a negative angle, counterclockwise.
	 *
	 * @param[in]  	holdMode can be set to "HARD" for brake mode or "SOFT" for
	 *              freewheel mode (without the quotes).
	 */
	void moveAngle(float angle, bool holdMode = BRAKEON);

	/**
	 * @brief      	This method returns a bool variable indicating wether the motor
	 *				is stalled or not
	 *
	 * @param[in]  	stallSensitivity - Sensitivity of stall detection (0.0 - 1.0), low is more sensitive
	 *
	 * @return     	0 = not stalled, 1 = stalled
	 */
	bool isStalled(float stallSensitivity = 0.992);

	/**
	 * @brief      	This method disables the PID until calling enablePid.
	 *
	 */
	void disablePid(void);

	/**
	 * @brief      	This method enables the PID after being disabled  (disablePid).
	 *
	 */
	void enablePid(void);

	/**
	 * @brief      	This method is used to change the PID proportional parameter P.
	 *
	 * @param[in]  	P - PID proportional part P
	 *
	 */
	void setProportional(float P);

	/**
	 * @brief      	This method is used to change the PID integral parameter I.
	 *
	 * @param[in]  	I - PID integral part I
	 *
	 */
	void setIntegral(float I);

	/**
	 * @brief      	This method is used to change the PID differential parameter D.
	 *
	 * @param[in]  	D - PID differential part D
	 *
	 */
	void setDifferential(float D);

	/**
	 * @brief      	This method is used to invert the drop-in direction pin interpretation.
	 *
	 * @param[in]  	invert - 0 = not inverted, 1 = inverted
	 *
	 */
	void invertDropinDir(bool invert);

	/**
	 * @brief      	This method is used to tune Drop-in parameters.
	 *				After tuning uStepper S-lite the parameters are saved in EEPROM
	 *				
	 * 				Usage:
	 *				Set Proportional constant: 'P=10.002;'
	 *				Set Integral constant: 'I=10.002;'
	 *				Set Differential constant: 'D=10.002;'
	 *				Invert Direction: 'invert;'
	 *				Get Current PID Error: 'error;'
	 *				Get Run/Hold Current Settings: 'current;'
	 *				Set Run Current (percent): 'runCurrent=50.0;'
	 *				Set Hold Current (percent): 'holdCurrent=50.0;'	
	 *
	 */	
	void dropinCli();

	/**
	 * @brief      	This method is used for the dropinCli to take in user commands.
	 *
	 * @param[in]  	cmd - input from terminal for dropinCli
	 *			
	 */
	void parseCommand(String *cmd);

	/**
	 * @brief      	This method is used to print the dropinCli menu explainer:
	 *				
	 * 				Usage:
	 *				Show this command list: 'help;'
	 *				Get PID Parameters: 'parameters;'
	 *				Set Proportional constant: 'P=10.002;'
	 *				Set Integral constant: 'I=10.002;'
	 *				Set Differential constant: 'D=10.002;'
	 *				Invert Direction: 'invert;'
	 *				Get Current PID Error: 'error;'
	 *				Get Run/Hold Current Settings: 'current;'
	 *				Set Run Current (percent): 'runCurrent=50.0;'
	 *				Set Hold Current (percent): 'holdCurrent=50.0;'	
	 *
	 */	
	void dropinPrintHelp();

private:

	/**
	 * @brief      	This method is used internally for stall detection.
	 *
	 * @return     	0 = not stalled, 1 = stalled
	 *			
	 */
	bool detectStall(void);

	/**	This variable holds the dropin settings.
	*	@see dropinCliSettings_t*/
	dropinCliSettings_t dropinSettings;

	/**
	 * @brief      	This method loads the dropin settings stored in EEPROM. 
	 *
	 *				This method loads the dropin settings stored in EEPROM. If the data stored in EEPROM
	 *				is valid, the settings are applied. If the data is not valid, the data loaded from
	 *				EEPROM is discarded, and the default data specifed at compile time are used isntead.
	 *
	 * @return     	0 = stored settings NOT valid, 1 = stored settings valid
	 *			
	 */
	bool loadDropinSettings(void);

	/**
	 * @brief      	This method stores the current dropin settings in EEPROM
	 *			
	 */
	void saveDropinSettings(void);

	/**
	 * @brief      	This method is used internally for stall detection.
	 *
	 * @param[in]	settings - address of the dropin settings object, of which to calculate the checksum
	 *
	 * @return     	checksum of dropin settings struct
	 *			
	 */
	uint8_t dropinSettingsCalcChecksum(dropinCliSettings_t *settings);
};

/** Global definition of I2C object for use in Arduino sketch */
extern i2cMaster I2C;		

#endif