# Blueprint Protocol

This file describes the Blueprint protocol used on I2C bus.
In this file we won't use any actual value, but the defines used through the
code. The actual value can be found the Protocol.h file.

## HEADER

Each message has an header with 3 bytes:

1. first byte is the COMMAND (what to do)
2. second byte is the ARGUMENT (the target of the COMMAND)
3. third byte is the LENGTH (this specify how many bytes follows the HEADER i.e.
   the payload length). Please note:
   - if CRC is used the LENGTH does not include the last CRC8 byte
   - LENGTH can be 0 if not payload is expected

## PAYLOAD

It follows the HEADER and has to be LENGTH long, it specifies the value of the
argument.

## CRC

By default the protocol adds a CRC8 at the end of each message immediately after
the payload, the use of the CRC can be disable removing the definition of BP_USE_CRC.
When used the message is discarded and not used if the CRC is wrong.
This CRC is based on an 8-bit CRC using the following polynomial:
C(x) = x^8 + x^2 + x^1 + 1
All bytes of the message from COMMAND to the last bytes of payload are included
in the CRC calculation.

## Commands

The first byte of each message is the COMMAND code.

### Controller commands

These are the commands (first byte of the message) sent by the Controller and
received by the Expansions:

- set (used to set something in the expansion), value BP_CMD_SET
- get (used to get something in the expansion), value BP_CMD_GET

### Expansions commands (answers)

These are the commands (first byte of the message) sent by the Expansions and
received by the Controller:

- set answer (used to set something in the expansion), value BP_ANS_SET
- get answer (used to get something in the expansion), value BP_ANS_GET

## Arguments

The second byte of the message is the ARGUMENT: this byte is intended to specify
which is the target or the action performed by the COMMAND.

### Example of arguments

- ARG_CONTROLLER_RESET (value 0x01) specify that the SET COMMAND is aimed to
  "reset" the Expansion to is default I2C address.

- ARG_ADDRESS (value 0x02) specify that the SET COMMAND is aimed to set the I2C
  address of the EXPANSION.

## Payloads

The payload specify the value of the argument: for example ARG_ADDRESS in SET
COMMAND is aimed to set the address of the I2C expansion, therefore the PAYLOAD
must be the value of the address. In this case the LENGTH field must be set to 1
specifying that the PAYLOAD length is 1 byte long.
Different messages have different payloads, with different lengths.

## Unandled messages

If an expansion receives a messages which is not supposed to handle
(and this message is expecting an answer from the expansion) the first
2 bytes of the answer will be 0xFA, 0xFE: this sequence signals that the expansion
received the request but this request is not supported by this expansion.


## Messages

The following describes each message and the related answer

### RESET EXPANSION I2C ADDRESS (+)

Apply to: All expansion types

When an expansion receive this message it resets its own I2C address to the
default one (0x0A) and de-initialize I2C bus (the I2C bus is automatically
initialized if the DETECT IN, DETECT OUT pins are in the correct state)

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_CONTROLLER_RESET (0x01)
    LEN_CONTROLLER_RESET (0x01)
  - Payload:
    CONTROLLER_RESET_CODE (0x56)
  - CRC
- Expansion answer
  No answer expected

### SET I2C ADDRESS (+)

Apply to: All expansion types

When an expansion receive this message it sets its own I2C address to the
one specified in the payload.

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_ADDRESS (0x02)
    LEN_ADDRESS (0x01)
  - Payload:
    -> the new I2C address to be used (1 byte)
  - CRC
- Expansion answer
  No answer expected

### GET I2C ADDRESS AND TYPE (+)

Apply to: All expansion types

When an expansion receive this message it answers with its own address and type.

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_ADDRESS_AND_TYPE (0x03)
    LEN_ADDRESS_AND_TYPE (0x00)
  - Payload: None
  - CRC
- Expansion answer
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_ADDRESS_AND_TYPE (0x03)
    ANS_LEN_ADDRESS_AND_TYPE (0x02)
  - Payload:
    -> the address (1 byte)
    -> the type of expansion (1 byte)
  - CRC

