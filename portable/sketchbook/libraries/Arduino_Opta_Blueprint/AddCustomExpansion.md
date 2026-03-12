# How to add a new expansion to Opta BluePrint

==============================================

## Introduction

This little guide is intended for people that want to add the support for a new
expansion to Opta BluePrint.

To add new expansion to the BluePrint library is necessary to provide:

- a new FW (this is the 'sketch' running on the expansion hardware that receives
  command from Opta Controller and send back information)
- a new <Something>Expansion class which is derived from the Expansion class
  contained in this Library (in the following this be referred as the 'expansion
  class', this is intended to be all the necessary files to have a fully usable
  expansion on the Opta Controller)

The FW and expansion class must be developed in a different folder/repository
and no changes or PR should be required in this Opta BluePrint library.
We suggest to create a separate library.

It is suggested that FW source files and the expansion class are put in the
same folder for simplicity reasons (this allows to share information between
the FW and expansion class in a easy way).

## How to make a new FW

The FW will be composed by the following file

- An OptaSomething.h file
- An OptaSomething.cpp file
- An OptaSomethingCfg.h file (optional ?)
- A Something.ino sketch that will be the entry point to use implementation
  in the Opta<Something> files

Please note that the filename convention indicated above is just to keep
consistency with the file naming used in BluePrint library, but it is optional.

We recommend the sketch Something.ino to be placed in a different folder
(something like 'firmware'): for example the Digital expansion FW resides in
folder ./firmware/Digital/Digital.ino but it uses the OptaDigital .h and .cpp in
the ./src folder

So a good starting point to develop an expansion FW is the following folder
structure (suppose your expansion is called "NewExpansion")

```
/root_folder_of_new_expansion_library
|-firmware/NewExpansion/NewExpansion.ino
|-src/
      |-OptaNewExpansion.h
      |-OptaNewExpansion.cpp
      |-OptaNewExpansionCfg.h (optional)
```

It is required that every FW version will be identified with a version number in
this form: MAJOR.MINOR.RELEASE
MAJOR, MINOR and release are bytes (uint8_t);
This defines are placed in OptaNewExpansionCfg.h

