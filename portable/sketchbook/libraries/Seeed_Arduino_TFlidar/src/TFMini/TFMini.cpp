#include"TFMini.h"

void TFMini::begin(SoftwareSerial *TFSerial,unsigned long baud_rate){
    TFSerial->begin(baud_rate);
    _TFTransporter = TFSerial;
}

void TFMini::begin(HardwareSerial *TFSerial,unsigned long baud_rate){
    TFSerial->begin(baud_rate);
    _TFTransporter = TFSerial;
}

bool TFMini::check_header(recv_package *package){
    uint8_t recv_data;
#ifdef DEBUG_EN
    Serial.print("check_header ");
#endif  
    for(uint8_t index = 0; index < 2; index++){
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

bool TFMini::verify_data(recv_package *package){
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

bool TFMini::get_frame_data(void){
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

uint16_t TFMini::get_distance(){
    uint16_t distance = _package->distance[0] + (uint16_t)_package->distance[1] * 255;
    return distance < 1200 ? distance : 1200;
}

uint16_t TFMini::get_strength(){
    uint16_t strength = _package->strength[0] + (uint16_t)_package->strength[1] * 255;
    return strength;
}
bool TFMini::set_baud_rate(uint32_t baud_rate){
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

bool TFMini::configure(uint8_t down[],int n1,int buff[],uint8_t up[],int n2,String info){
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