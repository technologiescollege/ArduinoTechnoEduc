/* -------------------------------------------------------------------------- */
/* FILE NAME:   middleWareInterface.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240212
   DESCRIPTION: This sketch provide an example of a simplified interface to 
                deal with Opta Epansion, it only uses 3 function
                read, write and execute
                This interface will be the same for every supported expansion
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla 
                Public License (MPL), v 2.0. You can obtain a copy of the MPL 
                at http://mozilla.org/MPL/2.0/. 
   NOTES:       This example is for Opta DIGITAL                                                               */
/* -------------------------------------------------------------------------- */

/* Basics: 
 * ------
 * Expansions are conceived as a set of Register held at a certain address 
 * Addresses defined for each expansion can be found in a specific header file
 * (For Opta DIGITAL is DigitalExpansionAddresses.h) 
 * Along with addresses a set of possible operations are defined for a certain
 * expansion type.
 * To use this kind of interface you need a Controller: this is responsible 
 * to give you a pointer to a certain Expansion 
 * At this point every operation performed on the Expansion is a sequence of
 * 1. write some registers to setup operation parameters (optional) 
 * 2. execute the required operation 
 * 3. read one or more register to get the result of the operation (optional) 
 *
 Please note that this kind of interface is actually used by Expansions
 sub-classes (have a look to DigitalExpansion). This subclasses offer the same
 functions with a set of member function but the functions will be different 
 depending on the Expansion type (Analog expansion class will have a different
 interface respect to the Digital expansion, however BOTH will support this
 simplified interface of write / read / execute)

 In this simplified interface some basics OptaController function might be useful
 - getExpansionNum() return the number of expansion currently attached
 - getExpansionType(i) return type of the i-th expansion ExpansionType_t*/

#include "OptaBlue.h"

using namespace Opta;

/* an expansion pointer is needed for each for each possible expansion */
Expansion *expansions[OPTA_CONTROLLER_MAX_EXPANSION_NUM];

/* function to print the expansion type nicely */
void printExpansionType(ExpansionType_t t);
/* function to print a 16 bit value always using 5 numbers */
void printUint16(uint16_t v);