We recommend to use an #ifdef NEWEXPANSION clause in the file
OptaNewExpansion.cpp (or, alternatively, to use #ifndef ARDUINO_OPTA ) so that
your FW implementation file will not be compiled when the application sketch for
Opta Controller is build.

**_NOTE: in the following we suppose that the new expansion "name" is
"NewExpansion"_**

### OptaNewExpansion class

The files OptaNewExpansion file .h and .cpp must contain the implementation
of the class that handles the Expansion FW.

This class must inherit from Module class.
Module class declaration is contained in the OptaBlueModule.h header file so
that this file must be included in OptaNewExpansion.h

The class will be something like

```
class OptaNewExpansion : public Module {

};
```

**IMPORTANT**

The constructor for your new OptaNewExpansion class must always explicitly call
the Module class constructor `Module(TwoWire *tw, int _detect_in, int
_detect_out)` specifying:

- the TwoWire object used for I2C Blueprint communication
- the DETECT IN pin used by the new expansion
- the DETECT OUT pin used by the new espansion

The Module class defines some pure virtual functions so that every expansion
must implement these functions:

- `virtual uint8_t getMajorFw();` must return the Major FW version

- `virtual uint8_t getMinorFw();` must return the Minor FW version

- `virtual uint8_t getReleaseFw();` must return the Release FW version

- `virtual std::string getProduct();` must return a string
  containing a description of the new expansion. This description will be used to
  identify in a unique way the **_type_** of expansion. It must never happen
  that 2 different kind of expansions have the same description. The number of
  byte used is 32 (if the vector has more than 32 bytes, then only the first 32
  are actually used). Product description must be chosen carefully in order to
  avoid the fact that different producers use the same description, so a
  combination of the producer plus a unique identifier for the product should be
  enough. For example Opta Analog uses "Arduino Opta Analog" as product
  description (and only uses 21 byte).

- `virtual void goInBootloaderMode()` this function must implement a procedure to
  put the expansion in Bootloader mode in order to allow the upgrade of the FW. In
  case the Expansion does not provided the possibility to upgrade the FW just
  provide a dummy empty implementation.

- `virtual void readFromFlash(uint16_t add, uint8_t *buffer, uint8_t dim);` this
  function must implement the reading from flash (or anything similar) expansion
  device at address `add` of `dim` bytes, the bytes must be put into the `buffer`
  and the buffer must be at least `dim` long.
  Pay attention: read from flash function is in any case "driven" by an I2C
  transaction from Opta Controller so the maximum `dim` is 32 bytes (i.e. FLASH or
  EEPROM or any form of implementation is read 32 bytes at each transaction).
  Also the address is only 2 bytes wide because the I2C protocol reserve 2 bytes
  for the address.
  This is the default implementation you get for free by simply implementing
  this function (this means that at the controller level all is already
  implemented). Those limitations can be overcome by defining and handling a
  different I2C message.
  In case your expansion does not have this function just provide an empty,
  dummy implementation.

- `virtual void writeInFlash(uint16_t add, uint8_t *buffer, uint8_t dim);`this
  function must implement the writing into flash (or anything similar) expansion
  device at address `add` of `dim` bytes, the bytes copied from the
  `buffer` and so the buffer must be at least `dim` long.
  Pay attention: write into flash function is in any case "driven" by an I2C
  transaction from Opta Controller so the maximum writable `dim` is 32 bytes (i.e.
  FLASH or EEPROM or any form of implementation is written 32 bytes at each
  transaction).
  Also the address is only 2 bytes wide because the I2C protocol reserve 2 bytes
  for the address.
  This is the default implementation you get for free by simply v
  this function (this means that at the controller level all is already
  implemented). Those limitation can be overcome by defining
  and handling a different I2C message.
  In case your expansion does not have the possibility to write in flash just
  provide an empty, dummy implementation.

- `virtual void initStatusLED();` this function must provide a way to correctly
  initialize the HW for the LED status (on Arduino Opta expansions is present a
  RGB LED status which is used to signal the status of the expansion regarding
  the Address assignement: RED -> ready to get the I2C address, BLUE -> no
  address and waiting for other expansion to get their own address, GREEN -> I2C
  address acquired).
  In case Status led is not present just provide an empty, dummy implementation.

- `virtual void setStatusLedReadyForAddress();` this function must set the status
  LED in the state that indicates that the expansion has not I2C address but can
  acquire a new one.
  In case Status led is not present just provide an empty, dummy implementation.

- `virtual void setStatusLedWaitingForAddress();` this function must set the
  status LED in a state that indicates that the expansion has not address, but is
  not ready to acquire new one (typically is waiting for other expansions to get
  their address).
  In case Status led is not present just provide an empty, dummy implementation.

- `virtual void setStatusLedHasAddress();` this function must set the status LED
  in a state that indicates that the expansion has acquired a valid I2C address.
  In case Status led is not present just provide an empty, dummy implementation.

## Overriding basic virtual methods

Basic Module class has 4 virtual methods that need to be overridden in your
derived class.

- `begin()` must contain all the necessary initialization for the new expansion
- `update()` is the function that will be called repeatedly in your FW (see for
  example in the folder firmware for an example), this function should manage
  all the cyclical tasks that your application have to performs
- `end()` must contain all the shut down operation needed to proper stop the
  expansion
- `parse_rx()` will be called to give the opportunity to the expansion to
  recognize and parse the I2C messages received, in case the expansion recognize
  a message as to be handled it has to put in the tx_buffer the correct message
  answer for that controller request and **return the number of byte to be
  transmitted in the answer** (return -1 if the message is not handled by the
  expansion).

**VERY IMPORTANT**

All previous overridden method MUST always call the base version at the very
beginning (this is because the base Module has to have the chance to perform all
tasks common to every expansions).
An note about `parse_rx()`: when you call `Module::parse_rx()` in your overridden
`parse_rx()` it will certainly happen that the base Module itself will be
managing some messages, you don't need to deal with them, in this case just check for
the returned value. If the value is -1 this means that the message has not been
handled by the base version and you need to check if your expansion manages it,
if the value is greater or equal to 0 the base Module version has already took
care of the message and you just need to return the same value.

**VERY IMPORTANT**
Do not make any change to the `expansion_type` (an integer in the Module class).
This is set in Module class and must NOT be modified in any Custom expansion.

### Performance request
Each expansion performs Assign Address Process using a common class Module, which
is contained in OptaBlueModule files (.h and .cpp).
The Module::begin() function performs the assign address process at the very
beginning, but the function Module::update() is responsible to change the I2C address 
of the expansion when a new expansion is plugged in and the assign address process
is done a second time (this function has to be called in the NewExpansion::update()
function, see above). 
In the latter case the change of the address (being performed in 
Module::update()) is done in the main loop of the expansion.
So there is a delay between the time the expansion gets the message (I2C interrupt) and
the moment the espansion actually sets the address (in the main loop).
The actual set of the expansion, in other word, depends on how "fast" is the main
loop, however the main controller waits for a certain time and then retry the
process.
The expansion must set the new address as fast as it can in order to avoid retries
from the controller.
This means that is is strongly suggested that the main loop of the expansion (except for 
the call to the Module::update() function) only runs when the address is valid, 
so that when not a valid address has been set up, it can react very quickly to the 
address set request.
A good way to understand if the expansion has a valid address is:
```
if(wire_i2c_address <= OPTA_DEFAULT_SLAVE_I2C_ADDRESS || 
   wire_i2c_address >= OPTA_CONTROLLER_FIRST_TEMPORARY_ADDRESS) {
    // NO VALID ADDRESS ACQUIRED
}
```

## OptaNewExpansionCfg.h

This is an optional configuration file that contains all the configuration
`#define` used to configure the FW.
If the file is present it must be included only in OptaNewExpansion.h file: this
means that this file will contain information relevant only to the FW level and
won't "export" any information useful at the _application_ level (i.e. on the
Opta Controller side).

