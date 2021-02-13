#include"TFLuna.h"


// uint8_t UART[5]={0x5a,0x05,0x0a,0x00,0x69};//Modify communication mode to UART
// uint8_t I2C[5]={0x5a,0x05,0x0a,0x01,0x6a};//Modify communication mode to I2C

// String info_UART="UART ok";
// String info_I2C="I2C ok";


void TFLuna::begin(SoftwareSerial *TFSerial,uint32_t baud_rate){
    TFSerial->begin(baud_rate);
    _TFTransporter = TFSerial;
}

void TFLuna::begin(HardwareSerial *TFSerial,uint32_t baud_rate){
    TFSerial->begin(baud_rate);
    _TFTransporter = TFSerial;
}

bool TFLuna::check_header(recv_package *package){
    uint8_t recv_data;
#ifdef DEBUG_EN
    Serial.print("check_header ");
#endif  
    for(uint8_t index = 0; index < 2; index++){
        package->header[index] = 0x00;
        package->header[index] = (uint8_t)_TFTransporter->read();
        if(RECV_HEADER != package->header[index]){
#ifdef DEBUG_EN
            Serial.println("failed");
#endif              
            return false;
        }
    }
#ifdef DEBUG_EN
    Serial.println("successful");
#endif     
    return true;
}

bool TFLuna::verify_data(recv_package *package){
    uint16_t checksum = package->header[0] + package->header[1]\
    + package->distance[0] + package->distance[1]     \
    + package->strength[0] + package->strength[1]     \
    + package->chip_temperature[0] + package->chip_temperature[1];
#ifdef DEBUG_EN
    Serial.print("verify_data ");
#endif 
    if((checksum & 0xff) == (uint16_t)(package->checksum)){
#ifdef DEBUG_EN
    Serial.println("successful");
#endif
        return true;
    }
#ifdef DEBUG_EN
    Serial.println("failed");
#endif
    return false;    
}

bool TFLuna::get_frame_data(void){
    while(_TFTransporter->read() >= 0);    
    while(!(9 < _TFTransporter->available()));
    uint16_t check;
    if(!check_header(_package)){
        return false;
    }
    for (uint8_t i = 2; i < 9; i++) { //save data in array
        uart[i] = _TFTransporter->read();
    }
    _package->header[0] = RECV_HEADER;
    _package->header[1] = RECV_HEADER;
    _package->checksum = uart[8];
    _package->distance[0] = uart[2];
    _package->distance[1] = uart[3];
    _package->strength[0] = uart[4];
    _package->strength[1] = uart[5];
    _package->chip_temperature[0] = uart[6];
    _package->chip_temperature[1] = uart[7];
    if(!verify_data(_package)){
#ifdef DEBUG_EN
    Serial.print("UART <= ");
    for(uint8_t index = 0 ;index < 2 ;index++){
        Serial.print(_package->header[index],HEX);
        Serial.print(" ");
    }        
    for(uint8_t index = 0 ;index < 2 ;index++){
        Serial.print(_package->distance[index],HEX);
        Serial.print(" ");
    }
    for(uint8_t index = 0 ;index < 2 ;index++){
        Serial.print(_package->strength[index],HEX);
        Serial.print(" ");
    }        
    for(uint8_t index = 0 ;index < 2 ;index++){
        Serial.print(_package->chip_temperature[index],HEX);
        Serial.print(" ");
    }      
    Serial.println(_package->checksum & 0xff,HEX);
#endif  
        return false;
    }
    return true;
}

uint16_t TFLuna::get_distance(){
    uint16_t distance = _package->distance[0] + (uint16_t)_package->distance[1] * 255;
    return distance < 800 ? distance : 800;
}

uint16_t TFLuna::get_strength(){
    uint16_t strength = _package->strength[0] + (uint16_t)_package->strength[1] * 255;
    return strength;
}

uint8_t TFLuna::get_chip_temperature(){
    float chip_temperature = _package->chip_temperature[0] + (uint16_t)_package->chip_temperature[1] * 255;
    chip_temperature = (uint8_t)(chip_temperature / 8 - 256);
    return chip_temperature;
}

