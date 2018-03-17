/*
 * MP3Player_KT403A.h
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
 

#ifndef __MP3PLAYER_KT403A_H__
#define __MP3PLAYER_KT403A_H__


#include <SoftwareSerial.h>

extern SoftwareSerial mp3;


void SelectPlayerDevice(uint8_t device);
void SpecifyMusicPlay(uint16_t index);
void SpecifyfolderPlay(uint8_t folder, uint8_t index);
void PlayPause(void);
void PlayResume(void);
void PlayNext(void);
void PlayPrevious(void);
void PlayLoop(void);
void SetVolume(uint8_t volume);
void IncreaseVolume(void);
void DecreaseVolume(void);
uint8_t QueryPlayStatus(void);
void printReturnedData(void);


#endif