## Sharing information between FW and the controller

It might be good to have a place to put configurations or type definition that
are used both by FW and the Opta Controller application.
We suggest to call this file NewExpansionCommonCfg.h (always remember to use the
proper name of your expansion instead of 'NewExpansion' used here for simplicity).

This file will be included both in the OptaNewExpansion.h file (and this will
inform the FW) and in the NewExpansionExpansion.h (that name is ugly but it
simply means that the expansion class to be provided to the application /
controller level is SomethingExpansion and since we decided to use
'NewExpansion' as the name of our made up new expansion the name of the file
will NewExpansionExpansion.h)

So, at this point we can further populate our New Expansion library with the
files to be used at the controller/application level.

```
/rood_folder_of_new_expansion_library
|-firmware/NewExpansion/NewExpansion.ino
|-src/
      |-OptaNewExpansion.h
      |-OptaNewExpansion.cpp
      |-OptaNewExpansionCfg.h (optional)
      |-NewExpansionCommonCfg.h (common share configuration between FW and application)
      |-NewExpansionExpansion.h (header file for the expansion class at application level)
      |-NewExpansionExpansion.cpp (implementation file for the expansion at the application level)

```

Just for completeness the following expresses the "allocation" of the files to
FW or application.

```
/rood_folder_of_new_expansion_library
|-firmware/NewExpansion/NewExpansion.ino
|-src/
      |-OptaNewExpansion.h     (FW header class)
      |-OptaNewExpansion.cpp   (FW implementation class)
      |-OptaNewExpansionCfg.h  (FW configuration, optional)
      |-NewExpansionCommonCfg.h (FW / APP shared configuration and types)
      |-NewExpansionExpansion.h (APP header class)
      |-NewExpansionExpansion.cpp (APP implementation class)
```