uint16_t TFLuna::get_version(int buff[]){
    uint8_t getversion[4]={0x5a,0x04,0x01,0x5f};//Get firmware version
    int return_version[7]={0};//The response after getting firmware versions
    String info_getversion="get version ok";
    configure(getversion,sizeof(getversion),return_version\
    ,sizeof(return_version)/sizeof(int),info_getversion);
    #ifdef DEBUG_EN
        for(uint8_t index = 0 ;index < sizeof(return_version)/sizeof(int);index ++){
            Serial.print(return_version[index],HEX);
            Serial.print(" ");
        }
    #endif
        for(uint8_t index = 0 ;index < 3;index ++){
            buff[index] = return_version[index+3];
        } 
    return true;
}

bool TFLuna::set_output_status(bool status){
    //The response after setting enable switch
    int return_switch[5]={0};
    uint8_t enable[5]={0x5a,0x05,0x07,0x00,0x66};//Enable data output
    uint8_t disable[5]={0x5a,0x05,0x07,0x01,0x67};//Disable data output 
    String info_disable="disable ok";
    String info_enable="enable ok";
    if(status){
        if(configure(enable,sizeof(enable),return_switch,enable\
        ,sizeof(return_switch)/sizeof(int),info_enable)){
            return true;
        }
    }
    else{
        if(configure(disable,sizeof(disable),return_switch,disable\
        ,sizeof(return_switch)/sizeof(int),info_disable)){
            return true;
        }
    }
    return false;
}
bool TFLuna::set_frame_rate(samplerate_mode mode){
    uint8_t samplerate_01[6]={0x5a,0x06,0x03,0x01,0x00,0x64};//Set the frame rate  to 1Hz  
    uint8_t samplerate_10[6]={0x5a,0x06,0x03,0x0a,0x00,0x6d};//Set the frame rate  to 10Hz 
    uint8_t samplerate_100[6]={0x5a,0x06,0x03,0x64,0x00,0xc7};//Set the frame rate  to 100Hz 
    //The response after setting samplerate
    int return_samplerate[6]={0};
    String info_samplerate_01="samplerate_01 ok";
    String info_samplerate_10="samplerate_10 ok";
    String info_samplerate_100="samplerate_100 ok";
    if(SAMPLERATE_1HZ == mode){
        if(configure(samplerate_01,sizeof(samplerate_01),return_samplerate,samplerate_01 \
        ,sizeof(return_samplerate)/sizeof(int),info_samplerate_01)){
            return true;
        }
    }
    else if (SAMPLERATE_10HZ == mode){
        if(configure(samplerate_10,sizeof(samplerate_10),return_samplerate,samplerate_10 \
        ,sizeof(return_samplerate)/sizeof(int),info_samplerate_10)){
            return true;
        }
    }
    else if (SAMPLERATE_100HZ == mode){
        if(configure(samplerate_100,sizeof(samplerate_100),return_samplerate,samplerate_100 \
        ,sizeof(return_samplerate)/sizeof(int),info_samplerate_100)){
            return true;
        }
    }else
    {
        return false;
    }
    return false;
}

bool TFLuna::reset_device(){
    uint8_t reset[4]={0x5a,0x04,0x02,0x60};//reset
    int return_reset[5]={0};//The response after resetting
    uint8_t reset_success[5]={0x5a,0x05,0x02,0x00,0x60};
    uint8_t reset_fail[5]={0x5a,0x05,0x02,0x01,0x61};
    String info_reset="reset ok";
    if(configure(reset,sizeof(reset),return_reset,\
    reset_success,sizeof(return_reset)/sizeof(int),info_reset)){
        return true;
    };
    return false;
}

bool TFLuna::factory_reset(){
    uint8_t factoryreset[4]={0x5a,0x04,0x10,0x6e};//Restore factory settings
    uint8_t factoryreset_success[5]={0x5a,0x05,0x10,0x00,0x6e};
    uint8_t factoryreset_fail[5]={0x5a,0x05,0x10,0x01,0x6f};
    //The response after restoring factory settings
    int return_factoryreset[5]={0};
    String info_factoryreset="factoryreset ok";
    if(configure(factoryreset,sizeof(factoryreset),return_factoryreset,\
    factoryreset_success,sizeof(return_factoryreset)/sizeof(int),info_factoryreset)){
        return true;
    }
    return false;
}

