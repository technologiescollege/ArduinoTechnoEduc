# uStepper S-lite

The library contains support for driving the stepper, reading out encoder data. A few examples are included to show the functionality of the library.
The library is supported and tested with in Arduino IDE 1.8.12.

For more information, visit www.ustepper.com

## Installation

Installation is split into two parts - Hardware and Library. Both are required to use the uStepper S-lite boards.

### Hardware Installation 

### Instructions for users with the hardware for the original uStepper installed

To add hardware support for uStepper in the Arduino IDE (1.8.5+) do the following:
 - Open Arduino
 - Go to "Tools->Board->Boards Manager..."
 - Search for "uStepper"
 - Choose the "uStepper" board and press "Remove" button
 - close the Boards Manager
 - Go to "File->preferences"
 - Almost at the bottom there is a field stating: "Additional Boards Manager URLs" replace your currently inserted uStepper URL this URL: https://raw.githubusercontent.com/uStepper/uStepperHardware/master/package_ustepper_index.json
 - Press OK
 - Go to "Tools->Board->Boards Manager..."
 - Go to the bottom (after it has loaded new files) select "uStepper by ON Development IVS" and press install

### Instructions for users new to uStepper

To add hardware support for uStepper in the Arduino IDE (1.8.5+) do the following:
 - Open Arduino
 - Go to "File->preferences"
 - Almost at the bottom there is a field stating: "Additional Boards Manager URLs" insert this url: https://raw.githubusercontent.com/uStepper/uStepperHardware/master/package_ustepper_index.json
 - Press OK
 - Go to "Tools->Board->Boards Manager..."
 - Go to the bottom (after it has loaded new files) select "uStepper by ON Development IVS" and press install

You have now added uStepper hardware support and should be able to select uStepper under tools -> boards.

### Library Installation

To add the uStepper S-lite library do the following:
- Open Arduino IDE (Version 1.8.5 or above)
- Go to "Sketch->Include Library->Manage Libraries..."
- Search for "uStepper S-lite"
- Select "uStepper S-lite" and press install
- Close Library Manager

## Documentation
The documentation for this library can be found at the following URL:

http://ustepper.com/docs/ustepperslite/html/index.html

### MAC (and a few Windows) Users:
In order to make the uStepper S-lite show up in the "ports" list, you need to install the VCP driver from the following link: 
https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

##To Do
- Update keywords.txt
- Better comments
- Variable cleanup

## Known Bugs
- none

## Change Log
1.1.0:

- Fixed problem with deceleration not being used on stop and change of speed
- Added functionality to choose between hard stop and soft stop (hardStop() and softStop())

1.0.0:

- Fixed dropin and PID
- Complete rewrite of step generation algorithm, to increase performance and accuracy
- Added CLI interface to adjust dropin parameters and store to EEPROM
- Added docs
- ALOT of bugfixes 

0.1.1:

- Updated uStepperServo example
- Removed timer1 tampering from uStepperServo.cpp
- Updated Readme

0.1.0:	

- Initial release

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">uStepper</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="www.ustepper.com" property="cc:attributionName" rel="cc:attributionURL">ON Development</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