## The NewExpansion application class

Class `NewExpansionExpansion` must be derived from Expansion class.

It is important to understand that Expansion class implements a very simple
model valid for (hopefully) every expansion type.
From the Expansion class perspective each expansion is just a bunch of
"registers" (there are 2 kind of registers, integer `iregs` and floating point
`fregs`).
Those registers are implemented using `maps` so that you can dynamically add or
remove registers on the fly.
Each register is identified by an address that the user can define.
For example have a look to DigitalExpansionAddress.h: the status of an output is
represented by a register

- ADD_DIGITAL_0_OUTPUT represents the address of the register containing the
  status of the digital output 0,
- ADD_DIGITAL_1_OUTPUT represents the address of the register containing the
  status of the digital output 1
  ...and so on.
  When you want to set the digital output 0 of the expansion to an high level you
  set 1 into the register at the address ADD_DIGITAL_0_OUTPUT and you send this to
  the expansion.

The Expansion class essentially provides 3 methods:

- read() to read a register
- write() to write a register
- execute() to execute an operation (such sending the new output status to the
  expansion to switch on an output).

Every operation performed on an expansion is implemented using these three basic
functions.

To set up a programmable function channel for example:

1. you write a bunch of register holding the configuration of the channel
2. you execute a _configure_ (BEGIN_CHANNEL_AS_ADC), this will use the content
   of the proper registers to send a message to expansion (via the Controller
   class) that will inform the expansion about the desired configuration

In the same way you can get information from the expansion:

1. write register holding information about what you want to get
2. execute a _get_ operation (GET_SINGLE_ANALOG_OUTPUT)
3. read the registers that are filled by the execute function with the
   information you wanted to get from the expansion

This generic abstraction should work on every possible expansion.

Of course, read(), write() and execute() are virtual function so that you can
customize them for the needs of your expansion.

For example to spare memory and be more efficient the DigitalExpansion class
override the standard Expansion::write() function. When you write a digital
output register such ADD_DIGITAL_0_OUTPUT (for example) the overridden function
will set a bit in the register at the address ADD_DIGITAL_OUTPUT.
So that the register at the address ADD_DIGITAL_0_OUTPUT is not really
allocated and does not take space in memory, moreover the register at the
address ADD_DIGITAL_OUTPUT (that maps all the outputs as a bit field integer) is
immediately ready to be transferred to the expansion to update the output during
an execute.

### About the execute() function

The execute() function has always the same structure (and we strongly suggest
the same structure on each derived class): it takes an integer as parameter that
identify the operation to be performed (set an output / read an input /
configure a channel and so). Depending on the action to be executed the execute()
function always perform a I2C transaction to communicate with the expansion.
To this purpose I2C_TRANSACTION macro is used.

This macro (I2C_TRANSACTION) is conceived to be generic and takes 3
parameters:

- a pointer to a function preparing the message to be sent on I2C bus (this
  function returns the number of bytes to be transmitted)
- a pointer to a function able to parse the answer received by the controller (
  returns true if parsing is correct)
- the number of bytes expected in the answer

So suppose the we want to write an Arduino like `digitalWrite()` function for
an expansion, these are the operation required:

1. the digitaWrite() function sets the registers holding the information about
   the output status
2. execute the operation SET_DIGITAL_OUTPUT
3. the execute() function "triggers" an I2C transaction using I2C_TRANSACTION
   macro
4. this function use the "prepare" function pointer to understand how to set up
   the transmission buffer (this typically will write the information held in
   the registers into the transmission buffer)
5. send the transmission buffer using the Controlled function send()
6. wait for the controller to return an answer
7. parse the received answer using the pointer to the "parsing" function (this
   will write perhaps some other registers that can be then read to return some
   information, although this is not the case of digitalWrite())

The I2C_TRANSACTION already takes cares of timeouts and will call the failed
communication callback if set.