Type of expansion values:
Opta Digital Invalid -> 0x01
Opta Digital Mechanical -> 0x02
Opta Digital State Solid -> 0x03
Opta Analog -> 0x04
Custom expansion get a dynamic expansion type See AboutCustomExpansionType.md

### GET DIGITAL VALUES (+)

Apply to: Opta Digital

When an expansion receive this message it answers with the status of digital
inputs.

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OD_GET_DIGITAL_INPUTS (0x04)
    LEN_OD_GET_DIGITAL_INPUTS (0x00)
  - Payload: None
  - CRC
- Expansion answer
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_ADDRESS_AND_TYPE (0x04)
    ANS_LEN_OD_GET_DIGITAL_INPUTS (0x02)
  - Payload:
    -> the status of digital inputs (2 byte)
  - CRC

Note: the status of digital inputs is a bit mask word (2 bytes): each bit of
the word represent the status of digital input (1 means HIGH, 0 means LOW).

### GET ANALOG VALUES (+)

Apply to: Opta Digital

When an expansion receive this message it answers with the value of the analog
pin received as argument (the analog value is the raw analog value in bits).

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OD_GET_ANALOG_INPUT (0x05)
    LEN_OD_GET_ANALOG_INPUT (0x01)
  - Payload:
    -> the number of the PIN (1 byte)
  - CRC
- Expansion answer
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OD_GET_ANALOG_INPUT (0x05)
    ANS_LEN_OD_GET_ANALOG_INPUT (0x02)
  - Payload:
    -> the analog value of the PIN (2 byte) - LSB first
  - CRC

Note: the analog value is a WORD (2 bytes) - first is sent the LSB then the MSB.

### GET ANALOG ALL VALUES (+)

Apply to: Opta Digital

When an expansion receive this message it answers with the analog values of all
its PINs.

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OD_GET_ALL_ANALOG_INPUTS (0x07)
    LEN_OD_GET_ALL_ANALOG_INPUTS (0x00)
  - Payload: None
  - CRC
- Expansion answer
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OD_GET_ALL_ANALOG_INPUTS (0x07)
    ANS_LEN_OD_GET_ALL_ANALOG_INPUTS (32 bytes)
  - Payload:
    -> the analog value of all the 16 PINs (each pin is 2 bytes) - LSB first
  - CRC

Note: the payload is array of 16 words (LSB first) each value represent the
analog value of the corresponding PIN.

### SET DIGITAL OUTPUTS (+)

Apply to: Opta Digital

When an expansion receive this message it sets its own I2C address to the
one (0x0A) specified in the payload.

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OD_SET_DIGITAL_OUTPUTS (0x06)
    LEN_OD_SET_DIGITAL_OUTPUTS (0x01)
  - Payload:
    -> the status of all digital outputs (1 byte)
  - CRC
- Expansion answer
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OD_SET_DIGITAL_OUTPUTS (0x06)
    ANS_LEN_OD_SET_DIGITAL_OUTPUTS (0x00)
  - Payload: None
  - CRC

Note: the status of all digital output is a bit-mask: each bit correspond to the
status the PIN will be set upon to by the expansion at the reception of the SET
message (1 --> HIGH, 0 --> LOW)
Note (2): the Expansion answer is requested in order to ensure that the message
is properly received (CRC is OK), if the message answer is not received the
Controller will trigger the related callback function (if set).

### SET DEFAULT OUTPUT VALUES AND TIMEOUT (+)

Apply to: Opta Digital

Each Digital expansion monitors the I2C and expects to receive an I2C message within
a certain timeout, if the timeout expires the Expansion reset its output to the
a default value. This is a safety policy meant to avoid the fact that outputs
sticks to a value in case the controller for some reasons is not able to
communicate with the expansion.
This message sets the default value for the Opta Digital expansion and the
related timeout.
A timeout equal to 0xFFFF means no timeout.

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OD_DEFAULT_AND_TIMEOUT (0x08)
    LEN_OD_DEFAULT_AND_TIMEOUT (0x03)
  - Payload:
    -> the status of all digital outputs (1 byte)
    -> the timeout (2 bytes) - LSB first
  - CRC
