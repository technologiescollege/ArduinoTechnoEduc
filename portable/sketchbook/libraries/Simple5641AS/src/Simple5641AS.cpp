/*
  Simple5641AS.cpp - Library for simple use of the 5641AS LED 7-segment display
  Documentation and examples available at https://github.com/adrian200223/Simple5641AS
  Released into the public domain.
*/

#include "Simple5641AS.h"

// Simple5641AS constructor
Simple5641AS::Simple5641AS(const uint8_t * segmentPins, const uint8_t * digitSelectionPins)
{
    memcpy(_segmentPins, segmentPins, sizeof(uint8_t) * 8);
    memcpy(_digitSelectionPins, digitSelectionPins, sizeof(uint8_t) * 4);

    for (int i = 0; i < sizeof(_segmentPins); i++) pinMode(_segmentPins[i], OUTPUT);
    for (int i = 0; i < sizeof(_digitSelectionPins); i++) pinMode(_digitSelectionPins[i], OUTPUT);

    for (int i = 0; i < sizeof(_segmentPins); i++) digitalWrite(_segmentPins[i], LOW);
    for (int i = 0; i < sizeof(_digitSelectionPins); i++) digitalWrite(_digitSelectionPins[i], LOW);
}

// Clean a integer number
int  Simple5641AS::cleanNumber(int number) {
    return number % 10000;
}

// Get number of decimals on a number. Max: 4
int Simple5641AS::decimalPlaces(float number)
{
    char buffer[12];
    dtostrf(number, 3, 4, buffer);

    int counter = strlen(buffer) - 1;
    while (buffer[counter] != '.' && buffer[counter] == '0' && counter != 0) {
        counter--;
    }

    int integerCounter = 0;
    while (buffer[integerCounter] != '.' && integerCounter + counter < strlen(buffer) + 1) {
        integerCounter++;
    }

    return counter - integerCounter;
}

// All pins LOW -> all pins selected
void Simple5641AS::selectAllDigits()
{
    digitalWrite(_digitSelectionPins[0], LOW);
    digitalWrite(_digitSelectionPins[1], LOW);
    digitalWrite(_digitSelectionPins[2], LOW);
    digitalWrite(_digitSelectionPins[3], LOW);
}

// 1 pin LOW, rest of pins HIGH -> 1 pin selected
void Simple5641AS::selectDigit(int dig)
{
    digitalWrite(_digitSelectionPins[0], HIGH);
    digitalWrite(_digitSelectionPins[1], HIGH);
    digitalWrite(_digitSelectionPins[2], HIGH);
    digitalWrite(_digitSelectionPins[3], HIGH);

    digitalWrite(_digitSelectionPins[dig], LOW); 
}

// all segments LOW
void Simple5641AS::clean()
{
    digitalWrite(_segmentPins[0], LOW);
    digitalWrite(_segmentPins[1], LOW);
    digitalWrite(_segmentPins[2], LOW);
    digitalWrite(_segmentPins[3], LOW);
    digitalWrite(_segmentPins[4], LOW);
    digitalWrite(_segmentPins[5], LOW);
    digitalWrite(_segmentPins[6], LOW);
    digitalWrite(_segmentPins[7], LOW);
}

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void Simple5641AS::shortDisplayNumber(int n)
{
    digitalWrite(_segmentPins[0], (segmentCodes[n] & ( 1 << 7 )) >> 7 ? HIGH : LOW);
    digitalWrite(_segmentPins[1], (segmentCodes[n] & ( 1 << 6 )) >> 6 ? HIGH : LOW);
    digitalWrite(_segmentPins[2], (segmentCodes[n] & ( 1 << 5 )) >> 5 ? HIGH : LOW);
    digitalWrite(_segmentPins[3], (segmentCodes[n] & ( 1 << 4 )) >> 4 ? HIGH : LOW);
    digitalWrite(_segmentPins[4], (segmentCodes[n] & ( 1 << 3 )) >> 3 ? HIGH : LOW);
    digitalWrite(_segmentPins[5], (segmentCodes[n] & ( 1 << 2 )) >> 2 ? HIGH : LOW);
    digitalWrite(_segmentPins[6], (segmentCodes[n] & ( 1 << 1 )) >> 1 ? HIGH : LOW);
    digitalWrite(_segmentPins[7], LOW);
}