**IMPORTANT**
There are 3 operations the basic expansion class takes care which are common to 
all classes (WRITE in flash, READ in flash, get FW version).
If you want to use these operation remember to call `Expansion::execute();` in 
your `defaul` case switch of execute. This will use the base `execute()` function
performing these basic operations.


**IMPORTANT**
There is another important point to be remembered when overriding the execute()
function (and this, of course is something every expansion must do in order to
introduce specific operations): at the end of each **execute** function remember
to call `ctrl->updateRegs(*this);`.
Why this? Well this library works using copies of expansion objects: you ask the
controller to give you an expansion using OptaController.getExpansion() and the
controller will return a copy of the proper expansion object that is held inside
the controller. This is safe because you don't allocate anything and your copy
will be destroyed as soon it goes out of scope, however when you work on the
expansion, you are working **on a copy**. This means that the content of the
registers of the expansion held by the Controller are no more aligned with the
ones of your copy.
Calling `ctrl->updateRegs(*this);` will copy back the content of yours registers
to the expansion object held by the controller, so that if you get another
(perhaps in a different function) you don't lose any changes you made elsewhere.
For an example look to a digital expansion execute function.

### Mandatory functions to be implemented in NewExpansionExpansion class

In addition to the copy of i2c_transaction function, the following function
must be always implemented in the NewExpansionExpansion class:

- a copy constructor in the form
  `NewExpansionExpansion::NewExpansionExpansion(Expansion &other)`
  This copy constructor is crucial. Simply copy an existing one and just change
  the types.
  If you look at the different copy constructors already implemented they all do
  the same operation. They copy all the information from the object held by the
  controller into your object (also the registers), but only if the object held by
  the controller is of the same type (otherwise invalid expansion information are
  returned). This is important: you don't need to worry about allocating or
  deallocating object (this is managed by the controller), you always get a copy
  of your expansion that will be automatically destroyed when it goes out of
  scope.
  _IMPORTANT NOTE_: the copy constructor must use a special Controller function
  to know the type of the Expansion (see below section NewExpansion enumerative
  type).

- a static `makeExpansion()` method that returns an `Expansion *` pointer and that
  actually allocates an object of the NewExpansionExpansion class via `new` C++
  operator. This function will be used by the Controller to perform the
  allocation of the object in the correct way (this way the controller is
  agnostic of the expansions but can still make them).
  This function should not be called directly by an application, since it is
  intended that the Controller makes and deletes the objects.
  Besides the use of this function to allocate an object would be completely
  meaningless because it will lack of the necessary information that only the
  Controller can fill in.

- a static `getProduct()` method that return the string identifying the type of
  the expansion. This function MUST return the exact same string returned by the
  getProduct() function defined at the FW level. If you look at the
  implementation of these two functions in the already implemented class you'll
  see that they return the same string (which is a shared configuration
  parameter contained in the <Expansion>CommonCfg.h header file). 

- a static `startUp(Controller *)` method that will perform the
  initialization of your expansions (of all the expansions of the same type).
  The purpose of this function is very important however hard to explain.
  Suppose that you have an expansion with some programmable output function (Opta Analog
  is a good example, because each channel can be RTD, ADC, DAC). The controller
  application will probably set up all the desired channel function once in the
  setup() function (the usual Arduino initialization function). So the
  controller will execute the setup() function just once at the beginning
  (suppose setting channel 0 as DAC and channel 1 as ADC and so on).
  After that the Controller just uses the expansion and has no need to configure
  the channels of the expansion anymore. Now suppose that you have to power down
  you expansion, but you don't want to reset also the controller (maybe because
  the controller is controlling other expansions you don't want to shut down).
  After you perform the operation on shut down expansion you power it up again.
  The Controller sees the "new" expansion, performs a new expansion discovery
  process and assign address and all work seamless, but... Well the expansion you
  shut down lost its configuration, but the Controller did not perform again an
  initial setup() function because it was not reset, so the configuration is
  lost. The expansion, in this case, is not more usable until you reset the
  controller and the setup() function is performed again.
  The purpose of the static `startUp(Controller *)` function is to avoid cases like that:
  this function must provide a way to reinitialize all the possible expansions
  of that type to the desired configuration. This can be a complex task, in the
  Analog expansion a specific class to hold configuration information for each
  possible Analog expansion is implemented in AnalogExpansionCfg.h file.
  This function will be passed to the controller so that controller can
  re-initialize the expansions every time a discovery expansion process is
  finished.
  The startUp function are always called after the assign address process is completed.
  Each startUp callback has to set up properly all the expansion of a certain type
  in 2 possible situations:
  - when the controller is re-programmed (since single expansion are not reset)
    the startUp must send to all the expansions all that is need to put the expansion
    in its "default state" (output off and so on...)
  - when a single expansion is hot plug in into the chain (or one of the expansion
    is reset for some reason) the startUp function must send to all the expansions 
    of that type, all that is needed to put the expansion in last known state.
  This is the approach used with digital and Analog expansion, however he behavior 
  of a start up function could depend on how expansion FW works:
  if your custom expansion type does not need such function just write an empty
  function. 

