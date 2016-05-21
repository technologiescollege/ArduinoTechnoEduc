// demo of Grove - Led Bar
// this demo will show you how to use setLevel function of this library
// first, you need to choos which io to used. 

#include <LED_Bar.h>

LED_Bar bar(9, 8);                  // config Io here, (clk, dio)

void setup()
{
    // nothing to initialize
}

void loop()
{
    for(int i=0; i<=10; i++)
    {
        bar.setLevel(i);
        delay(100);
    }
}
