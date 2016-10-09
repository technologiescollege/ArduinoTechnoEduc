#include "SerialALLBOT.h"

#include <Arduino.h>

SerialALLBOT::SerialALLBOT(int count)
	:ALLBOT(count)
{
	_callBackCount 	= 0;
}

SerialALLBOT::~SerialALLBOT()
{
	delete[] _callbacks;
}

void SerialALLBOT::update(void)
{
	checkAndHandleCommand();
	sendStatus();
}

void SerialALLBOT::registerCommand(String name, command_callback callback)
{
	_callbacks[_callBackCount].name = name;
	_callbacks[_callBackCount].cb  	= callback;
	_callBackCount++;
}

void SerialALLBOT::checkAndHandleCommand()
{	
	if (Serial.available())
	{
		String rawcommand = Serial.readString();
		
		int openingCharIndex 	= rawcommand.indexOf('<');
		int closeCharIndex 		= rawcommand.indexOf('>');

		if (openingCharIndex == -1 || closeCharIndex == -1)
		{
			rawcommand = "";
			return;
		}
		
		// Flush serial buffer
		while (Serial.available()) 
		{
			Serial.read();
		}
	
		// Remove anything before '<' and anything after '>'
		rawcommand.remove(0, openingCharIndex + 1);
		rawcommand.remove(closeCharIndex - 1);			
	
		// Extract command
		int space 		= rawcommand.indexOf(' '); 
		String command 	= rawcommand.substring(0, space);
			
		// Extract params
		int numParams = 0;
		int intargs[10];
		
		if (space != 1)
		{
			rawcommand.remove(0, space + 1);		
			space = rawcommand.indexOf(' ');			
			while (space != -1)
			{
				intargs[numParams] = rawcommand.substring(0, space).toInt();
				
				// Jump to next space
				rawcommand.remove(0, space + 1);
				space = rawcommand.indexOf(' ');
				numParams++;
			}

			intargs[numParams] = rawcommand.toInt();
		}

		// Loop through all registered commands
		for (int i = 0; i < _callBackCount; i++)
		{
			if (_callbacks[i].name == command)
			{
				_callbacks[i].cb(numParams, intargs);
				sendCommandComplete();
				break;
			}
		}
		
		rawcommand = "";
	}
}
	
void SerialALLBOT::sendCommandComplete()
{
	Serial.println("A-OK");
	Serial.flush();
}

void SerialALLBOT::sendStatus()
{ 
	// Digital ports
	for (int i = 0; i < 14; i++)
	{
		Serial.println("digitalport/" + String(i) + " " + String(int(getDigitalPortStatus(i))));
		Serial.flush();
	}
  
	// Analog ports
	for (int i = 0; i < 6; i++)
	{
		Serial.println("analogport/" + String(i) + " " + String(getAnalogPortStatus(i)));
		Serial.flush();
	}
}

bool SerialALLBOT::getDigitalPortStatus(int pin)
{
	// 0 - 7
	if (pin < 8) 
	{
		// Port configured as output?
		if (bitRead(DDRD, pin) > 0)
		{
			return bitRead(PORTD, pin);
		}
		
		else
		{
			return (digitalRead(pin) > 0);
		}
	}
  
	// 8 - 13
	else if (pin < 14)
	{
		
		if (bitRead(DDRB, pin >> 8) > 0)
		{
			return bitRead(PORTB, pin);
		}
		
		else
		{
			return (digitalRead(pin) > 0);
		}
  }
  
  return false;
}

int SerialALLBOT::getAnalogPortStatus(int pin)
{
	// 0 - 5
	if (pin < 6) 
	{
    /*
    // Port configured as output?
    if (bitRead(DDRC, pin) > 0)
    {
       return bitRead(PORTC, pin);
    }
    else
    {
       return analogRead(pin);
    }*/
    
		return analogRead(pin);
	}
  
	return 0; 	
}