Then the new expansion class can define whatever custom additional function: the
important point here is to remain to read / write / execute paradigm.

## Controller additional initialization function for Custom expansion

If you have followed the previous indications, you should have a FW and an
application expansion class.
Typically for "Arduino" expansion the processes to write sketches for Opta
Controller that uses Arduino expansion (for example Opta Digital and Opta Analog)
is quite straightforward:

1. call `OptaController.begin()` during `setup()`: this will properly initialize
   the controller
2. if needed proper initialize the expansions you want to use with the right
   configuration (this can be performed in various way, please have look at the
   example provided with this library for more details)
3. wherever you need ask the controller to give you a copy of an expansion using
   OptaController.getExpansion(index_of_expansion)
4. check for the validity of expansion using the bool() operator: the expansion
   will be valid only if the type of expansion you requested is actually the
   same than the "hardware" level
5. Use the expansion (set output, get input and so on)
6. as soon as you expansion copy will go out of scope it will be automatically
   deleted because is a copy and is not allocated on the heap (so no worries for
   memory management, the controller will destroy the expansion object it holds when
   needed)

This should be quite simple (have a look at the provided example), however there
is a missing point for custom expansion: the controller is not aware of the fact
that additional custom expansion exist.
How to deal with that?

**VERY IMPORTANT**
When writing a sketch (an example) for a custom expansion (i.e. an expansion
which is not a direct Arduino product) you must **register** the new expansion
to the Controller using `OptaController.registerCustomExpansion()` just after
the `OptaController.begin()` function.
The `OptaController.registerCustomExpansion()` parameters are the function you
added to your NewExpansionExpansion class: see previous paragraph.

So typically you must perform a call like:

```
OptaController.registerCustomExpansion(NewExpansion::getProduct(),
                                       NewExpansion::makeExpansion, 
                                       NewExpansion::startUp);
```

Please note that I am assuming that your custom expansion is called
`NewExpansion` but this is of course a generic name that has to be converted in
the actual expansion name.

## NewExpansion enumerative type

The use of `registerCustomExpansion` leads to an important point: custom expansions get a
dynamically calculated "enumerative" type.

For example: Opta Analog is identified by a type number equal to
EXPANSION_OPTA_ANALOG (the actual value is unimportant and should never be used,
but in this particular case is 4).
This means that if you use `Controller.getExpansionType(i)` and at the index `i`
you have an Analog expansion you'll get always `EXPANSION_OPTA_ANALOG` value (i.e. 4).

This happens because the controller is aware of the existence of Analog
expansion even if they do not call the register function (this is a privilege
reserved for Arduino expansions).

But when you register a custom expansion via `registerCustomExpansion` this is not
true anymore: the Controller registers the expansion and assigns to the
expansion type a unique value.

