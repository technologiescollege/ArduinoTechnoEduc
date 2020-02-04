/********************************************************************************************
*       File:       i2cMaster.cpp                           		                            *
*       Version:    1.0.0                                                                   *
*       Date:       April 29th, 2019                                                         *
*       Author:     Thomas Hørring Olsen                                                    *
*                                                                                           *   
*********************************************************************************************
*                       i2cMaster class                   		                            *
*                                                                                           *
*   This file contains the implementation of the class methods, used to communicate over 	*
*	the I2C bus.																			*
*                                                                                           *
*********************************************************************************************
*   (C) 2019                                                                                *
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
/** @file i2cMaster.cpp
 * @brief      	This file contains the implementation of the class methods, used to
 * 				communicate over the I2C bus.
 *
 * @author     Thomas Hørring Olsen (thomas@ustepper.com)
 */

#include "i2cMaster.h"
#include "Arduino.h"
#include <util/delay.h>
bool i2cMaster::cmd(uint8_t cmd)
{
	uint16_t i = 0;
	// send command
	_SFR_MEM8(this->twcr) = cmd;
	// wait for command to complete
	while (!(_SFR_MEM8(this->twcr) & (1 << TWINT1)))
	{
		i++;
		if(i == 3000)
		{
			return false;
		}
		_delay_us(1);
	}
	
	// save status bits
	status = _SFR_MEM8(this->twsr) & 0xF8;	

	return true;
}

bool i2cMaster::read(uint8_t slaveAddr, uint8_t regAddr, uint8_t numOfBytes, uint8_t *data)
{
	uint8_t i;

	if(this->start(slaveAddr, WRITE) == false)
	{
		this->stop();
		return false;
	}

	if(this->writeByte(regAddr) == false)
	{
		this->stop();
		return false;
	}

	if(this->restart(slaveAddr, READ) == false)
	{
		this->stop();
		return false;
	}

	for(i = 0; i < (numOfBytes - 1); i++)
	{
		if(this->readByte(ACK, &data[i]) == false)
		{
			this->stop();
			return false;
		}	
	}

	if(this->readByte(NACK, &data[numOfBytes-1]) == false)
	{
		this->stop();
		return false;
	}

	this->stop();
	
	return 1; 
}

bool i2cMaster::write(uint8_t slaveAddr, uint8_t regAddr, uint8_t numOfBytes, uint8_t *data)
{
	uint8_t i = 0;	
	if(this->start(slaveAddr, WRITE) == false)
	{
		this->stop();
		return false;
	}
	if(this->writeByte(regAddr) == false)
	{
		this->stop();
		return false;
	}
	for(i = 0; i < numOfBytes; i++)
	{
		if(this->writeByte(*(data + i)) == false)
		{
			this->stop();
			return false;
		}
	}
	this->stop();
	return 1;
}

bool i2cMaster::readByte(bool ack, uint8_t *data)
{
	if(ack)
	{
		if(this->cmd((1 << TWINT1) | (1 << TWEN1) | (1 << TWEA1)) == false)
		{
			return false;
		}

	}
	
	else
	{
		if(this->cmd((1 << TWINT1) | (1 << TWEN1)) == false)
		{
			return false;
		}
	}

	*data = _SFR_MEM8(this->twdr);

	return true;
}

bool i2cMaster::start(uint8_t addr, bool RW)
{
	// send START condition
	this->cmd((1<<TWINT1) | (1<<TWSTA1) | (1<<TWEN1) | (1 << TWEA1));

	if (this->getStatus() != START && this->getStatus() != REPSTART) 
	{
		return false;
	}

	// send device address and direction
	_SFR_MEM8(this->twdr) = (addr << 1) | RW;
	this->cmd((1 << TWINT1) | (1 << TWEN1) | (1 << TWEA1));
	
	if (RW == READ) 
	{

		return this->getStatus() == RXADDRACK;
	} 

	else 
	{
		return this->getStatus() == TXADDRACK;
	}
}

bool i2cMaster::restart(uint8_t addr, bool RW)
{
	return this->start(addr, RW);
}

bool i2cMaster::writeByte(uint8_t data)
{
	_SFR_MEM8(this->twdr) = data;

	this->cmd((1 << TWINT1) | (1 << TWEN1) | (1 << TWEA1));

	return this->getStatus() == TXDATAACK;
}

bool i2cMaster::stop(void)
{
	uint16_t i = 0;
	//	issue stop condition
	_SFR_MEM8(this->twcr) = (1 << TWINT1) | (1 << TWEN1) | (1 << TWSTO1);


	// wait until stop condition is executed and bus released
	while (_SFR_MEM8(this->twcr) & (1 << TWSTO1))
	{
		i++;
		if(i == 1000)
		{
			return false;
		}
		_delay_us(1);
	}

	status = I2CFREE;

	return 1;
}


uint8_t i2cMaster::getStatus(void)
{
	return status;
}

void i2cMaster::begin(void)
{
	// set bit rate register to 12 to obtain 400kHz scl frequency (in combination with no prescaling!)
	_SFR_MEM8(this->twbr) = 1;
	// no prescaler
	_SFR_MEM8(this->twsr) &= 0xFC;
}

void i2cMaster::begin(bool channel)
{
	if(channel)
	{
		this->twsr = 0xD9;
		this->twbr = 0xD8;
		this->twdr = 0xDB;
		this->twcr = 0xDC;	
	}
	else
	{
		this->twsr = 0xB9;
		this->twbr = 0xB8;
		this->twdr = 0xBB;
		this->twcr = 0xBC;	
	}
	// set bit rate register to 12 to obtain 400kHz scl frequency (in combination with no prescaling!)
	_SFR_MEM8(this->twbr) = 1;
	
}

void* i2cMaster::operator new(size_t size)
{
	void *object = malloc(size);
	return object;
}

i2cMaster::i2cMaster(bool channel)
{
	if(channel)
	{
		this->twsr = 0xD9;
		this->twbr = 0xD8;
		this->twdr = 0xDB;
		this->twcr = 0xDC;	
	}
	else
	{
		this->twsr = 0xB9;
		this->twbr = 0xB8;
		this->twdr = 0xBB;
		this->twcr = 0xBC;	
	}
}

i2cMaster::i2cMaster(void)
{
	
}