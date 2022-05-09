/*
  Simple5641AS.h - Library for simple use of the 5641AS LED 7-segment display
  Documentation and examples available at https://github.com/adrian200223/Simple5641AS
  Released into the public domain.
*/

#ifndef Simple5641_h
#define Simple5641_h

#include <Arduino.h>

const uint8_t segmentCodes[] = {
                     B11111100, 
                     B01100000,
                     B11011010,
                     B11110010,
                     B01100110,
                     B10110110,
                     B10111110,
                     B11100000,
                     B11111110,
                     B11110110 };

class Simple5641AS 
{
  public:
    Simple5641AS(const uint8_t * segmentPins, const uint8_t * digitSelectionPins);
    
    void selectAllDigits();
    void selectDigit(int digit);
    
    void clean();
    
    void shortDisplayNumber(int n);
    void shortDisplayNumber(int n, boolean dot);

    void shortDisplayCode(uint8_t code);

    void cycle(long digitDisplayTime, int number, int dot);
    void customDisplayCycle(long digitDisplayTime, uint8_t codes[4]);
    
    void countDown(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from, int to);
    void countDown(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from);
    void countDown(int from);
    void countDown(float from);
    void slowCountDown(int from);
    void slowCountDown(float from);
    
    void countUp(long digitDisplayTime, long cycles, long cycleDelay, int dot, int from, int to);
    void countUp(long digitDisplayTime, long cycles, long cycleDelay, int dot, int to);
    void countUp(int to);
    void countUp(float to);
    void slowCountUp(int to);
    void slowCountUp(float to);
    
    void zeroAnimation(long cycleDelay, long cycles);
    void zeroAnimation();
    
    void displayNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot);
    void displayNumber(int seconds, int number, int dot);
    void displayNumber(int seconds, int number);
    void displayNumber(int seconds, float number);

    void displayCode(long cycles, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot);
    void displayCode(int seconds, uint8_t codes[4], int dot);

    void visualExplanation();
    void dangerousVisualExplanation();

  private:
    int decimalPlaces(float number);
    int cleanNumber(int number);
    uint8_t _segmentPins[8];
    uint8_t _digitSelectionPins[4];
};

#endif
