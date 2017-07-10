/*
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Modified Time: June 2015
 * Description: This demo can measure the color chromaticity of ambient light or the color of objects,
 * 				and via Chainable RGB LED Grove displaying the detected color.
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Wire.h>
#include <GroveColorSensor.h>
#include <ChainableLED.h>

#define CLK_PIN   7
#define DATA_PIN  8
#define NUM_LEDS  1			//The number of Chainable RGB LED

ChainableLED leds(CLK_PIN, DATA_PIN, NUM_LEDS);

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
    int red, green, blue;
	GroveColorSensor colorSensor;
	colorSensor.ledStatus = 1;							// When turn on the color sensor LED, ledStatus = 1; When turn off the color sensor LED, ledStatus = 0.
	while(1)
	{
		colorSensor.readRGB(&red, &green, &blue);		//Read RGB values to variables.
		delay(300);
		Serial.print("The RGB value are: RGB( ");
		Serial.print(red,DEC);
		Serial.print(", ");
		Serial.print(green,DEC);
		Serial.print(", ");
		Serial.print(blue,DEC);
	    Serial.println(" )");
		colorSensor.clearInterrupt();
		for(int i = 0; i<NUM_LEDS; i++)
		{
			leds.setColorRGB(i, red, green, blue);
		}
	}
}
