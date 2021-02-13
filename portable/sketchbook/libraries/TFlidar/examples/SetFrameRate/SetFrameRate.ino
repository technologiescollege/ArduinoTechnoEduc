#include "TFLidar.h"
//NOTE this demo cannot work with   TFmini for now
#define SERIAL Serial

#if defined(SEEED_XIAO_M0)
    #define uart  Serial1
#elif defined(SEEED_WIO_TERMINAL)
    SoftwareSerial uart(0, 1);
#else
    SoftwareSerial uart(2, 3);
#endif

TFLuna SeeedTFLuna;
TFLidar SeeedTFLidar(&SeeedTFLuna);

void setup() {
    // put your setup code here, to run once:
    SERIAL.begin(9600);
    while(!Serial);
    int buff[3] = {0};
    SeeedTFLidar.begin(&uart,9600);
    SeeedTFLidar.set_frame_rate(SAMPLERATE_100HZ);
    if(SeeedTFLidar.save_config()){
        SERIAL.println("config successful");
    }
    else{
        SERIAL.println("config failed");
    }

}
void loop() {
}