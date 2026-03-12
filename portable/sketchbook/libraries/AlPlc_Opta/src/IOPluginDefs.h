
#pragma once

// Also defined in AlPlcMisrac.h ; howevere required here to let include this .h directly in the sketch
typedef uint8_t bool_t;

#if defined( ARDUINO_PORTENTA_H7_M7 )

	#define NUM_DIGITAL_INPUTS		8
	#define NUM_DIGITAL_OUTPUTS		8
	#define NUM_ANALOG_INPUTS		3		// DINT
	#define NUM_ANALOG_OUTPUTS		4		// REAL
	#define NUM_PROG_DIGITAL_IO     12
	#define NUM_SYS_TEMP_PROBES     3		// REAL
	#define NUM_SYS_ENCODERS        2		// DINT*3

	#define IO_TOT_DIGITAL_OUTPUTS		(NUM_DIGITAL_OUTPUTS + NUM_PROG_DIGITAL_IO)
	#define IO_TOT_DIGITAL_INPUTS		NUM_DIGITAL_INPUTS
	#define IO_TOT_ANALOG_OUTPUTS		NUM_ANALOG_OUTPUTS
	#define IO_TOT_ANALOG_INPUTS		(NUM_ANALOG_INPUTS + NUM_SYS_TEMP_PROBES)

	typedef struct _sysEncoderInputType
	{
		int currentState;
		int pulsesCount;
		int revolutionsCount;
	} sysEncoderInputType;

#elif defined( ARDUINO_OPTA )

	// from Arduino_Opta_Blueprint\src
    // instead of including OptaBlue.h, which is the root that would pull in C++, include this here just for I/O number
	#include <DigitalCommonCfg.h>
	#include <AnalogCommonCfg.h>

	// Definitions similar to that of DigitalCommonCfg.h
	#define OPTA_ANALOG_IN_NUM		OA_AN_CHANNELS_NUM			// pragrammable, max 8 channels
	#define OPTA_ANALOG_OUT_NUM		OA_AN_CHANNELS_NUM			// pragrammable, max 8 channels
	#define OPTA_PWM_OUT_NUM		OA_PWM_CHANNELS_NUM			// 4 pwm
	#define OPTA_LED_OUT_NUM		OA_LED_NUM

	typedef struct
	{
		uint32_t period;
		uint32_t pulse;
	} PWM_t;
	
	typedef union
	{
		float rtd;
		uint16_t an;
		bool_t dig;
		int32_t raw;
	} ProgChannel_t;

	#define NUM_PROG_INPUTS			8
	#define NUM_RELAY_OUTPUTS		4
	#define NUM_LED_OUTPUTS			7
	#define NUM_BUTTON_INPUTS		1

	// At the moment lower than that actually supported by the library (OPTA_CONTROLLER_MAX_EXPANSION_NUM=10),
	// agreed with Arduino to deal with maximum I2C speed.
	#define MAX_OPTA_IOEXP 5
	typedef uint16_t ExpType_t;

	#define IO_TOT_ANEXP_OUT			(OPTA_ANALOG_OUT_NUM + OPTA_PWM_OUT_NUM * 2)   // 16 out per exp

	#define IO_TOT_DIGITAL_INPUTS		(NUM_PROG_INPUTS + NUM_BUTTON_INPUTS + MAX_OPTA_IOEXP*OPTA_DIGITAL_IN_NUM)
	#define IO_TOT_DIGITAL_OUTPUTS		(NUM_RELAY_OUTPUTS + NUM_LED_OUTPUTS + MAX_OPTA_IOEXP*OPTA_DIGITAL_OUT_NUM)
	// since on digital exp the prog inputs can also be used as analog and are more than the inputs of the analog exp, use those
	#define IO_TOT_ANALOG_INPUTS		(NUM_PROG_INPUTS + MAX_OPTA_IOEXP*OPTA_DIGITAL_IN_NUM)
	#define IO_TOT_ANALOG_OUTPUTS		(MAX_OPTA_IOEXP*IO_TOT_ANEXP_OUT)

#endif

//  GetDigitalInput
#define IOPLUGIN_GETDIGITALINPUT_FN		"IOPlugin_GetDigitalInput"
typedef bool_t (* IOPlugin_GetDigitalInput_func)( uint8_t id );

//  GetDigitalOutput
#define IOPLUGIN_GETDIGITALOUTPUT_FN	"IOPlugin_GetDigitalOutput"
typedef bool_t (* IOPlugin_GetDigitalOutput_func)( uint8_t id );

//  SetDigitalOutput
#define IOPLUGIN_SETDIGITALOUTPUT_FN	"IOPlugin_SetDigitalOutput"
typedef bool_t (* IOPlugin_SetDigitalOutput_func)( uint8_t id, bool_t value );

//  GetAnalogInput
#define IOPLUGIN_GETANALOGINPUT_FN		"IOPlugin_GetAnalogInput"
typedef int32_t (* IOPlugin_GetAnalogInput_func)( uint8_t id );

//  GetAnalogOutput
#define IOPLUGIN_GETANALOGOUTPUT_FN		"IOPlugin_GetAnalogOutput"
typedef int32_t (* IOPlugin_GetAnalogOutput_func)( uint8_t id );

//  SetAnalogOutput
#define IOPLUGIN_SETANALOGOUTPUT_FN		"IOPlugin_SetAnalogOutput"
typedef bool_t (* IOPlugin_SetAnalogOutput_func)( uint8_t id, int32_t value );
