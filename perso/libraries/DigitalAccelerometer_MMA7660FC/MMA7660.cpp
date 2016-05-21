/*****************************************************************************/
//    Function:     Cpp file for class MMC7660
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//    Arduino IDE: Arduino-1.0
//    Author:     Frankie.Chu
//    Date:      Jan 10,2013
//    Version: v0.9b
//    by www.seeedstudio.com
//
//  modify by loovee
//  2013-9-25
//  add time out funtion in getXYZ
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/
#include <Wire.h>
#include <Arduino.h>
#include "MMA7660.h"

//EDU:Ajouts---------------------------------------------------------

MMA7660::MMA7660(){	
	m_lastX = 500;				// Position horizontale
	m_lastY = 500;				// Position horizontale
	m_lastZ = 500;				
	noise=30;						// On part du principe que le bruit est de 50 (d'après nos tests)
	m_nbrOfEch=5;				// Nombre d'échantillons pour une mesure
	correctionEnX=-27;						// Décalage en X
	correctionEnY=0;							// Y
	correctionEnZ=0;							// Z
	
}


// ---------- Retourne l'accélération en g sur X
/*FR*/ int MMA7660::mesurerX(){
		
		return effectiveGetAcceleration('X');			
		
}
/*US*/ float MMA7660::getAccelerationX(){
	getAcceleration(&accX,&accY,&accZ);
		
	return accX;
}

// ---------- Retourne l'accélération en g sur Y
/*FR*/ int MMA7660::mesurerY(){

		return effectiveGetAcceleration('Y');	
}
/*US*/ float MMA7660::getAccelerationY(){
	getAcceleration(&accX,&accY,&accZ);
	return accY;
}

// ---------- Retourne l'accélération en g sur Z
/*FR*/ int MMA7660::mesurerZ(){return getAccelerationZ();}
/*US*/ float MMA7660::getAccelerationZ(){
	getAcceleration(&accX,&accY,&accZ);
	return accZ;
}

int MMA7660::effectiveGetAcceleration(char axis){

		
		//-- On mesure la valeur brute (-1.5 > 1.5)
			//Serial.println("--------------------");
			int temp=0;
			uint16_t ecart=0;
			
			//Serial.println(temp);
			for(uint8_t i=0; i<m_nbrOfEch;i++){
				if(axis=='X'){temp+= 1000*getAccelerationX();}			
				if(axis=='Y'){temp+= 1000*getAccelerationY();}	
				if(axis=='Z'){temp+= 1000*getAccelerationZ();}
				delay(5);
			}
			temp/=m_nbrOfEch;
			//Serial.println("Moyenne : ");
			//Serial.println(temp);
			
		//-- On mappe
		if(temp>0){
			temp=map(temp,0,1000,500,1000);
		}
		else{
			temp=map(temp,-1000,0,0,500);
			//Serial.println("Après map : ");
			//Serial.println(temp);
		}
	    
		//-- On ré-étalonne
			if(axis=='X'){temp=temp+correctionEnX;}			
			if(axis=='Y'){temp=temp+correctionEnY;Serial.println(temp);}	
			if(axis=='Z'){temp=temp+correctionEnZ;}
			
		//-- On fixe des limites
			if(temp>1000){temp=1000;}
			if(temp<0      ){temp=0;}
	
		//-- Si la valeur est significative, on la sauvegarde
			if(axis=='X'){
				if(temp > m_lastX+noise || temp < m_lastX-noise){
				//-- On stocke la nouvelle valeur en mémoire
					m_lastX=temp;
				}
			}			
			if(axis=='Y'){
				//Serial.println("m_lastY+noise : ");
				//Serial.println(m_lastY+noise);
				if(temp > (m_lastY+noise) || temp < (m_lastY-noise)){
				//-- On stocke la nouvelle valeur en mémoire
					m_lastY=temp;
										Serial.println("Sauvegarde de temp avec : ");
					//Serial.println(temp);
				}
			}	
			if(axis=='Z'){
				if(temp > m_lastZ+noise || temp < m_lastZ-noise){
				//-- On stocke la nouvelle valeur en mémoire

					m_lastZ=temp;
				}
			}
			
		
		//-- On retourne la valeur
			if(axis=='X'){return m_lastX;}			
			if(axis=='Y'){return m_lastY;}	
			if(axis=='Z'){return m_lastZ;}		
	
}