- Expansion answer
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OD_DEFAULT_AND_TIMEOUT (0x08)
    ANS_LEN_OD_DEFAULT_AND_TIMEOUT (0x00)
  - Payload: None
  - CRC

Note: the status of all digital output is a bit-mask: each bit correspond to the
status the PIN will be set upon to by the expansion at the reception of the SET
message (1 --> HIGH, 0 --> LOW)
Note (2): the timeout is a word (2 bytes) LSB sent first
Note (3): the Expansion answer is requested in order to ensure that the message
is properly received (CRC is OK), if the message answer is not received the
Controller will trigger the related callback function (if set).

### CONFIGURE OPTA ANALOG CHANNEL AS ADC (+)

Apply to: Opta Analog

Configure an Opta Analog channel as ADC

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_CH_ADC (0x09)
    LEN_OA_CH_ADC (0x07)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> ADC type (1 byte) (0 voltage adc, 1 current adc)
    -> use pulldown (1 Enable, 2 Disable)
    -> use rejection (1 Enable, 2 Disable)
    -> use diagnostic (1 Enable, 2 Disable)
    -> number of point of the moving average (max 255)
    -> add ADC to another function (1 Enable, 2 Disable)
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

  ### GET OPTA ANALOG ADC VALUE (adc bit 0-65535) (+)

  Apply to: Opta Analog

  Get the current value of ADC channel

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OA_CH_ADC (0x0A)
    LEN_OA_CH_ADC (0x01)
  - Payload:
    -> channel (1 byte) from 0 to 7
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OA_GET_ADC (0x0A)
    ANS_LEN_OA_GET_ADC (0x03)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> the analog value of the channe (2 byte) - LSB first
  - CRC

  ### GET OPTA ANALOG ALL ADC VALUE (adc bit 0-65535) AT ONCE (+)

  Apply to: Opta Analog

  Get the current value of ADC channel

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OA_GET_ALL_ADC (0x0B)
    LEN_OA_GET_ALL_ADC (0x00)
  - Payload: None
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OA_GET_ALL_ADC (0x0B)
    ANS_LEN_OA_GET_ALL_ADC (0x10)
  - Payload:
    -> 16 bytes (2 for each channel) - LSB first
  - CRC

### CONFIGURE OPTA ANALOG CHANNEL AS DAC (+)

Apply to: Opta Analog

Configure an Opta Analog channel as DAC

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_CH_DAC (0x0C)
    LEN_OA_CH_DAC (0x05)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> DAC type (1 byte) (0 voltage dac, 1 current dac)
    -> limit current (1 Enable, 2 Disable)
    -> use slew rate (1 Enable, 2 Disable)
    -> slew rate (0 - 16)
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### SET OPTA ANALOG DAC CHANNEL VALUE (0 - 8191) (+)

Apply to: Opta Analog

Set Opta Analog DAC channel value

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_DAC (0x0D)
    LEN_OA_SET_DAC (0x04)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> DAC value (2 bytes) (0 - 8191) - LSB first
    -> UPDATE DAC immediately (1 - immediately, 0 - no immediate update)
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### APPLY DAC VALUE TO PHYSICAL OUTPUTs (+)

Apply to: Opta Analog

If set dac message above is used with UPDATE DAC immediately equal to 0 the
DAC value is not output until this message is received (this can be used to 
update DAC synchronously)

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_ALL_DAC (0x22)
    LEN_OA_SET_ALL_DAC (0x00)
  - Payload: no payload
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC  

### CONFIGURE OPTA ANALOG CHANNEL AS RTD (+)

Apply to: Opta Analog

Configure an Opta Analog channel as RTD

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_CH_RTD (0x0E)
    LEN_OA_CH_RTD (0x06)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> use 3 wire (1 byte) (1 Enable, 2 Disable)
    -> current mA as float on 4 bytes
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### GET OPTA ANALOG RTD CHANNEL VALUE (Ohm) (+)

Apply to: Opta Analog

Get Opta Analog RTD channel value

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OA_GET_RTD (0x0F)
    LEN_OA_GET_RTD (0x01)
  - Payload:
    -> channel (1 byte) from 0 to 7
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OA_GET_RTD (0x0F)
    ANS_LEN_OA_GET_RTD (0x05)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> value 4 bytes as float
  - CRC

