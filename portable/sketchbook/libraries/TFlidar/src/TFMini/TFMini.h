#ifndef TF_MINI_H__
#define TF_MINI_H__

#include "TFBase.h"
#include <Arduino.h>

class TFMini : public TFBase {
    public:
        void begin(SoftwareSerial *TFSerial , unsigned long baud_rate = 115200);
        void begin(HardwareSerial *TFSerial , unsigned long baud_rate = 115200);
        uint16_t get_distance(void);
        uint16_t get_strength(void);
        uint8_t get_chip_temperature(void){};
        uint16_t get_version(int buff[]){};
        bool set_output_status(bool status){};
        bool set_comunication_mode(uint8_t * cmd){};
        bool set_frame_rate(samplerate_mode mode){};
        bool reset_device(void){};
        bool factory_reset(void){};
        bool save_config(void){};
        bool set_baud_rate(uint32_t baud_rate);
        bool get_frame_data(void);
    private:
        Stream *_TFTransporter;
        recv_package *_package = (recv_package*) malloc(sizeof(recv_package));
        uint8_t uart[9];  //save data measured by LiDAR
        //The parameter configuration function, down is the instruction to be written,
        //n1 is the number of downs,buff receives response data,
        //up receives theoretical response data in communication protocols,
        //n2 is the number of buff data,info is the prompt information of corresponding instructions
        bool configure(uint8_t down[],int n1,int buff[],uint8_t up[],int n2,String info);
        bool configure(uint8_t down[],int n1,int buff[],int n2,String info){};
        bool configure(uint8_t down[],int n1,String info){};
    protected:
        bool check_header(recv_package *package);
        bool verify_data(recv_package *package);
};
#endif