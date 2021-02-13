# Seeed_Arduino_TFlidar  [![Build Status](https://travis-ci.com/Seeed-Studio/Seeed-Arduino-TFlidar.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Seeed-Arduino-TFlidar)

## Introduction

Seeed Arduino lidar is a seeed's library that aim is to drive all lidar of TF.this library having development for now.

## Usage

!!!Note
   you have to use the SetBaudRate.ino to set baud rate of TF lidar to 9600,if you want to use software serial at the 3.3v mcu.
this code has been tested on Seeeduino V4.2 by using TF luna.

**this code has been tested on Wio terminal by using TF luna and SoftwareSerial.**
**this code has been tested on Seeeduino V4.2 by using TF luna.**

```c++
#include "TFLidar.h"
#define USE_3V3_SOFT_SERIAL

#ifdef USE_3V3_SOFT_SERIAL
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
#ifdef USE_3V3_SOFT_SERIAL
  SeeedTFLidar.begin(&uart,9600);
#else
  SeeedTFLidar.begin(&uart,115200);
#endif
}

void loop() {
  while(!SeeedTFLidar.get_frame_data()){
  #ifdef USE_3V3_SOFT_SERIAL
      delay(20);
  #else
      delay(1);
  #endif
  }
  // put your main code here, to run repeatedly:
  SERIAL.print("dist = ");
  SERIAL.print(SeeedTFLidar.get_distance()); //output measure distance value of LiDAR
  SERIAL.println(" ");
}
```

## API Reference

- begin(SoftwareSerial *TFSerial,unsigned long baud_rate):void

```C++
SoftwareSerial uart(2, 3);
SeeedTFLidar.begin(&uart,115200);
```

- begin(HardwareSerial *TFSerial,unsigned long baud_rate):void

```C++
SeeedTFLidar.begin(&Serial1,115200);
```

- get_frame_data:bool

```C++
while(!SeeedTFLidar.get_frame_data()){
    delay(1);
}
```

- get_distance():uint16_t

```C++
int distance;
while(!SeeedTFLidar.get_frame_data()){
    delay(1);
}
distance = SeeedTFLidar.get_distance();
```

- get_strength():uint16_t

```C++
int distance;
while(!SeeedTFLidar.get_frame_data()){
    delay(1);
}
distance = SeeedTFLidar.get_strength();
```

----

This software is written by seeed studio<br>
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>


[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Grove_LED_Bar)](https://github.com/igrigorik/ga-beacon)