This value is returned by the function `registerCustomExpansion` itself (or -1 if
something's wrong).

You don't need to save this value since it can be always retrieved using
the controller function `OptaController.getExpansionType(string)`.
As the parameter of this function always use the NewExpansion::getProduct()
function.

So suppose that your custom expansion is at the position 2 in the chain of
expansions, to know that the expansion is of New Expansion type you can do
something like:

- to get the dynamical enumerative type assigned to your expansion type use
  `OptaController.getExpansionType(NewExpansion::getProdut())`
- to get the dynamical actual enumerative type of the expansion at the position
  2 (this is related to the actual hardware) use `OptaController.getExpansionType(2)`

`OptaController.getExpansionType(NewExpansion::getProdut())` returns
the type of the product (dynamically calculate) equal for each product.
`OptaController.getExpansionType(index)` while the expression returns the actual
type the controller assigned to the custom expansion hardware.

Please note that the use of getExpansionType(index) is not so much interesting
at the practical level.
You can always avoid to use it by asking for an expansion using
`NewExpansionExpansion exp = OptaController.getExpansion(index)` and then
checking for the validity of the returned expansion using the bool() operator
(i.e. `if(exp)` -> this will be true only if the expansion at index is actually
of type NewExpansion).

At this point we can understand the general form of the NewExpansion copy
constructor:

```

NewExpansionExpansion::NewExpansionExpansion(Expansion &other) {
  NewExpansionExpansion &de = (NewExpansionExpansion &)other;

  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = other.getCtrl();
  index = 255;

  if (ctrl != nullptr) {
    if (other.getType() ==
        ctrl->getExpansionType(NewExpansionExpansion::getProduct())) {
      iregs = de.iregs;
      fregs = de.fregs;
      type = other.getType();
      i2c_address = other.getI2CAddress();
      ctrl = other.getCtrl();
      index = other.getIndex();
    }
  }
}
```

**IMPORTANT NOTE**: The dynamical type is assigned by the Controller after the
assign address process and the expansions are discovered.
If you don't have any expansion NewExpansion attached and you use
`Controller.getExpansionType(NewExpansionExpansion::getProduct())` you'll get -1
until a NewExpansion is attached to Opta (this ensure that you get only
NewExpansionExpansion objects actually related with a real hardware).

## About I2C Messages Definition

To communicate with expansion a protocol over I2C is defined.
For organisation purpose the message "defines" have been splitted in different
files for different expansions.

- OptaBlueProtocol.h contains a few helper general defines to help in message
  definition (you should include this file in your message definition file)
- OptaModuleProtocol.h contains the definition of the messages that have to be
  handled by every expansion. You don't have to worry about this messages and
  you don't have to include this file in your expansion library, as far as you
  follow the previous instructions your expansion will manage correctly those
  messages.
- OptaDigitalProtocol.h contains all the messages related to Opta Digital
- OptaAnalogProtocol.h contains all the messages related to Opta Analog

Feel free to re-use already defined messages in your expansion (for example you
may re-use message to set digital output defined for Opta Digital, as far as it
fits your needs).

We suggest to keep the definition of your custom messages (i.e. specific
messages handled by your custom expansion) in a separate header file (always to
keep consistency with Opta BluePrint library), so the final structure of your
custom expansion library should be something like:

```
/rood_folder_of_new_expansion_library
|-firmware/NewExpansion/NewExpansion.ino
|-src/
|-OptaNewExpansion.h     (FW header class)
|-OptaNewExpansion.cpp   (FW implementation class)
|-OptaNewExpansionCfg.h  (FW configuration, optional)
|-NewExpansionCommonCfg.h (FW / APP shared configuration and types)
|-NewExpansionExpansion.h (APP header class)
|-NewExpansionExpansion.cpp (APP implementation class)
|-OptaNewExpansionProtocol.h (I2C messages definition)
```

IMPORTANT:
The header file OptaNewExpansionProtocol.h must always include
OptaBlueProtocol.h so that you can use same configuration and same messages part
definition.

OptaNewExpansionProtocol.h will be then included both in application level file
(NewExpansionExpansion.h) and FW level file (OptaNewExpansion.h).