### CONFIGURE OPTA ANALOG RTD UPDATE TIME (+)

Apply to: Opta Analog

Configure the update time of RDT channel (this is used for each RTD channel)
Take into account that 3 wire RTD takes around 800 ms per channel.

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_RTD_UPDATE_TIME (0x10)
    LEN_OA_SET_RTD_UPDATE_TIME (0x02)
  - Payload:
    -> update time (ms) - 2 bytes - LSB first
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### CONFIGURE OPTA ANALOG CHANNEL AS DIGITAL INPUT (+)

Apply to: Opta Analog

Configure an Opta Analog channel as Digital Input

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_CH_DI (0x11)
    LEN_OA_CH_DI (0x09)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> filter comparator (1 Enable, 2 Disable)
    -> invert comparator output (1 Enable, 2 Disable)
    -> enable comparator (1 Enable, 2 Disable)
    -> use simple debounce (1 Enable, 2 Disable) - disable means Integrator
    debounce
    -> scale voltage comparator (1 Enable, 2 Disable)
    -> threshold voltage comparator (0 - 31)
    -> current sink (0 - 31)
    -> debounce time (0 - 31)
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### GET OPTA ANALOG DIGITAL INPUT STATUS (+)

Apply to: Opta Analog

Get Opta Analog DIGITAL INPUT status

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_OA_GET_DI (0x12)
    LEN_OA_GET_DI (0x00)
  - Payload: None
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_OA_GET_DI (0x12)
    ANS_LEN_OA_GET_DI (0x01)
  - Payload:
    -> all DI status (1 byte) as bit mask
  - CRC

### CONFIGURE OPTA ANALOG CHANNEL AS HIGH IMPEDANCE (+)

Apply to: Opta Analog

Configure an Opta Analog channel as High Impedance

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_CH_DI (0x24)
    LEN_OA_CH_DI (0x01)
  - Payload:
    -> channel (1 byte) from 0 to 7
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC


### SET OPTA ANALOG PWM (+)

Apply to: Opta Analog

Set Opta digital PWM channel values

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_PWM (0x13)
    LEN_OA_SET_PWM (0x09)
  - Payload:
    -> channel (1 byte) from 0 to 3
    -> pwm period in usec (4 bytes) - LSB first
    -> pwm pulse (high) in usec (4 bytes) - LSB first
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### SET OPTA ANALOG LED (+)

Apply to: Opta Analog

Set Opta digital PWM channel values

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_LED (0x15)
    LEN_OA_SET_LED (0x01)
  - Payload:
    -> LEDs status (1 byte) as bitmask
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### GET FW VERSION (+)

Apply to: All expansion types

Get FW version in the form MAJOR.MINOR.RELEASE

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_GET_VERSION (0x16)
    LEN_GET_VERSION (0x00)
  - Payload: None
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_GET (0x03)
    ANS_ARG_GET_VERSION (0x16)
    ANS_LEN_GET_VERSION (0x03)
  - Payload:
    -> major (1 byte)
    -> minor (1 byte)
    -> release (1 byte)
  - CRC

### REBOOT expansion (+)

Apply to: All expansion types

Put expansion in Boot Loader Mode

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_REBOOT (0xF3)
    LEN_REBOOT (0x02)
  - Payload:
    -> first reboot command (1 byte) - value REBOOT_1_VALUE (0x58)
    -> second reboot command (1 byte) - value REBOOT_2_VALUE (0x32)
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_REBOOT (0xF3)
    ANS_LEN_REBOOT (0x01)
  - Payload:
    -> reboot confirmation code (1 byte) - value ANS_REBOOT_CODE (0x74)
  - CRC

### SAVE information in FLASH memory (+)

Apply to: All expansion types

Save up to 32 bytes in the Data Flash of the device

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_SAVE_IN_DATA_FLASH (0x17)
    LEN_SAVE_IN_DATA_FLASH (35)
  - Payload:
    -> address 2 bytes - LSB first
    -> size (1 - 32) 1 byte
    -> 32 bytes of information (all 32 bytes must be provided but only 'size'
    are actually saved)
  - CRC
