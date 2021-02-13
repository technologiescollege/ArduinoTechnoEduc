/*
    Seeed_vl53l0x.h
    Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR

    Copyright (c) 2018 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : downey
    Create Time: April 2018
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef _SEEED_VL53L0X_H
#define _SEEED_VL53L0X_H

#include <Arduino.h>
#include <Wire.h>

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "required_version.h"

#define DEBUG_EN
#define IIC_DEFAULT_ADDR  0x29
#define CONTINUOUS_MODE_MESURE_TIME  5



#ifndef SEEED_DN_DEFINES
#define SEEED_DN_DEFINES

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL_DB SerialUSB
#else
    #define SERIAL_DB Serial
#endif


typedef int            s32;
typedef long unsigned int   u32;
typedef short          s16;
typedef unsigned short u16;
typedef char           s8;
typedef unsigned char  u8;

typedef enum {
    NO_ERROR = 0,
    ERROR_PARAM = -1,
    ERROR_COMM = -2,
    ERROR_OTHERS = -128,
} err_t;


#define CHECK_RESULT(a,b)   do{if(a=b)  {    \
            SERIAL_DB.print(__FILE__);    \
            SERIAL_DB.print(__LINE__);   \
            SERIAL_DB.print(" error code =");  \
            SERIAL_DB.println(a);                   \
            return a;   \
        }}while(0)

#endif

class Seeed_vl53l0x {

  public:
    ~Seeed_vl53l0x() {}
    void print_pal_error(VL53L0X_Error Status);

    VL53L0X_Error VL53L0X_common_init();
    VL53L0X_Error VL53L0X_single_ranging_init();
    VL53L0X_Error VL53L0X_high_speed_ranging_init();
    VL53L0X_Error VL53L0X_high_accuracy_ranging_init();
    VL53L0X_Error VL53L0X_long_distance_ranging_init();
    VL53L0X_Error VL53L0X_continuous_ranging_init();

    VL53L0X_Error PerformSingleRangingMeasurement(VL53L0X_RangingMeasurementData_t* RangingMeasurementData);
    VL53L0X_Error PerformContinuousRangingMeasurement(VL53L0X_RangingMeasurementData_t* RangingMeasurementData);
  private:
    void VL53L0X_IIC_init(u8 IIC_ADDRESS = IIC_DEFAULT_ADDR);
    VL53L0X_Error check_version();
    VL53L0X_Error VL53L0X_set_limit_param();
    VL53L0X_Error VL53L0X_calibration_oprt();
    VL53L0X_Error VL53L0X_calibration_set();


    VL53L0X_Dev_t MyDevice;
    VL53L0X_Dev_t* pMyDevice = &MyDevice;
    VL53L0X_Version_t   Version;
    VL53L0X_Version_t*   pVersion   = &Version;
    //VL53L0X_DeviceInfo_t DeviceInfo;

};


#endif