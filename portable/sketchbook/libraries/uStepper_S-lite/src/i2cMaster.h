/********************************************************************************************
*       File:       i2cMaster.h                          		                            *
*		Version:    1.1.0                                           						*
*      	Date: 		June 14, 2020 	                                    					*
*      	Author: 	Thomas Hørring Olsen                                   					*
*                                                   										*
*********************************************************************************************
*                       i2cMaster class                   		                            *
*                                                                                           *
*   This file contains the implementation of the class methods, used to communicate over 	*
*	the I2C bus.																			*
*                                                                                           *
*********************************************************************************************
*   (C) 2020                                                                                *
*                                                                                           *
*   uStepper ApS                                                                            *
*   www.ustepper.com                                                                        *
*   administration@ustepper.com                                                             *
*                                                                                           *
*   The code contained in this file is released under the following open source license:    *
*                                                                                           *
*           Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International         *
*                                                                                           *
*   The code in this file is provided without warranty of any kind - use at own risk!       *
*   neither uStepper ApS nor the author, can be held responsible for any damage             *
*   caused by the use of the code contained in this file !                                  *
*                                                                                           *
********************************************************************************************/
/** @file i2cMaster.h
 * @brief      	This file contains the implementation of the class methods, used to
 * 				communicate over the I2C bus.
 *
 * @author     Thomas Hørring Olsen (thomas@ustepper.com)
 */
#ifndef _I2CMASTER_H_
#define _I2CMASTER_H_

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>

/** I2C bus is not currently in use */
#define I2CFREE 0						

/** Value for RW bit in address field, to request a read */
#define READ  1							

/** Value for RW bit in address field, to request a write */
#define WRITE 0							

/** start condition transmitted */
#define START  0x08						

/** repeated start condition transmitted */
#define REPSTART 0x10					

/** slave address plus write bit transmitted, ACK received */
#define TXADDRACK  0x18					

/** data transmitted, ACK received */
#define TXDATAACK 0x28					

/** slave address plus read bit transmitted, ACK received */
#define RXADDRACK 0x40				

/** value to indicate ACK for i2c transmission */
#define ACK 1							

/** value to indicate NACK for i2c transmission */
#define NACK 0							

/**
 * @brief      Prototype of class for accessing the TWI (I2C) interface of the
 *             AVR (master mode only).
 *
 *             This class enables the use of the hardware TWI (I2C) interface in
 *             the AVR (master mode only), which is used for interfacing with
 *             the encoder. This class is needed in this library, as arduino's
 *             build in "wire" library uses interrupts to access the TWI
 *             interface, and since the uStepper library needs to use the TWI
 *             interface within a timer interrupt, this library cannot be used.
 *             As a result of this, the "wire" library, cannot be used in
 *             sketches using the uStepper library, as this will screw with the
 *             setups, and make the sketch hang in the timer interrupt routine.
 *             Instead, if the programmer of the sketch needs to interface with
 *             external I2C devices, this class should be used. This library
 *             contains a predefined object called "I2C", which should be used
 *             for these purposes.
 *
 *             The functions "read()" and "write()", should be the only
 *             functions needed by most users of this library !
 */
class i2cMaster
{
	private:
		/** Contains the status of the I2C bus */
		uint8_t status;			


		/**
		 * @brief      Sends commands over the I2C bus.
		 *
		 *             This function is used to send different commands over the
		 *             I2C bus.
		 *
		 * @param      cmd   - Command to be send over the I2C bus.
		 */
		bool cmd(uint8_t cmd);
		volatile uint8_t twsr;
		volatile uint8_t twbr;
		volatile uint8_t twdr;
		volatile uint8_t twcr;
		
	public:

		void* operator new(size_t size);

		/**
		 * @brief      Constructor
		 *
		 *             This is the constructor, used to instantiate an I2C
		 *             object. Under normal circumstances, this should not be
		 *             needed by the programmer of the arduino sketch, since
		 *             this library already has a global object instantiation of
		 *             this class, called "I2C".
		 */
		i2cMaster(bool channel);

		/**
		 * @brief      Constructor
		 *
		 *             This is the constructor, used to instantiate an I2C
		 *             object. Under normal circumstances, this should not be
		 *             needed by the programmer of the arduino sketch, since
		 *             this library already has a global object instantiation of
		 *             this class, called "I2C".
		 */
		i2cMaster(void);
		
		/**
		 * @brief      Reads a byte from the I2C bus.
		 *
		 *             This function requests a byte from the device addressed
		 *             during the I2C transaction setup. The parameter "ack" is
		 *             used to determine whether the device should keep sending
		 *             data or not after the reception of the currently
		 *             requested data byte.
		 *
		 * @param      ack   - should be set to "ACK" if more bytes is wanted,
		 *                   and "NACK" if no more bytes should be send (without
		 *                   the quotes)
		 * @param      data  - Address of the variable to store the requested
		 *                   data byte
		 *
		 * @return     Always returns 1
		 */
		bool readByte(bool ack, uint8_t *data);