/* This function will populate the array an can be called in the main loop */
void getDigitalExpansions() {
  for(int i = 0; i < OptaController.getExpansionNum(); i++) {
    if(expansions[i] == nullptr) {
       /* !!! TO GET the pointer to the expansion call getExpansionPtr(device) */
       expansions[i] = OptaController.getExpansionPtr(i);
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                 SETUP                                      */
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  /* initialized to show some result on the serial line */
  Serial.begin(115200);
  delay(2000);
  /* !!! ALWAYS call begin() for the OptaController */
  OptaController.begin();
}

/* -------------------------------------------------------------------------- */
/*                                  LOOP                                      */
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  /* !!! ALWAYS call update() in the main loop to support expansion discovery */
  OptaController.update();
  /* call the function to populate the array of Expansions */
  getDigitalExpansions();

  /* loop over the expansion to control them */
  for(int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    /* !!! ALWAYS check for the validity of the expansions */
    if(expansions[i] != nullptr) {
         /* CHECK FOR THE EXPANSION TYPE - this is important because otherwise 
          * you can request an operation or read and write a register that are 
          * not supported by this kind of expansion (however there is no harm
          * in doing that since the operation is not actually execute and
          * execute will return an error code)*/
	 if(expansions[i]->getType() == EXPANSION_OPTA_DIGITAL_STS ||
            expansions[i]->getType() == EXPANSION_OPTA_DIGITAL_MEC) {
           
            /* ####################################
             * EXAMPLE 1  
             * #################################### */

            /* ----------------------------------------- */
            /* get and print information about expansion */
            /* ----------------------------------------- */
            Serial.print("\n\nExpansion index: ");
            Serial.print(expansions[i]->getIndex());
            Serial.print(" type: ");
            printExpansionType(expansions[i]->getType());
            Serial.print(" I2C address: ");
            Serial.println(expansions[i]->getI2CAddress());

            /* ####################################
             * EXAMPLE 2 - method 1  
             * #################################### */

            /* ------------------------------------------- */
            /* set digital output - METHOD 1 use bit masks */
            /* ------------------------------------------- */
     
            /* DIGITAL has 8 output so each is mapped on a bit of an uint8_t 
             * you can set/reset each pin by setting/resetting the corresponding
             * bit */
            Serial.println("Setting pin 1 3 5 7");
            /* setting digital output 1, 3, 5, 7 on */
            unsigned int digital_out = 0;
            digital_out |= (1 << 1);
            digital_out |= (1 << 3);
            digital_out |= (1 << 5);
            digital_out |= (1 << 7);

            /* !!! WRITE the register corresponding to the digital output */
            expansions[i]->write(ADD_DIGITAL_OUTPUT, digital_out);
            /* !!! EXECUTE the operation to set the digital output */
            unsigned int  err = expansions[i]->execute(SET_DIGITAL_OUTPUT);
            /* execute returns 0 if all is correct or an error code defined in
             * OptaExpansion.h */
            //Serial.println("(1) err = " + String(err));
            
            /* keep the output in that state for 2 seconds */
            delay(2000); 

            /* ####################################
             * EXAMPLE 2 - method 2  
             * #################################### */

            /* --------------------------------------------------------- */
            /* set digital output - METHOD 2 use individual PIN register */
            /* --------------------------------------------------------- */
     
            /* EACH digital output has an individual register from
             * ADD_DIGITAL_0_OUTPUT to ADD_DIGITAL_1_OUTPUT 
             * writing 0 in these register reset the pin 
             * writing 1 in these registers set the pin 
             * The actual operation is performed when executed is called */
            
            Serial.println("ReSetting pin 1 3 5 7, Setting pin 0 2 4 6");
            /* setting digital output 1, 3, 5, 7 OFF */
            expansions[i]->write(ADD_DIGITAL_1_OUTPUT, (unsigned int)0);
            expansions[i]->write(ADD_DIGITAL_3_OUTPUT, (unsigned int)0);
            expansions[i]->write(ADD_DIGITAL_5_OUTPUT, (unsigned int)0);
            expansions[i]->write(ADD_DIGITAL_7_OUTPUT, (unsigned int)0);
            
            /* setting digital output 0, 2, 4, 6 ON */
            expansions[i]->write(ADD_DIGITAL_0_OUTPUT, (unsigned int)1);
            expansions[i]->write(ADD_DIGITAL_2_OUTPUT, (unsigned int)1);
            expansions[i]->write(ADD_DIGITAL_4_OUTPUT, (unsigned int)1);
            expansions[i]->write(ADD_DIGITAL_6_OUTPUT, (unsigned int)1);

            /* !!! EXECUTE the operation to set the digital output */
            err = expansions[i]->execute(SET_DIGITAL_OUTPUT);
            /* execute returns 0 if all is correct or an error code defined in
             * OptaExpansion.h */
            //Serial.println("(1) err = " + String(err));
            
            /* keep the output in that state for 2 seconds */
            delay(2000); 
     
            /* ####################################
             * EXAMPLE 3 - METHOD 1 
             * #################################### */
            /* ------------------------------- */
            /* get digital input as input mask */
            /* ------------------------------- */

            /* !!! EXECUTE the operation to get the digital input */
            err = expansions[i]->execute(GET_DIGITAL_INPUT);
            //Serial.println("(2) err = " + String(err));

           unsigned int digital_in = 0;

           /* READ THE STATE of digital input as a single bit masked integer */
           expansions[i]->read(ADD_DIGITAL_INPUT,digital_in);
           
           /* print the status of the digital input */
           for(int k = 0; k < DIGITAL_IN_NUM; k++) {
             if(digital_in & (1 << k)) {
               Serial.print("HH");
             }
             else {
               Serial.print("LL");
             }
             Serial.print(' ');
          }
          Serial.println();
          
          /* ####################################
           * EXAMPLE 3 - METHOD 2
           * #################################### */
          /* ------------------------------------ */
          /* get digital input as single register */
          /* ------------------------------------ */
          
          /* !!! Execute get digital input to refresh the status (not
           * strictly required here because we have just read the input
           * just few lines above)*/
          err = expansions[i]->execute(GET_DIGITAL_INPUT);
          /* DIGITAL input can also be accessed via individual registers
           * from ADD_DIGITAL_0_INPUT to ADD_DIGITAL_15_INPUT */
           /* READ THE STATE of digital input as a single bit masked integer */
          unsigned int status = 0;
          expansions[i]->read(ADD_DIGITAL_3_INPUT,status);
          Serial.println("Digital input 3 status " + String(status));
          expansions[i]->read(ADD_DIGITAL_8_INPUT,status);
          Serial.println("Digital input 8 status " + String(status));
	
		
          /* ####################################
           * EXAMPLE 4  
           * #################################### */

          /* -------------------------------- */
          /* Read single pin as analog input  */
          /* -------------------------------- */

          uint8_t pin = 5;
          unsigned int analog_input = 0;
          /* !!! write the PIN to read */
          expansions[i]->write(CTRL_ADD_EXPANSION_PIN, (unsigned int)pin);
          /* !!! EXECUTE the get single analog input operation */
          err =  expansions[i]->execute(GET_SINGLE_ANALOG_INPUT);
          //Serial.println("(4) err = " + String(err));
          /* !!! read the register corresponding to the 5 pin*/
          expansions[i]->read(ADD_PIN_5_ANALOG_IN,analog_input);
          Serial.println("SINGLE Analog pin 5 value (1): " + String(analog_input));
          /* same pin can be accessed with */
          expansions[i]->read(BASE_ADD_ANALOG_IN + pin,analog_input);
          Serial.println("SINGLE Analog pin 5 value (2): " + String(analog_input));

          /* ####################################
           * EXAMPLE 5  
           * #################################### */

          /* --------------------------------- */
          /* Read ALL input as analog at once  */
          /* --------------------------------- */
	   
          /* !!! execute the operation to get all the input as analog */
          err = expansions[i]->execute(GET_ALL_ANALOG_INPUT);
          //Serial.println("(5) err = " + String(err));
          Serial.println("READING all Analog pin values (2):");

          expansions[i]->read(ADD_PIN_0_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_1_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_2_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_3_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_4_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_5_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_6_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_7_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_8_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_9_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_10_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_11_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_12_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_13_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_14_ANALOG_IN,analog_input);
          printUint16(analog_input);
          expansions[i]->read(ADD_PIN_15_ANALOG_IN,analog_input);
          printUint16(analog_input);
          Serial.println();

          Serial.println("READING all Analog pin values (2):");
          /* the same can be achieved with */
          for(int k = 0; k < ANALOG_IN_NUM; k++) {
             expansions[i]->read(DIGITAL_EXPANSION_ADDRESS + BASE_ADD_ANALOG_IN
             + k,analog_input);
             printUint16(analog_input);
             
          }
          Serial.println();
        }
    }
  }


}

/* -------------------------------------------------------------------------- */
void printExpansionType(ExpansionType_t t) {
/* -------------------------------------------------------------------------- */  
  if(t == EXPANSION_NOT_VALID) {
    Serial.print("Unknown!");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_MEC) {
    Serial.print("Opta --- DIGITAL [Mechanical]  ---");
  }
  else if(t == EXPANSION_OPTA_DIGITAL_STS) {
    Serial.print("Opta --- DIGITAL [Solid State] ---");
  }
  else if(t == EXPANSION_DIGITAL_INVALID) {
    Serial.print("Opta --- DIGITAL [!!Invalid!!] ---");
  }
  else if(t == EXPANSION_OPTA_ANALOG) {
    Serial.print("Opta ~~~ ANALOG ~~~");
  }
  else {
    Serial.print("Unknown!");
  }
}

/* -------------------------------------------------------------------------- */
void printUint16(uint16_t v) {
/* -------------------------------------------------------------------------- */  
  if(v < 10) {
    Serial.print("    ");
  }
  else if(v < 100) {
    Serial.print("   ");

  }
  else if(v < 1000) {
    Serial.print("  ");

  }
  else if(v < 10000) {
     Serial.print(" ");
  }
  Serial.print(v);
  Serial.print(" ");
}
