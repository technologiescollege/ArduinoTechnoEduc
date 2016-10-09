#include "Duinoedu_MirfAdd.h"





Duinoedu_MirfAdd::Duinoedu_MirfAdd() {
	
	serialInfos = 1;
	m_chanel = -1;
	for(int8_t i=0;i<DATA_WIDTH;i++){
		data[i]=0.0;
	}
	
							
			
}
// METHODES HAUT NIVEAU
void Duinoedu_MirfAdd::branch(int8_t _cePin, int8_t _csnPin){
	Nrf24l::cePin  = _cePin;
	Nrf24l::csnPin = _csnPin;
	Nrf24l::spi    = &MirfHardwareSpi;
	Nrf24l::init();	
}

void Duinoedu_MirfAdd::configDefaultChannelAndPayload(int8_t _channel, int8_t _payload){
	setChannel(_channel);
	setPayload(_payload);
	Nrf24l::config();
}

void Duinoedu_MirfAdd::setMyName(String _name){
	setName(_name, MY_NAME);
}

void Duinoedu_MirfAdd::setReceiverName(String _name){
	setName(_name, RECEIVER_NAME);
}




void Duinoedu_MirfAdd::sendFloatInVirtualChanel(float _number, uint8_t _virtualChannel){
	//---- Nombre avec le canal (on ajoute 2000 à chaque canal)
		float valueWithCh;
	
	//---- On ajoute le canal à la valeur
		if(_number>=0){
			valueWithCh = _number + _virtualChannel*20000;		
		}else{
			valueWithCh = _number - _virtualChannel*20000;
		}
	
	//Nrf24l::send((byte *) &valueWithCh);
	DEBUG("Sending:"); DEBUGLN(valueWithCh);
	//while(Nrf24l::isSending());
	
}

float Duinoedu_MirfAdd::getFloatInVirtualChanel(uint8_t _virtualChannel){
	float 		valueWithCh=0;
	uint32_t    valueWithChX100;
	uint8_t 	chanel;
	float	    value;
	uint32_t	valueX100;

	uint32_t    divisorX100 = 2000000;
	
	
	if(Nrf24l::dataReady()){
		Serial.println("\r\n-----------");
		//---- On récupère la donnée brute
			Nrf24l::getData((byte *) &valueWithCh); 
			DEBUG("ValCh:"); DEBUGLN(valueWithCh);
		
		//---- On supprime la virgule 
			valueWithChX100 = valueWithCh*100;
			DEBUG("ValChX100:"); DEBUGLN(valueWithChX100);
		
		//---- On récupère le canal par division
			chanel = valueWithChX100/divisorX100;
			DEBUG("DivisorX100:"); DEBUGLN(divisorX100);
			DEBUG("Ch:"); DEBUGLN(chanel);
		
		//---- On récupère la valeur sans virgule par modulo
			valueX100  = valueWithChX100%divisorX100;
			DEBUG("ValX100:"); DEBUGLN(valueX100);
		
		//---- On récupère la bonne valeur	
			value = (float)(valueX100/100);
	}

	return value;
	
}






// METHODES PLUS BAS NIVEAU
void Duinoedu_MirfAdd::setName(String _name, int8_t _type){
	char tab[5];
	_name.toCharArray(tab,5);
	
	if(_type==MY_NAME){
		if(m_myName != _name){
			Nrf24l::setRADDR((byte *) tab);
			m_myName = _name;
			DEBUG("MyName:");DEBUGLN(m_myName);
		}
	}
	if(_type==RECEIVER_NAME){
		if(m_receiverName != _name){
			Nrf24l::setTADDR((byte *) tab);
			m_receiverName = _name;
			DEBUG("RcName:");DEBUGLN(m_receiverName);
		}
	}
}




void Duinoedu_MirfAdd::setChannel(int8_t _channel){
	if(m_chanel != _channel){
		Nrf24l::channel = _channel;
		m_chanel = _channel;
		DEBUG("Ch:");DEBUGLN(this->channel);
	}
}


void Duinoedu_MirfAdd::setPayload(int8_t _payload){
	if(m_payload != _payload){
		Nrf24l::payload = _payload;
		m_payload = _payload;
		DEBUG("Pl:");DEBUGLN(this->payload);
	}
}