// ---------- Résultat dans 3 variables
/*FR*/ void MMA7660::mesurerDans(float &valX, float &valY, float &valZ){
	returnIn(valX, valY, valZ);
}
/*US*/ void MMA7660::returnIn(float &valX, float &valY, float &valZ){
	//stockage dans le variables utilisateur
	getAcceleration(&valX,&valY,&valZ);
	x=valX;
	y=valY;
	z=valZ;

}
// ---------- Retourne la somme absolue XZZ
/*FR*/ float MMA7660::mesurerXYZ(){return getAccelerationXYZ();}
/*US*/ float MMA7660::getAccelerationXYZ(){
	getAcceleration(&accX,&accY,&accZ);
	float sum = abs(accX) + abs(accY) + abs(accZ);
	return sum;
}

// ---------- Retourne une valeur image du mouvement
/*FR*/ float MMA7660::mesurerMouvement(){return getMouvement();}
/*US*/ float MMA7660::getMouvement(){
	float dt1=mesurerXYZ();
	delay(100);
	float dt2=mesurerXYZ();
	float dt=abs(dt1-dt2);
	if(dt<0.20){ dt=0; }
	//Serial.println(dt);
	return dt;
}


// ---------- Initialisation
/*FR*/ void  MMA7660::brancher(){init();}

/*Function: Write a byte to the register of the MMA7660*/
void MMA7660::write(uint8_t _register, uint8_t _data)
{
    Wire.begin();
    Wire.beginTransmission(MMA7660_ADDR);
    Wire.write(_register);
    Wire.write(_data);
    Wire.endTransmission();
}
/*Function: Read a byte from the regitster of the MMA7660*/
uint8_t MMA7660::read(uint8_t _register)
{
    uint8_t data_read;
    Wire.begin();
    Wire.beginTransmission(MMA7660_ADDR);
    Wire.write(_register);
    Wire.endTransmission();
    Wire.beginTransmission(MMA7660_ADDR);
    Wire.requestFrom(MMA7660_ADDR,1);
    while(Wire.available())
    {
        data_read = Wire.read();
    }
    Wire.endTransmission();
    return data_read;
}

void MMA7660::init()
{
    setMode(MMA7660_STAND_BY);
    setSampleRate(AUTO_SLEEP_120);
    setMode(MMA7660_ACTIVE);
}

void MMA7660::setMode(uint8_t mode)
{
    write(MMA7660_MODE,mode);
}
void MMA7660::setSampleRate(uint8_t rate)
{
    write(MMA7660_SR,rate);
}
/*Function: Get the contents of the registers in the MMA7660*/
/*          so as to calculate the acceleration.            */
unsigned char MMA7660::getXYZ(int8_t *x,int8_t *y,int8_t *z)
{

    unsigned char val[3];
    int count = 0;
    val[0] = val[1] = val[2] = 64;
    
    long timer1 = millis();
    long timer2 = 0;
    
    while(Wire.available() > 0)
    {
        timer2 = millis();
        if((timer2-timer1)>500)
        {
            return 0;
        }
    }

    Wire.read();
    Wire.requestFrom(MMA7660_ADDR,3);

    timer1 = millis();
    while(Wire.available())
    {
    
        if(count < 3)
        {

            timer1 = millis();
            while ( val[count] > 63 )  // reload the damn thing it is bad
            {
                val[count] = Wire.read();

                timer2 = millis();
                if((timer2-timer1)>50)
                {
                    return 0;
                }

            }
        }

        count++;
        
        timer2 = millis();
        
        if((timer2-timer1)>500)
        {
            return 0;
        }

    }
    
    *x = ((char)(val[0]<<2))/4;
    *y = ((char)(val[1]<<2))/4;
    *z = ((char)(val[2]<<2))/4;
    
    return 1;
}

unsigned char MMA7660::getAcceleration(float *ax,float *ay,float *az)
{
    int8_t x,y,z;
    if(!getXYZ(&x, &y, &z))return 0;
    *ax = x/21.00;
    *ay = y/21.00;
    *az = z/21.00;
    
    return 1;
    
}