bool TFLuna::save_config(){
    uint8_t save[4]={0x5a,0x04,0x11,0x6f};//save
    //The response after saving
    int return_save[5]={0};
    uint8_t save_success[5]={0x5a,0x05,0x11,0x00,0x70};
    uint8_t save_fail[5]={0x5a,0x05,0x11,0x01,0x71};
    String info_save="save ok";
    if(configure(save,sizeof(save),return_save,save_success,\
    sizeof(return_save)/sizeof(int),info_save)){
        return true;
    }
    return false;
}
bool TFLuna::set_baud_rate(uint32_t baud_rate){
    uint8_t baud_rate_array[8] = {0x5a, 0x08, 0x06, 0x80, 0x25, 0x00, 0x00, 0x00};
    int baud_rate_return[8] = {0};
    uint16_t check_sum = 0;
    String baud_rate_info = "baud_rate config successful";
    if(baud_rate >> 8 > 0xff){
        baud_rate_array[5] = (baud_rate >> 16) & 0xff;
        baud_rate_array[4] = (baud_rate >> 8) & 0xff;
        baud_rate_array[3] = baud_rate & 0xff;
    }
    else{
        baud_rate_array[4] = baud_rate >> 8;
        baud_rate_array[3] = baud_rate & 0xff;
    }
    for (uint8_t index = 0; index < sizeof(baud_rate_array); index ++){
        check_sum += baud_rate_array[index];
    }
    baud_rate_array[7] = (uint8_t)check_sum;
    configure(baud_rate_array,sizeof(baud_rate_array),baud_rate_return,baud_rate_array\
    ,sizeof(baud_rate_return)/sizeof(int),baud_rate_info);
    return true;
}

bool TFLuna::configure(uint8_t down[],int n1,int buff[],uint8_t up[],int n2,String info){
    uint8_t num=0;
    for(uint8_t i=0;i<n1;i++){
        _TFTransporter->write(down[i]);
        delayMicroseconds(100);
    }
    while(!_TFTransporter->available());
    if(down[2] == 0x06) return true;
    while(true){
        if(_TFTransporter->read()==0x5a){ //assess communication protocol frame header 0x5a
            buff[0]=0x5a;
            for(uint8_t i=1;i<n2;i++){
                delay(5);
                buff[i]=_TFTransporter->read(); 
                #ifdef DEBUG_EN
                    Serial.print(buff[i],HEX);
                    Serial.print('\t');
                #endif
            }
            for(uint8_t i=0;i<n2;i++){
                #ifdef DEBUG_EN
                    Serial.print(up[i],HEX);
                    Serial.print('\t');
                #endif                
                if(buff[i]==up[i]){
                    #ifdef DEBUG_EN
                        Serial.print(buff[i],HEX);
                        Serial.print('\t');
                    #endif
                    num++;
                }    
            }
            if(num==n2){
                num=0;
                #ifdef DEBUG_EN
                    Serial.print(info);
                    Serial.print('\n'); 
                #endif
                return true;
            }
            return false;
        }
    }  
}

bool TFLuna::configure(uint8_t down[],int n1,int buff[],int n2,String info)
{   
    for(uint8_t i=0;i<n1;i++){
        _TFTransporter->write(down[i]);
    }
    while(!_TFTransporter->available());
    while(true){
        if(_TFTransporter->read()==0x5a){ //assess communication protocol frame header 0x5a
            buff[0]=0x5a;
            for(uint8_t i=1;i<n2;i++){
                delay(5);
                buff[i]=_TFTransporter->read();
            }
        #ifdef DEBUG_EN
            for(uint8_t i=0;i<n2;i++){
                Serial.print(buff[i],HEX);  
                Serial.print('\t');
            }
            Serial.print(info);
            Serial.print('\n');
        #endif 
        return true;
        }
    } 
}

bool TFLuna::configure(uint8_t down[],int n1,String info)
{
    for(uint8_t i=0;i<n1;i++){
        _TFTransporter->write(down[i]);
    } 
    #ifdef DEBUG_EN
        Serial.print(info);
        Serial.print('\n');  
    #endif 
    return true;
}