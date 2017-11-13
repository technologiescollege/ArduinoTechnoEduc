// demo of Grove - Led Bar
// this demo will show you how to use void ledIndexBit(unsigned int index_bits) of this library
// set led single bit, a bit contrl a led
// such as, index_bits = 0x05, then led 0  and led 3 will on, the others will off

#include <LED_Bar.h>

LED_Bar bar(9, 8);                  // config Io here, (clk, dio)

void setup()
{
    // the set_LED_Index set the indivial LED state.
    // for example:the 0b000001101010101, the "1101010101" means:
    // LED10------ON;
    // LED9------ON;
    // LED8------OFF;
    // LED7------ON;
    // LED6------OFF; 
    // LED5------ON;
    // LED4------0FF;
    // LED3------ON;
    // LED2------OFF;
    // LED1------ON;
    
    bar.ledIndexBit(0b000001101010101);
}

void loop()
{

}