		/**
		 * @brief      sets up I2C connection to device, reads a number of data
		 *             bytes and closes the connection
		 *
		 *             This function is used to perform a read transaction
		 *             between the arduino and an I2C device. This function will
		 *             perform everything from setting up the connection,
		 *             reading the desired number of bytes and tear down the
		 *             connection.
		 *
		 * @param      slaveAddr   -	7 bit address of the device to read from
		 * @param      regAddr     -	8 bit address of the register to read from
		 * @param      numOfBytes  -	Number of bytes to read from the device
		 * @param      data        -	Address of the array/string to store the
		 *                         bytes read. Make sure enough space are
		 *                         allocated before calling this function !
		 *
		 * @return     1			-	Currently always returns this value. In the future
		 *             this value will be used to indicate successful
		 *             transactions.
		 */		
		bool read(uint8_t slaveAddr, uint8_t regAddr, uint8_t numOfBytes, uint8_t *data);

		/**
		 * @brief      sets up connection between arduino and I2C device.
		 *
		 *             This function sets up the connection between the arduino
		 *             and the I2C device desired to communicate with, by
		 *             sending a start condition on the I2C bus, followed by the
		 *             device address and a read/write bit.
		 *
		 * @param      addr  -	Address of the device it is desired to
		 *                   communicate with
		 * @param      RW    -	Can be set to "READ" to setup a read transaction
		 *                   or "WRITE" for a write transaction (without the
		 *                   quotes)
		 *
		 * @return     1		-	Connection properly set up
		 * @return     0		-	Connection failed
		 */
		bool start(uint8_t addr, bool RW);
		
		/**
		 * @brief      Restarts connection between arduino and I2C device.
		 *
		 *             This function restarts the connection between the arduino
		 *             and the I2C device desired to communicate with, by
		 *             sending a start condition on the I2C bus, followed by the
		 *             device address and a read/write bit.
		 *
		 * @param      addr  -	Address of the device it is desired to
		 *                   communicate with
		 * @param      RW    -	Can be set to "READ" to setup a read transaction
		 *                   or "WRITE" for a write transaction (without the
		 *                   quotes)
		 *
		 * @return     1		-	Connection properly set up
		 * @return     0		-	Connection failed
		 */
		bool restart(uint8_t addr, bool RW);
		
		/**
		 * @brief      Writes a byte to a device on the I2C bus.
		 *
		 *             This function writes a byte to a device on the I2C bus.
		 *
		 * @param      data  - Byte to be written
		 *
		 * @return     1	- Byte written successfully
		 * @return     0	- transaction failed
		 */
		bool writeByte(uint8_t data);
		
		/**
		 * @brief      sets up I2C connection to device, writes a number of data
		 *             bytes and closes the connection
		 *
		 *             This function is used to perform a write transaction
		 *             between the arduino and an I2C device. This function will
		 *             perform everything from setting up the connection,
		 *             writing the desired number of bytes and tear down the
		 *             connection.
		 *
		 * @param      slaveAddr   -	7 bit address of the device to write to
		 * @param      regAddr     -	8 bit address of the register to write to
		 * @param      numOfBytes  -	Number of bytes to write to the device
		 * @param      data        -	Address of the array/string containing data
		 *                         to write.
		 *
		 * @return     1			-	Currently always returns this value. In the future
		 *             this value will be used to indicate successful
		 *             transactions.
		 */		
		bool write(uint8_t slaveAddr, uint8_t regAddr, uint8_t numOfBytes, uint8_t *data);

		/**
		 * @brief      Closes the I2C connection
		 *
		 *             This function is used to close down the I2C connection,
		 *             by sending a stop condition on the I2C bus.
		 *
		 * @return     1	-	This is always returned Currently - in a later
		 *             version, this should indicate that connection is
		 *             successfully closed
		 */
		bool stop(void);
		
		/**
		 * @brief      Get current I2C status
		 *
		 *             This function returns the status of the I2C bus.
		 *
		 * @return     Status of the I2C bus. Refer to defines for possible
		 *             status
		 */
		uint8_t getStatus(void);
		
		/**
		 * @brief      Setup TWI (I2C) interface
		 *
		 *             This function sets up the TWI interface, and is
		 *             automatically called in the instantiation of the uStepper
		 *             encoder object.
		 */
		void begin(void);

		/**
		 * @brief      Setup TWI (I2C) interface
		 *
		 *             This function sets up the TWI interface, and is
		 *             automatically called in the instantiation of the uStepper
		 *             encoder object.
		 */
		void begin(bool channel);
};

#endif