- Expansion answer: None

### READ information from flash (+)

Apply to: All expansion types

Read up to 32 byte from the data flash of the device

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_GET_DATA_FROM_FLASH (0x18)
    LEN_GET_DATA_FROM_FLASH (3)
  - Payload:
    -> address 2 bytes - LSB first
    -> size (1 - 32) 1 byte
  - CRC
- Expansion answer:
  - Header:
    BP_ANS_SET (0x03)
    ANS_ARG_GET_DATA_FROM_FLASH (0x19)
    ANS_LEN_GET_DATA_FROM_FLASH (35)
  - Payload:
    -> address 2 bytes - LSB first
    -> size (1 - 32) 1 byte
    -> 32 bytes of information (32 bytes are always read but information is only
    in the first 'size' of them)
  - CRC


### SET TIMEOUT TIME TO FORCE DEFAULT VALUES (+)

Apply to: Opta Analog

Set Opta Analog Timeout time, if the expansion does not receive messages from
the controller for more than this time the outputs (DAC and PWM) are set
to the values set by the messages ARG_OA_SET_DAC_DEFAULT or ARD_OA_SET_DEFAULT_PWM
(see below for details).

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_TIMEOUT_TIME (0x31)
    LEN_OA_SET_RTD_UPDATE_TIME (0x02) // "shared define"
  - Payload:
    -> timeout in ms (2 bytes) - LSB first
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC


### SET OPTA ANALOG DAC CHANNEL VALUE (0 - 8191) AFTER COMMUNICATION TIMEOUT (+)

Apply to: Opta Analog

Set Opta Analog DAC default channel value, this value will be applied on the
channel when a timeout in the communication wiht the controller occurs

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARG_OA_SET_DAC_DEFAULT (0x3D)
    LEN_OA_SET_DAC (0x04)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> DAC value (2 bytes) (0 - 8191) - LSB first
    -> UPDATE DAC immediately (1 byte) - not used
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### SET OPTA ANALOG PWM (+)

Apply to: Opta Analog

Set Opta digital PWM channel values

- Controller request
  - Header:
    BP_CMD_SET (0x01)
    ARD_OA_SET_DEFAULT_PWM (0x33)
    LEN_OA_SET_PWM (0x09)
  - Payload:
    -> channel (1 byte) from 0 to 3
    -> pwm period in usec (4 bytes) - LSB first
    -> pwm pulse (high) in usec (4 bytes) - LSB first
  - CRC
- Expansion answer (ACK answer): 
  - Header:
    BP_ANS_SET (0x04)
    ANS_ARG_OA_ACK (0x20)
    ANS_LEN_OA_ACK (0)
  - Payload: no payload
  - CRC

### GET CHANNEL CONFIGURATION (+)

Apply to: Opta Analog

Get the actual function of the channel (to be used to verify when the expansion
has actually set up the requested function on that channel, in special cases
when outputs are issue just once in a while and so it is recommended to check
if the expansion has already set up the channel in advance)

- Controller request
  - Header:
    BP_CMD_GET (0x02)
    ARG_GET_CHANNEL_FUNCTION (0x40)
    LEN_GET_CHANNEL_FUNCTION (0x01)
  - Payload:
    -> channel (1 byte) from 0 to 7
  - CRC
- Expansion answer: 
  - Header:
    BP_ANS_GET (0x03)
    ANS_GET_CHANNEL_FUNCTION (0x40)
    LEN_ANS_GET_CHANNEL_FUNCTION (0x02)
  - Payload:
    -> channel (1 byte) from 0 to 7
    -> present channel function (1 byte) HIGH_IMPEDENCE (0), VOLTAGE_OUTPUT (1),
                                         CURRENT_OUTPUT (2), VOLTAGE_INPUT (3),
                                         CURRENT_INPUT (4), CURRENT_INPUT_LOOP_POWER (5), RESISTANCE_MEASUREMENT (6), DIGITAL_INPUT (7),
                                         DIGITAL_INPUT_LOOP_POWER (8), UNDEFINED (9),
                                         RESISTANCE_MEASUREMENT_3_WIRES (10)
  - CRC