// number -> segment code ->  digitalWrite(HIGH or LOW) for each pin, dot included
void Simple5641AS::shortDisplayNumber(int n, boolean dot)
{
    digitalWrite(_segmentPins[0], (segmentCodes[n] & ( 1 << 7 )) >> 7 ? HIGH : LOW);
    digitalWrite(_segmentPins[1], (segmentCodes[n] & ( 1 << 6 )) >> 6 ? HIGH : LOW);
    digitalWrite(_segmentPins[2], (segmentCodes[n] & ( 1 << 5 )) >> 5 ? HIGH : LOW);
    digitalWrite(_segmentPins[3], (segmentCodes[n] & ( 1 << 4 )) >> 4 ? HIGH : LOW);
    digitalWrite(_segmentPins[4], (segmentCodes[n] & ( 1 << 3 )) >> 3 ? HIGH : LOW);
    digitalWrite(_segmentPins[5], (segmentCodes[n] & ( 1 << 2 )) >> 2 ? HIGH : LOW);
    digitalWrite(_segmentPins[6], (segmentCodes[n] & ( 1 << 1 )) >> 1 ? HIGH : LOW);
    digitalWrite(_segmentPins[7], dot ? HIGH : LOW);
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot included
void Simple5641AS::shortDisplayCode(uint8_t code)
{
    digitalWrite(_segmentPins[0], (code & ( 1 << 7 )) >> 7 ? HIGH : LOW);
    digitalWrite(_segmentPins[1], (code & ( 1 << 6 )) >> 6 ? HIGH : LOW);
    digitalWrite(_segmentPins[2], (code & ( 1 << 5 )) >> 5 ? HIGH : LOW);
    digitalWrite(_segmentPins[3], (code & ( 1 << 4 )) >> 4 ? HIGH : LOW);
    digitalWrite(_segmentPins[4], (code & ( 1 << 3 )) >> 3 ? HIGH : LOW);
    digitalWrite(_segmentPins[5], (code & ( 1 << 2 )) >> 2 ? HIGH : LOW);
    digitalWrite(_segmentPins[6], (code & ( 1 << 1 )) >> 1 ? HIGH : LOW);
    digitalWrite(_segmentPins[7], (code & ( 1 << 0 )) >> 0 ? HIGH : LOW);
}

// cycle function: display one number
void Simple5641AS::cycle(long digitDisplayTime, int number, int dot)
{
    clean();
    selectDigit(0);
    shortDisplayNumber((number / 1000) % 10, dot == 0);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(1);
    shortDisplayNumber((number / 100) % 10, dot == 1);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(2);
    shortDisplayNumber((number / 10) % 10, dot == 2);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(3);
    shortDisplayNumber(number % 10, dot == 3);
    delayMicroseconds(digitDisplayTime);
    clean();
}

// customDisplayCycle function: display 4 symbols represented by 4 codes
void Simple5641AS::customDisplayCycle(long digitDisplayTime, uint8_t codes[4])
{
    clean();
    selectDigit(0);
    shortDisplayCode(codes[0]);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(1);
    shortDisplayCode(codes[1]);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(2);
    shortDisplayCode(codes[2]);
    delayMicroseconds(digitDisplayTime);
    clean();
    selectDigit(3);
    shortDisplayCode(codes[3]);
    delayMicroseconds(digitDisplayTime);
    clean();
}

// countDown function, with all parameters available
void Simple5641AS::countDown(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from, int to)
{
    from = cleanNumber(from);
    to = cleanNumber(to);

    int counter = from;
    long cycleIterator = 0L;
    while (counter >= to) {
        while (cycleIterator < cycles) {
            clean();
            selectDigit(0);
            shortDisplayNumber((counter / 1000) % 10, dot == 0);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(1);
            shortDisplayNumber((counter / 100) % 10, dot == 1);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(2);
            shortDisplayNumber((counter / 10) % 10, dot == 2);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(3);
            shortDisplayNumber(counter % 10, dot == 3);
            delayMicroseconds(digitDisplayTime);
            clean();
            
            delayMicroseconds(cycleDelay);
            cycleIterator++;
        }
        cycleIterator = 0L;
        counter--;
    }
}

// countDown function, starting from 0
void Simple5641AS::countDown(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from)
{
    countDown(digitDisplayTime, cycles, cycleDelay, dot, from, 0);
}

// countDown function, with default parameters. 1 decimal visible
void Simple5641AS::countDown(int from)
{
    countDown(155, 100, 0, 2, from * 10.f);
}

// countDown function, with default parameters. 1 decimal visible
void Simple5641AS::countDown(float from)
{
    countDown(155, 100, 0, 2, (int)(from * 10.f));
}

// slowCountDown function, with default parameters. 0 decimals visible
void Simple5641AS::slowCountDown(int from)
{
    countDown(2375, 100, 0, 5, from);
}

// slowCountDown function, with default parameters. 0 decimals visible
void Simple5641AS::slowCountDown(float from)
{
    countDown(2375, 100, 0, 5, (int)from);
}

// countUp function, with all parameters available
void Simple5641AS::countUp(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from, int to)
{
    from = cleanNumber(from);
    to = cleanNumber(to);

    int counter = from;
    long cycleIterator = 0L;
    while (counter < to) {
        while (cycleIterator < cycles) {
            clean();
            selectDigit(0);
            shortDisplayNumber((counter / 1000) % 10, dot == 0);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(1);
            shortDisplayNumber((counter / 100) % 10, dot == 1);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(2);
            shortDisplayNumber((counter / 10) % 10, dot == 2);
            delayMicroseconds(digitDisplayTime);
            clean();
            selectDigit(3);
            shortDisplayNumber(counter % 10, dot == 3);
            delayMicroseconds(digitDisplayTime);
            clean();
            
            delayMicroseconds(cycleDelay);
            cycleIterator++;
        }
        cycleIterator = 0L;
        counter++;
    }
}

// countUp funciton. Starting from 0
void Simple5641AS::countUp(long digitDisplayTime, long cycles, long cycleDelay, int dot, int to)
{
    countUp(digitDisplayTime, cycles, cycleDelay, dot, 0, to);
}

// countUp function, with defeault parameters. 1 decimal visible
void Simple5641AS::countUp(int to)
{
    countUp(155, 100, 0, 2, to * 10.f);
}

// countUp function, with defeault parameters. 1 decimal visible
void Simple5641AS::countUp(float to)
{
    countUp(155, 100, 0, 2, int(to * 10.f));
}

// slowCountUp function, with default parameters. 0 decimals visible
void Simple5641AS::slowCountUp(int to)
{
    countUp(2375, 100, 0, 5, to);
}

// slowCountUp function, with default parameters. 0 decimals visible
void Simple5641AS::slowCountUp(float to)
{
    countUp(2375, 100, 0, 5, (int)to);
}

// zeroAnimation function, with all parameters available
void Simple5641AS::zeroAnimation(long cycleDelay, long cycles)
{
    int counter = 8;
    long cycleIterator = 0L;
    while (counter > 0) {
        while (cycleIterator < cycles) {
            clean();
            selectAllDigits();
            cycleIterator % 4 == 0 && counter % 2 == 0 ? shortDisplayNumber(0, false) : clean();
            
            delayMicroseconds(cycleDelay);
            cycleIterator++;
        }
        cycleIterator = 0;
        counter--;
    }
}

// zeroAnimation function, with default parameters
void Simple5641AS::zeroAnimation()
{
    zeroAnimation(155, 100 * 16);
}

// displayNumber function, with all parameters available
void Simple5641AS::displayNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot)
{
    number = cleanNumber(number);

    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        selectDigit(0);
        shortDisplayNumber((number / 1000) % 10, dot == 0);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(1);
        shortDisplayNumber((number / 100) % 10, dot == 1);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(2);
        shortDisplayNumber((number / 10) % 10, dot == 2);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(3);
        shortDisplayNumber(number % 10, dot == 3);
        delayMicroseconds(digitDisplayTime);
        clean();
            
        delayMicroseconds(cycleDelay);
        cycleIterator++;
    }
}

