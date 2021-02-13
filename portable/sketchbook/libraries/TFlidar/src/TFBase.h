#ifndef TF_BASE_H__
#define TF_BASE_H__

#include <stdbool.h>
#include <stdint.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
// #define DEBUG_EN
#define RECV_HEADER 0x59
typedef struct {
    uint8_t header[2];
    uint8_t distance[2];
    uint8_t strength[2];
    uint8_t chip_temperature[2];
    uint16_t checksum;
} recv_package;
typedef enum{
    SAMPLERATE_1HZ = 0,
    SAMPLERATE_10HZ,
    SAMPLERATE_100HZ,
}samplerate_mode;

class TFBase {
    public:
        virtual void begin(SoftwareSerial *TFSerial , uint32_t baud_rate = 115200) = 0;
        virtual void begin(HardwareSerial *TFSerial , uint32_t baud_rate = 115200) = 0;
        virtual uint16_t get_distance(void) = 0;
        virtual uint16_t get_strength(void) = 0;
        virtual uint8_t get_chip_temperature(void) = 0;
        virtual uint16_t get_version(int buff[]) = 0;
        virtual bool set_output_status(bool status) = 0;
        virtual bool set_comunication_mode(uint8_t * cmd) = 0;
        virtual bool set_frame_rate(samplerate_mode mode) = 0;
        virtual bool reset_device() = 0;
        virtual bool factory_reset() = 0;
        virtual bool save_config() = 0;
        virtual bool get_frame_data() = 0;
        virtual bool set_baud_rate(uint32_t baud_rate) = 0;
    protected:
        virtual bool check_header(recv_package *package);
        virtual bool verify_data(recv_package *package);
};
#include "TFLuna/TFLuna.h"
#include "TFMini/TFMini.h"
#endif