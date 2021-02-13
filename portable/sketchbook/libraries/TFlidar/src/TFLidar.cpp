#include "TFLidar.h"

TFLidar::TFLidar(TFBase *TF_Lidar){
    _TF_Lidar = TF_Lidar;
}

void TFLidar::begin(SoftwareSerial *TFSerial,uint32_t baud_rate){
    _TF_Lidar->begin(TFSerial,baud_rate);
}

void TFLidar::begin(HardwareSerial *TFSerial,uint32_t baud_rate){
    _TF_Lidar->begin(TFSerial,baud_rate);
}

uint16_t TFLidar::get_distance(){
    return _TF_Lidar->get_distance();
}

uint16_t TFLidar::get_strength(){
    return _TF_Lidar->get_strength();
}

uint8_t TFLidar::get_chip_temperature(){
    return _TF_Lidar->get_chip_temperature();
}

bool TFLidar::get_frame_data(){
    return _TF_Lidar->get_frame_data();
}

uint16_t TFLidar::get_version(int buff[]){
    return _TF_Lidar->get_version(buff);
}

bool TFLidar::set_frame_rate(samplerate_mode mode){
    return _TF_Lidar->set_frame_rate(mode);
}

bool TFLidar::set_output_status(bool status){
    return _TF_Lidar->set_output_status(status);
}
bool TFLidar::reset_device(){
    return _TF_Lidar->reset_device();
}

bool TFLidar::factory_reset(void){
    return _TF_Lidar->factory_reset();
}

bool TFLidar::save_config(void){
    return _TF_Lidar->save_config();
}

bool TFLidar::set_baud_rate(uint32_t baud_rate){
    return _TF_Lidar->set_baud_rate(baud_rate);
}