// display a number for a number of seconds, and indicate where is the dot placed
void Simple5641AS::displayNumber(int seconds, int number, int dot)
{
    displayNumber(seconds * 690L, 400, 155, number, dot);
}

// display a integer number for a number of seconds, without any dots visible
void Simple5641AS::displayNumber(int seconds, int number)
{
    displayNumber(seconds * 690L, 400, 155, number, 5);
}

// display a float number, the dot is put accordingly
void Simple5641AS::displayNumber(int seconds, float number)
{
    number = (float)((long)(number * 1000.0)) / 1000.0;
    int decimals = decimalPlaces(number);
    float modifiedNumber = number;
    for (int i = 0; i < decimals; i++) modifiedNumber *= 10;
    displayNumber(seconds * 690L, 400, 155, (int)modifiedNumber, decimals == 0 ? 5 : 3 - decimals);
}

// displayCode function, with all parameters available
void Simple5641AS::displayCode(long cycles, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot)
{
    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        selectDigit(0);
        shortDisplayCode(codes[0]);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(1);
        shortDisplayCode(codes[1]);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(2);
        shortDisplayCode(codes[2]);
        delayMicroseconds(digitDisplayTime);
        clean();
        selectDigit(3);
        shortDisplayCode(codes[3]);
        delayMicroseconds(digitDisplayTime);
        clean();
        cycleIterator++;
    }
}

