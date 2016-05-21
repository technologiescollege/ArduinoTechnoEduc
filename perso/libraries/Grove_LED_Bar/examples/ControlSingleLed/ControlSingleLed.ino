// demo of Grove - Led Bar
// this demo will show you how to use setSingleLed(int num, int st) of this library
// there are 10 leds in Grove - Led Bar
// num - which led 
// st  - state: 0 off 1 on

#include <LED_Bar.h>

LED_Bar bar(6, 5);                          // config Io here, (clk, dio)

void setup()
{
    // nothing to initialize
}

void loop()
{
    for(int i=0; i<=10; i++)
    {
        if(0 == i)
        {
            bar.setSingleLed(0, 1);         // led0 on
            bar.setSingleLed(10, 0);        // led10 off
        }
        else
        {
            bar.setSingleLed(i, 1);         // ledi on
            bar.setSingleLed(i-1, 0);       // ledi-1 off
        }
        
        delay(100);
    }
}
