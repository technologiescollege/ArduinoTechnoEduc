# Arduino_OptaBluePrint

This library is intended to be used on **Arduino OPTA** (use Controller class) and
**Arduino OPTA DIGITAL** (use Module class) expansion.
A single Arduino OPTA can control several Arduino Digital module expansions.
More different types of expansion are expected and this library is ready to add
them.

## HW

Arduino OPTA and Arduino OPTA DIGITAL shares an I2C bus and a sort of UART bus (
OPTA Tx is wired on all OPTA DIGITAL Rx).

Arduino OPTA has a PIN that goes _toward_ expansions (other Arduino Digital), this
PIN is the DETECT PIN. Please note that this pin can only be input due to HW
implementation.

Each expansion (Arduino Digital) has 2 PIN connected on the expansion bus.

- DETECT IN goes toward the OPTA controller
- DETECT OUT goes toward other Arduino DIGITAL possibly connected

The Serial bus is used only to upgrade the FW on the expansions (they do not have 
a USB connector mounted). Basically the controller send an I2C message to expansion 
asking to go in bootloader mode, once the bootloader is active the Serial Line 
is used to download the new FW into the expansion. Then the controller tells the 
bootloader to reset the board and the upgrade is done.

### I2C

**Arduino OPTA** is always the master of the I2C, while the other expansions as
**Arduino OPTA DIGITAL** are always slave on the I2C.

## ASSIGN ADDRESSES PROCESS

### Purposes of the ASSIGN ADDRESSES PROCESS

Controller (OPTA) will be always placed on the leftmost position.
One or more expansions (not necessarily of the same type) can be chained on the
right of the controller.

```
+------------+     +-------------+     +-------------+     +-------------+
|            |     |             |     |             |     |             |
| Controller |<--->| Expansion 0 |<--->| Expansion 1 |<--->| Expansion 2 |  [...]
|            |     |             |     |             |     |             |
+------------+     +-------------+     +-------------+     +-------------+
```

At the end of the ASSIGN ADDRESSES PROCESS:

1. All expansion must have always the same address (this because the controller
   sketch must continue to address the same device, if addresses were chosen
   randomly the controller logic will have to change depending on the assigned
   addresses)
2. The expansion closest to Controller must have address 0xB, other expansions
   must have address that are increased of 1 going in the right direction (so
   in our case shown above:
   - Expansion 0 gets address 0x0B
   - Expansion 1 gets address 0x0C
   - Expansion 2 gets address 0x0D
     always).
3. This process must not be depending on the order the devices are powered up (or
   reset, if you will). As soon as 1 device is wired and powered up all the
   ASSIGN ADDRESSES PROCESS is refreshed.

### PIN usage

CONTROLLER DETECT PIN is always an INPUT: if HIGH there is not not need to assign
addresses, if LOW there are Expansions without an assigned address. At the reset
the controller always checks the DETECT pin if it LOW the Controller enter
into the ASSIGN ADDRESSES MODE. The controller remains that mode until the
DETECT pin goes HIGH.

At the reset each expansion calls the `reset()` function (called by `begin()`).
The `reset()` function performs the following operation:

- stops and de-initialize the I2C (this guarantee that expansion does not
  receive any message until the I2C is properly initialized again)
- puts the DETECT IN pin to LOW (this signals the Expansion has not valid
  address)
- puts the DETECT OUT pin LOW for OPTA_MODULE_DETECT_OUT_PIN_LOW ms (this
  signals other expansions that one expansion on the left has been reset)
- puts the DETECT OUT pin as INPUT_PULLUP
- remain in this state until the DETECT OUT becomes HIGH
- as soon as the DETECT OUT becomes HIGH the I2C is initialized as slave with
  the default address (0x0A)

After reset (see `updatePinStatus()`) the pin usage on Expansion depends upon the
address acquisition.
If address has NOT been acquired the DETECT IN (toward Controller) is kept LOW.
If address has been acquired:

- the DETECT IN (toward Controller) is briefly configured as INPUT to check if
  the DETECT OUT of the expansion on left is LOW (the expansion on the left is
  resetting its own address)
- the DETECT OUT is checked, if it becomes LOW this mean that an expansion has
  been inserted on the right
  The previous 2 conditions and the reception of the RESET message from the
  controller trigger again the function `reset()` so that the complete ASSIGN
  ADDRESS PROCESS is performed when one of this event is detected by one of the
  Expansion that already have an address.

### I2C assing process

At start-up the controller send to the address 0x0B (this is guarantee to be always
the first device on the right of the controller when addresses have been
assigned) the RESET message: this is done to deal with the case that expansions
have already addresses assigned and the controller is somehow reset or switched
off and on again. This message will trigger the first expansion to call the
`reset()` function (see above).

After OPTA_CONTROLLER_SETUP_INIT_DELAY if the DETEC PIN is LOW the Controller
starts the first phase of the assign addresses procedure.
During this phase the controller send:

1. a set address message to default address (0x0A); the address is set to
   OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS + OPTA_CONTROLLER_MAX_EXPANSION_NUM
   (0x0B + 0x0A) at first and then increased of one after each expansion has
   successfully set the address.
   All expansions, due to the "reset" procedure, have the I2C in a de-initialized state,
   except the rightmost one (because its DETECT OUT is HIGH): for this reason only
   the rightmost expansion receives the message and configure its address.
   Once the Expansion gets the address it put its DETECT IN pin HIGH to leave
   the other expansions on the left the chance to get their own address.
2. a get address and type message is sent to the last address set: this is done
   to ensure that the expansion has correctly obtained the address. With this message also
   a code that represent which expansion is attached is sent to the controller
   so that it can know what expansion types is dealing with (this make sense only
   if other expansion types than OPTA DIGITAL will be available - but this is expected).
   If an answer is received the address to be sent is increased by one and the
   controller start over: does the DETECT PIN is LOW? if yes go to the point 1
   otherwise continue with normal operations.

At the end of this phase:

- all expansion have a temporary address
- the controller knows the number of expansions and their types (and their temporary addresses)

However the addresses are not always the same: the first device on the right of
the controller will get an address that depends on the number of expansions
chained together.

The Controller detects the end of this first phase by the fact that the DETECT
PIN has a transition LOW -> HIGH.
When the Controller detects that it starts the second phase.
Starting from the device with the higher address (which is the one on right of the
controller), the controller reassign the addresses starting from 0x0B
(OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS).
At the end of this phase all the expansions have always the same address that
only depends upon the position in the expansion chain: from left to right 0x0B,
0x0C, 0x0D and so on...

During this second phase the Controller also creates the data structures used
to deal with all the expansions.
In particular:

- num_of_exp --> the number of expansions detected
- exp_add[] --> is an array that holds the addresses assigned to each
  expansion (index 0 is the leftmost expansion index [num_of_exp - 1] is
  the rightmost expansion)
- exp_type[] --> is an array that holds the type of each expansion (index 0
  is the leftmost expansion index [num_of_exp - 1] is the rightmost expansion)
- exp_index[] --> is an array holds the index in array of data structures
  that holds all the information related to a certain type of expansion
  To explain this lets have for example the following situation:

  1. expansion type A (index 0)
  2. expansion type B (index 1)
  3. expansion type A (index 2)
  4. expansion type B (index 3)
     So we have 2 expansion of type A and 2 expansion of type B.
     The controller then will create 2 arrays of different structures (classes) to
     hold the information of 2 different types of expansion.
     At the present only Digital expansions are defined and the correspondent array
     is kept in the variables:

  ```
  - uint8_t opta_digital_num;
  - OptaDigitalData *opta_digitals
  ```

  Suppose that OPTA digital is type A and that there is a different kind of
  expansion (maybe an OPTA ANALOG) which is the type B.
  Then we will have

  ```
  opta_digital_num = 2;
  opta_analog_num = 2;
  opta_digitals is an array with 2 position
  opta_analogs is an array with 2 position
  ```

  in this situation

  - exp_index[0] is 0 to indicate that (being the device at index 0 of type A
    or DIGITAL, this is got from exp_type[0]) all the information related
    to this device will be hold the opta_digitals[0] array position.
  - exp_index[1] is 0 to indicate that (being the device at index 1 of type B
    or ANALOG, this is got from exp_type[1]) all the information related
    to this device will be hold the opta_analogs[0] array position.
  - exp_index[2] is 1 to indicate that (being the device at index 2 of type A
    or DIGITAL, this is got from exp_type[2]) all the information related
    to this device will be hold the opta_digitals[1] array position.
  - exp_index[3] is 1 to indicate that (being the device at index 3 of type B
    or ANALOG, this is got from exp_type[3]) all the information related
    to this device will be hold the opta_analogs[1] array position.

  Using this structure and given an index (the index is alway the physical position of the
  expansions in the chain from left to right starting from 0) the controller knows always which
  expansion is dealing with and how to retrive / write information to it.

  Say we want to know the status of a pin on expansion which has index 2.
  The controller goes in exp_type[2] and gets the type of the device (type A or
  DIGITAL in our example), then it goes to exp_index[2] and retrive the position

  1. With this it will retreive information from opta_digitals[1].

## About classes

Module is the base class for Expansions.
This class handles the assign addresses process and so it parse the related messages
and handle the related Pin setting.

There are mainly 4 public functions in this class:

1. begin() -> initialize HW stuff
2. parse_rx() -> parse the messages related to assign addresses process
3. update() -> just call updatePinStatus() which is a private function handling the
   pin setting related to assign addresses process
4. end() -> does nothing

The OptaDigital is the class specific for the OptaDigital management.
So it handles all the HW stuff specific for that HW: in particular analog conversion,
digital input and digital output.

This class overrides the functions of the Module class, however the base class functions
are called into the derived class versions.
THIS IS IMPORTANT TO REMEMBER when new hw will be added: ALWAYS CALL the base
version function of the Module class, since this functions deal with the stuff that
all expansions are suppose to handle (such the assign addresses process).

In particular it is supposed that a base Module class will never be instantiated.
