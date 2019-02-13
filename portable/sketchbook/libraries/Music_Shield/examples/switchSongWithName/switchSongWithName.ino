/*
 * switchSongWithName.ino
 * Example sketch for MusicShield 2.0
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jack Shao (jacky.shaoxg@gmail.com)
 * Create Time: Mar 2015
 * Change Log :
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

/*
Need to connect a button to D2
*/


#include <SD.h>
#include <SPI.h>
#include <arduino.h>
#include <MusicPlayer.h>

int which_play = 0;
char mp3name[2][10] = {"2.mp3", "1.mp3"};
int flag=0;

void setup(void)
{
  Serial.begin(9600);
  player.begin();  //will initialize the hardware and set default mode to be normal.
  player.keyDisable();
  player.setPlayMode(PM_REPEAT_ONE); //set mode to repeat playing a song
  player.playOne("1.mp3");
  attachInterrupt(0, change, CHANGE);
}

void loop(void)
{

  player.play();
  
    if(flag)
    {
        flag = 0;
        player.playOne(mp3name[which_play]);
        which_play = 1-which_play;
    }
}

void change()
{
    flag = 1;
}