// displayCode function, with 3 parameters available
void Simple5641AS::displayCode(int seconds, uint8_t codes[4], int dot)
{
    displayCode(seconds * 690L, 400, 155, codes, dot);
}

// visualExplanation: a custom function to show how the numbers are displayed
void Simple5641AS::visualExplanation()
{
    int digitDisplayTime = 500;
    int cycleDelay = 100;
    int cycles = 2;
    int dot = 2;

    int counter = 05;
    long cycleIterator = 0L;
    while (counter >= 0) {
        while (cycleIterator < cycles) {
            clean();
            selectDigit(0);
            shortDisplayNumber((counter / 1000) % 10, dot == 0);
            delay(digitDisplayTime);
            clean();
            selectDigit(1);
            shortDisplayNumber((counter / 100) % 10, dot == 1);
            delay(digitDisplayTime);
            clean();
            selectDigit(2);
            shortDisplayNumber((counter / 10) % 10, dot == 2);
            delay(digitDisplayTime);
            clean();
            selectDigit(3);
            shortDisplayNumber(counter % 10, dot == 3);
            delay(digitDisplayTime);
            clean();
            
            delay(cycleDelay);
            digitDisplayTime *= 0.9;
            cycleDelay *= 0.9;
            cycleIterator++;
        }
        cycleIterator = 0L;
        counter--;
    }
}

// dangerousVisualExplanation: an increasingly quicker loop to show the number display.
// Not appropiate to photosensitive audiences
void Simple5641AS::dangerousVisualExplanation()
{
    int digitDisplayTime = 500;
    int cycleDelay = 100;
    int cycles = 2;
    int dot = 2;

    int counter = 100;
    long cycleIterator = 0L;
    while (counter >= 0) {
        while (cycleIterator < cycles) {
            clean();
            selectDigit(0);
            shortDisplayNumber((counter / 1000) % 10, dot == 0);
            delay(digitDisplayTime);
            clean();
            selectDigit(1);
            shortDisplayNumber((counter / 100) % 10, dot == 1);
            delay(digitDisplayTime);
            clean();
            selectDigit(2);
            shortDisplayNumber((counter / 10) % 10, dot == 2);
            delay(digitDisplayTime);
            clean();
            selectDigit(3);
            shortDisplayNumber(counter % 10, dot == 3);
            delay(digitDisplayTime);
            clean();
            
            delay(cycleDelay);
            cycleIterator++;
        }
        digitDisplayTime > 2 ? digitDisplayTime *= 0.8 : digitDisplayTime = 2;
        cycleDelay > 10 ? cycleIterator *= 0.8 : cycleIterator = 10;
        if (digitDisplayTime < 200) cycles < 100 ? cycles *= 1.1 : cycles = 100;
        cycleIterator = 0L;
        counter--;
    }
}
