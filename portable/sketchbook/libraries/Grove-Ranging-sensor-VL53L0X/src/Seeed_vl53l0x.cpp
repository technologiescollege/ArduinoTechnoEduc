/*
    Seeed_vl53l0x.cpp
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

#include "Seeed_vl53l0x.h"






/** @brief Print corresponding string of error code.
    @param error code.
 * */
void Seeed_vl53l0x::print_pal_error(VL53L0X_Error Status) {
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    // Serial.print("API Status:");
    // Serial.print(Status);
    // Serial.print("API error string:");
    // Serial.println(buf);
}

/** @brief Print ranging status through param:pRangingMeasurementData
    @param The struct contains ranging status.
 * */
void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData) {
    char buf[VL53L0X_MAX_STRING_LENGTH];
    uint8_t RangeStatus;

    /*
        New Range Status: data is valid when pRangingMeasurementData->RangeStatus = 0
    */
    RangeStatus = pRangingMeasurementData->RangeStatus;

    VL53L0X_GetRangeStatusString(RangeStatus, buf);
    // Serial.print("Range Status:");
    // Serial.print(RangeStatus);
    // Serial.println(buf);
}


/** @brief IIC init
    @param IIC address.default address is 0x29.
 * */
void Seeed_vl53l0x::VL53L0X_IIC_init(u8 IIC_ADDRESS) {
    Wire.begin();
    pMyDevice->I2cDevAddr      = IIC_ADDRESS;
    pMyDevice->comms_type      =  1;
    pMyDevice->comms_speed_khz =  400;
}

/** @brief The module need to calibrated after power-on.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_calibration_oprt() {
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    //Serial.println("Call of VL53L0X_PerformRefSpadManagement\n");
    Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
             &refSpadCount, &isApertureSpads); // Device Initialization
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    //Serial.println("Call of VL53L0X_PerformRefCalibration\n");
    Status = VL53L0X_PerformRefCalibration(pMyDevice,
                                           &VhvSettings, &PhaseCal); // Device Initialization
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief Called when module need to calibrated. retain.

 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_calibration_set() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
}

/** @brief Set limitation factor.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_set_limit_param() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD,
                                        (FixPoint1616_t)(1.5 * 0.023 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief As function name.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::check_version() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;

    status_int = VL53L0X_GetVersion(pVersion);
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        return Status;
    }

    if (pVersion->major != VERSION_REQUIRED_MAJOR ||
            pVersion->minor != VERSION_REQUIRED_MINOR ||
            pVersion->build != VERSION_REQUIRED_BUILD) {
        // Serial.println("VL53L0X API Version Error");
    }
    return Status;
}

/** @brief The common init part with different mode.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_common_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    FixPoint1616_t LimitCheckCurrent;

    VL53L0X_IIC_init();

    Status = check_version();
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    //Serial.println("Call of VL53L0X_DataInit\n");
    Status = VL53L0X_DataInit(&MyDevice);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    //Serial.println("Call of VL53L0X_StaticInit\n");
    Status = VL53L0X_StaticInit(pMyDevice);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    //Serial.println("Call of VL53L0X_calibration\n");
    Status = VL53L0X_calibration_oprt();
    //print_pal_error(Status);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    //Serial.println("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    //Serial.println("Call of VL53L0X_set_limit_param\n");
    Status = VL53L0X_set_limit_param();
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief The single ranging mode init.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_single_ranging_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    //Serial.println("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    //Serial.println("Call of VL53L0X_set_limit_param\n");
    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD,
                                        (FixPoint1616_t)(1.5 * 0.023 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief The single&high speed ranging ranging mode init.Reduce excute time.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_high_speed_ranging_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    //Serial.println("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                                        (FixPoint1616_t)(0.25 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                                        (FixPoint1616_t)(32 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
             30000);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}


/** @brief The single&high accuracy ranging ranging mode init.It will cost much more time

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_high_accuracy_ranging_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    //Serial.println("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }


    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                                        (FixPoint1616_t)(0.25 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                                        (FixPoint1616_t)(18 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
             200000);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief The single&long distance ranging ranging mode init.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_long_distance_ranging_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    //Serial.println("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
                                         VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                                        (FixPoint1616_t)(0.1 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetLimitCheckValue(pMyDevice,
                                        VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                                        (FixPoint1616_t)(60 * 65536));
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
             33000);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetVcselPulsePeriod(pMyDevice,
                                         VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    Status = VL53L0X_SetVcselPulsePeriod(pMyDevice,
                                         VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    return Status;
}

/** @brief The continuous ranging ranging mode init.Excute without interval.

    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::VL53L0X_continuous_ranging_init() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }
    return Status;
}

/** @brief start mesurement and get result.
    @param The result.
    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::PerformContinuousRangingMeasurement(VL53L0X_RangingMeasurementData_t*
        RangingMeasurementData) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    u8 stat = 0;
    u32 stop_stat = 0;
    Status = VL53L0X_StartMeasurement(pMyDevice);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }

    // while(1)
    // {
    while (1) {
        Status = VL53L0X_GetMeasurementDataReady(pMyDevice, &stat);
        if (VL53L0X_ERROR_NONE != Status) {
            return Status;
        }
        if (1 == stat) {
            break;
        }
    }

    Status = VL53L0X_GetRangingMeasurementData(pMyDevice, RangingMeasurementData);
    if (VL53L0X_ERROR_NONE != Status) {
        return Status;
    }
    if (RangingMeasurementData->RangeMilliMeter >= 2000) {
        // SerialUSB.println("out of ranger");
    } else {
        // SerialUSB.print("distance::");
        // SerialUSB.println(RangingMeasurementData->RangeMilliMeter);
    }

    VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    VL53L0X_PollingDelay(pMyDevice);
    // }

    /***************************************************stop_part************************/
    // Status = VL53L0X_StopMeasurement(pMyDevice);
    // if(VL53L0X_ERROR_NONE!=Status) return Status;

    // while(1)
    // {
    //     Status = VL53L0X_GetStopCompletedStatus(pMyDevice,(uint32_t*)&stop_stat);
    //     if(VL53L0X_ERROR_NONE!=Status) return Status;
    //     if(1==stop_stat) break;
    // }
    return Status;
}

/** @brief start mesurement and get result.
    @param The result.
    @return Error code, error if not equal to zero
 * */
VL53L0X_Error Seeed_vl53l0x::PerformSingleRangingMeasurement(VL53L0X_RangingMeasurementData_t* RangingMeasurementData) {
    FixPoint1616_t LimitCheckCurrent;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    u32 milli_value_start = 0, milli_value_stop = 0;
    //VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    int i = 0;
    if (Status == VL53L0X_ERROR_NONE) {
        milli_value_start = millis();
        Status = VL53L0X_PerformSingleRangingMeasurement(pMyDevice,
                 RangingMeasurementData);
        milli_value_stop = millis();

        // Serial.print("time of mesurement: ");
        // Serial.println(milli_value_stop-milli_value_start);

        return Status;
    }
}

