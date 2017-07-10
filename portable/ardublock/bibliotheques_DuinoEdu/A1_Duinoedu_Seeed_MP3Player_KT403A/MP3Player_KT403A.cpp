/*
 * MP3Player_KT403A.cpp
 * A library for Grove-Serial MP3 Player V2.0
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Dec 2015
 * Modified Time:
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include <Arduino.h>
#include "MP3Player_KT403A.h"


// Note: You must define a SoftwareSerial class object that the name must be mp3, 
//       but you can change the pin number according to the actual situation.
//SoftwareSerial mp3(2, 3);         // define in the demo file 


/**************************************************************** 
 * Function Name: SelectPlayerDevice
 * Description: Select the player device, U DISK or SD card.
 * Parameters: 0x01:U DISK;  0x02:SD card
 * Return: none
****************************************************************/ 
void SelectPlayerDevice(uint8_t device)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x09);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(device);
    mp3.write(0xEF);
    delay(200);
}

/**************************************************************** 
 * Function Name: SpecifyMusicPlay
 * Description: Specify the music index to play, the index is decided by the input sequence of the music.
 * Parameters: index: the music index: 0-65535.
 * Return: none
****************************************************************/ 
void SpecifyMusicPlay(uint16_t index)
{
    uint8_t hbyte, lbyte;
    hbyte = index / 256;
    lbyte = index % 256;
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x03);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(hbyte));
    mp3.write(uint8_t(lbyte));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: SpecifyfolderPlay
 * Description: Specify the music index in the folder to play, the index is decided by the input sequence of the music.
 * Parameters: folder: folder name, must be number;  index: the music index.
 * Return: none
****************************************************************/ 
void SpecifyfolderPlay(uint8_t folder, uint8_t index)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x0F);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(folder));
    mp3.write(uint8_t(index));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: PlayPause
 * Description: Pause the MP3 player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void PlayPause(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x0E);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
//  mp3.write(0xFE);
//  mp3.write(0xED);
    mp3.write(0xEF);
    delay(20);
//  return true;
}

/**************************************************************** 
 * Function Name: PlayResume
 * Description: Resume the MP3 player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void PlayResume(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x0D);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
//  mp3.write(0xFE);
//  mp3.write(0xEE);
    mp3.write(0xEF);
    delay(20);
//  return true;
}

/**************************************************************** 
 * Function Name: PlayNext
 * Description: Play the next song.
 * Parameters: none
 * Return: none
****************************************************************/ 
void PlayNext(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x01);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: PlayPrevious
 * Description: Play the previous song.
 * Parameters: none
 * Return: none
****************************************************************/ 
void PlayPrevious(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x02);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: PlayLoop
 * Description: Play loop for all the songs.
 * Parameters: none
 * Return: none
****************************************************************/ 
void PlayLoop(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x11);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(0x01);
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: SetVolume
 * Description: Set the volume, the range is 0x00 to 0x1E.
 * Parameters: volume: the range is 0x00 to 0x1E.
 * Return: none
****************************************************************/ 
void SetVolume(uint8_t volume)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x06);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(volume);
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: IncreaseVolume
 * Description: Increase the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void IncreaseVolume(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x04);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: DecreaseVolume
 * Description: Decrease the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void DecreaseVolume(void)
{
    mp3.write(0x7E);
    mp3.write(0xFF);
    mp3.write(0x06);
    mp3.write(0x05);
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(uint8_t(0x00));
    mp3.write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: printReturnedData
 * Description: Print the returned data that sent from the Grove_Serial_MP3_Player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void printReturnedData(void)
{
    unsigned char c;
    //check if there's any data sent from the Grove_Serial_MP3_Player
    while(mp3.available())
    {
        c = mp3.read();
        Serial.print("0x");
        Serial.print(c, HEX);
        Serial.print(" ");
    }
    Serial.println(" "); 
}

/**************************************************************** 
 * Function Name: QueryPlayStatus
 * Description: Query play status.
 * Parameters: none
 * Return: 0: played out; 1: other.
 * Usage: while(QueryPlayStatus() != 0);  // Waiting to play out.
****************************************************************/ 
uint8_t QueryPlayStatus(void)
{
    unsigned char c[10] = {0};
    uint8_t i = 0;
    //check if there's any data sent from the Grove_Serial_MP3_Player
    while(mp3.available())
    {
        c[i] = mp3.read();
        i++;
		delay(1);
		if (i == 10) break;
//        Serial.print(" 0x");
//        Serial.print(c[i], HEX);
    }
//    Serial.println(" "); 
    
    if(c[3] == 0x3C || c[3] == 0x3D || c[3] == 0x3E)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}