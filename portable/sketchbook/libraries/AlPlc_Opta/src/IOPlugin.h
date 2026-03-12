#ifndef IOPLUGIN_H
#define IOPLUGIN_H

/* local I/O */

#if defined( ARDUINO_PORTENTA_H7_M7 )

#include "ArduinoSpec/IODriver.h"

extern bool_t sysDigitalInputs[NUM_DIGITAL_INPUTS];
extern bool_t sysDigitalOutputs[NUM_DIGITAL_OUTPUTS];

extern int32_t sysAnalogInputs[NUM_ANALOG_INPUTS];
extern float sysAnalogOutputs[NUM_ANALOG_OUTPUTS];
extern bool_t sysProgrammableDigitalIO[NUM_PROG_DIGITAL_IO];

extern SYS_TEMPPROBE_TYPE sysTempProbes[NUM_SYS_TEMP_PROBES];

extern sysEncoderInputType sysEncoders[NUM_SYS_ENCODERS];

#elif defined( ARDUINO_OPTA )

#include "IOPluginDefs.h"

extern uint16_t sysProgrammableInputs[NUM_PROG_INPUTS];
extern bool_t sysRelayOutputs[NUM_RELAY_OUTPUTS];
extern bool_t sysLEDOutputs[NUM_LED_OUTPUTS];
extern bool_t sysButtonInputs[NUM_BUTTON_INPUTS];

typedef union
{
	struct
	{
		uint16_t programmableInputs[OPTA_DIGITAL_IN_NUM];  // bool_t / uint16_t
	} digital; // 32 bytes
	
	struct
	{
		ProgChannel_t analogInputs[OPTA_ANALOG_IN_NUM];  // float / uint16_t / bool_t
	} analog;  // 32 bytes
} ExpInput_t;

typedef union
{
	struct
	{
		bool_t relayOutputs[OPTA_DIGITAL_OUT_NUM];
	} digital; // 8 bytes

	struct
	{
		ProgChannel_t analogOutputs[OPTA_ANALOG_OUT_NUM]; // float / uint16_t
		PWM_t pwmOutputs[OPTA_PWM_OUT_NUM];
		bool_t ledOutputs[OPTA_LED_OUT_NUM];
	} analog; // 72 bytes
} ExpOutput_t;

extern ExpInput_t  sysExpInputs [MAX_OPTA_IOEXP];
extern ExpOutput_t sysExpOutputs[MAX_OPTA_IOEXP];
#endif

bool_t IOPlugin_GetDigitalInput(uint8_t id);
bool_t IOPlugin_GetDigitalOutput(uint8_t id);
bool_t IOPlugin_SetDigitalOutput(uint8_t id, bool_t value);
int32_t IOPlugin_GetAnalogInput(uint8_t id);
int32_t IOPlugin_GetAnalogOutput(uint8_t id);
bool_t IOPlugin_SetAnalogOutput(uint8_t id, int32_t value);

#endif 	//	IOPLUGIN_H
