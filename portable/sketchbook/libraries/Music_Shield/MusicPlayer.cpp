/*
 * MusicPlayer.cpp
 * A library for MusicShield 2.0
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jack Shao (jacky.shaoxg@gmail.com)
 * Create Time: Mar 2014
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

#include <SD.h>
#include <MusicPlayer.h>
#include  <avr/pgmspace.h>

typedef void (*voidFunctionPtr)(void);
volatile static voidFunctionPtr digitCtrlFunc[ENABLE_NUM_DIGIT];
char _DigitPinLeval[ENABLE_NUM_DIGIT];
char _DigitPinNum[ENABLE_NUM_DIGIT];

volatile static voidFunctionPtr analogCtrlFunc[ENABLE_NUM_ANALOG];
char _AnalogPinNum[ENABLE_NUM_DIGIT];

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 10;
static byte readBuf[READ_BUF_LEN];
volatile static boolean playdone = false;

MusicPlayer player;
Sd2Card     card;
SdVolume    volume;
SdFile      root;
SdFile      cur_file;

playingstatetype playingState;
ctrlState_t      ctrlState;


volatile static byte ledflagOn = 1;
volatile static int  ledcount = 50;
volatile static byte timerloop = 0;
volatile static byte recording_state = 0;
volatile static byte fastforward = 0;

/**************************************************************/
int freeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
void showString(PGM_P s)
{
  char c;
  while ((c = pgm_read_byte(s++)) != 0) Serial.print(c);
}

/**************************************************************/
ISR(TIMER1_OVF_vect)          //Timer1 Service
{
  //fill vs1053
  while (digitalRead(VS_DREQ) == 1 && playingState == PS_PLAY && cur_file.isOpen() && !fastforward)
  {
    byte readLen = 0;
    readLen = cur_file.read(readBuf, READ_BUF_LEN);
    vs1053.writeData(readBuf, readLen);
    if (readLen < READ_BUF_LEN)
    {
      vs1053.writeRegister(SPI_MODE, 0, SM_OUTOFWAV);
      vs1053.sendZerosToVS10xx();
      //report play done event here...
      playingState = PS_POST_PLAY;
      break;
    }
  }
  //update
  if (++timerloop >= 20)
  {
    player._hardtime_update();
    timerloop = 0;
  }
}
/**************************************************************/
//=============================================================
#define RESOLUTION 65536    // Timer1 is 16 bit
void MusicPlayer::_init_timer1(void)        //initialize Timer1 to 100us overflow
{
  TCCR1A = 0;                 // clear control register A
  TCCR1B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer

  long cycles;
  long microseconds = 500;   //setup microseconds here
  unsigned char clockSelectBits;
  cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if (cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum

  ICR1 = cycles;
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock

  TIMSK1 = _BV(TOIE1);
  TCNT1 = 0;
  sei();                      //enable global interrupt
}

/**************************************************************/
void MusicPlayer::begin(void)
{
  initIOForLED();
  if (!Key_Disable) initIOForKeys();
  _keys[0].setPara(KEY_PS, 1, 100, CS_PLAYPAUSE);
  _keys[1].setPara(KEY_VD, 2, 100, CS_DOWN);
  _keys[2].setPara(KEY_VU, 2, 100, CS_UP);
  _keys[3].setPara(KEY_BK, 3, 100, CS_PREV);
  _keys[4].setPara(KEY_NT, 3, 100, CS_NEXT);
  vs1053.init();
  /* init sd card */
  //SD.begin(chipSelect);
  pinMode(chipSelect, OUTPUT);
  if (!card.init(SPI_HALF_SPEED, chipSelect))
  {
    showString(PSTR("initialization failed. Things to check:\r\n"));
    showString(PSTR("* is a card is inserted?\r\n"));
    showString(PSTR("* Is your wiring correct?\r\n"));
    showString(PSTR("* did you change the chipSelect pin to match your shield or module?\r\n"));
    return;
  }
  showString(PSTR("Card type: "));
  switch (card.type())
  {
  case SD_CARD_TYPE_SD1:
    showString(PSTR("SD1\r\n"));
    break;
  case SD_CARD_TYPE_SD2:
    showString(PSTR("SD2\r\n"));
    break;
  case SD_CARD_TYPE_SDHC:
    showString(PSTR("SDHC\r\n"));
    break;
  default:
    showString(PSTR("Unknown\r\n"));
  }
  if (!volume.init(card))
  {
    showString(PSTR("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card\r\n"));
    return;
  }
  showString(PSTR("Volume type is FAT"));
  Serial.println(volume.fatType(), DEC);
  root.openRoot(volume);

  playlistInit();
  setVolume(40);
  _playmode = PM_NORMAL_PLAY;
  playingState = PS_PRE_PLAY;
  _playmode = PM_NORMAL_PLAY;
  ctrlState = CS_EMPTY;
  /* init timer1 */
  _init_timer1();
}

/**************************************************************/
void MusicPlayer::_hardtime_update(void)
{
  if (!Key_Disable) scanKey();
  controlLED();
}

void MusicPlayer::play(void)
{
  if (Analog_Enable) scanAnalogSensor();
  if (Digital_Enable) scanDigitalSensor();
  _play();
}
/**************************************************************/
void MusicPlayer::_play(void)
{
  song_t *songFile;
  switch (playingState)
  {
    //
  case PS_IDLE:
    if (ctrlState == CS_PLAYPAUSE)
    {
      playingState = PS_PRE_PLAY;
      ctrlState = CS_EMPTY;
    }
#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)
    if (ctrlState == CS_PLAYPAUSE_LONG)
    {
      playingState = PS_PRE_RECORD;
      ctrlState = CS_EMPTY;
    }
#endif
    if (cur_file.isOpen()) cur_file.close();
    break;
    //
  case PS_PRE_PLAY:
    if (playlistIsEmpty())
    {
      showString(PSTR("Playlist is empty.\r\n"));
      playingState = PS_IDLE;
      break;
    }
    vs1053.softReset();

    if (cur_file.isOpen()) cur_file.close();
    songFile = spl.p_songFile[spl.currentSongNum];

    if (!cur_file.open(&root, songFile->index, O_READ))
    {
      Serial.print(songFile->name);
      showString(PSTR(" open failed.\r\n"));
      playingState = PS_POST_PLAY;
      break;
    }
    showString(PSTR("Playing song "));
    Serial.print(songFile->name);
    showString(PSTR(" ...\r\n"));
    playingState = PS_PLAY;
    break;
    //
  case PS_PLAY:
    if (ctrlState == CS_PLAYPAUSE)
    {
      ctrlState = CS_EMPTY;
      playingState = PS_PAUSE;
    }
    if (ctrlState == CS_PLAYPAUSE_LONG)
    {
      ctrlState = CS_EMPTY;
      playingState = PS_IDLE;
    }
    if (ctrlState == CS_DOWN)
    {
      ctrlState = CS_EMPTY;
      adjustVolume(0);
    }
    if (ctrlState == CS_UP)
    {
      ctrlState = CS_EMPTY;
      adjustVolume(1);
    }
    if (ctrlState == CS_NEXT)
    {
      ctrlState = CS_EMPTY;

      if (_playmode == PM_SHUFFLE_PLAY) spl.currentSongNum = random(0, spl.songTotalNum);
      else if (spl.currentSongNum == spl.songTotalNum - 1)   spl.currentSongNum = 0;
      else ++spl.currentSongNum;
      cur_file.close();
      playingState = PS_PRE_PLAY;
    }
    if (ctrlState == CS_PREV)
    {
      ctrlState = CS_EMPTY;
      if (_playmode == PM_SHUFFLE_PLAY) spl.currentSongNum = random(0, spl.songTotalNum);
      else if (spl.currentSongNum == 0)  spl.currentSongNum = spl.songTotalNum - 1;
      else --spl.currentSongNum;
      cur_file.close();
      playingState = PS_PRE_PLAY;
    }

    if (ctrlState == CS_NEXT_LONG)
    {
      ctrlState = CS_EMPTY;
      fastforward = 1;
      uint32_t pos = cur_file.curPosition();
      if (pos + FASTFORWARD_LEN < cur_file.fileSize())
      {
        cur_file.seekSet(pos + FASTFORWARD_LEN);
      }
      fastforward = 0;
    }

    if (ctrlState == CS_PREV_LONG)
    {
      ctrlState = CS_EMPTY;
      fastforward = 1;
      uint32_t pos = cur_file.curPosition();
      if (pos - FASTFORWARD_LEN > 0)
      {
        cur_file.seekSet(pos - FASTFORWARD_LEN);
      }
      fastforward = 0;
    }
    break;
    //
  case PS_PAUSE:
    if (ctrlState == CS_PLAYPAUSE)
    {
      ctrlState = CS_EMPTY;
      playingState = PS_PLAY;
    }
    break;
    //
  case PS_POST_PLAY:
    showString(PSTR(" [done].\r\n"));
    if (cur_file.isOpen()) cur_file.close();
    if (_playmode == PM_SHUFFLE_PLAY)
    {
      spl.currentSongNum = random(0, spl.songTotalNum);
      playingState = PS_PRE_PLAY;
    } else if (_playmode == PM_NORMAL_PLAY)
    {
      if (spl.currentSongNum == spl.songTotalNum - 1)
      {
        playingState = PS_IDLE;
      } else
      {
        spl.currentSongNum++;
        playingState = PS_PRE_PLAY;
      }
    } else if (_playmode == PM_REPEAT_LIST)
    {
      if (spl.currentSongNum == spl.songTotalNum - 1)   spl.currentSongNum = 0;
      else spl.currentSongNum++;
      playingState = PS_PRE_PLAY;
    } else if (_playmode == PM_REPEAT_ONE)
    {
      playingState = PS_PRE_PLAY;
    }
    break;

    //
  case PS_PRE_RECORD:
    _preRecording();
    showString(PSTR("Recording...\r\n"));
    recording_state = 0;
    playingState = PS_RECORDING;
    break;
    //
  case PS_RECORDING:
    //recording
    _recording();
    break;
    //
  case PS_POST_RECORD:
    cur_file.close();
    vs1053.softReset();
    showString(PSTR("stop\r\n"));
    playingState = PS_IDLE;
    break;


  default:
    break;
  }
}
/**************************************************************/
void MusicPlayer::playOne(char *songFile)
{
  /*SdFile f;
  if (!f.open(&root, songFile, O_READ))
  {
    Serial.print(songFile);
    showString(PSTR(" does not exists.\r\n"));
    return;
  }
  f.close();
  uint16_t index = root.dirIndex();
  if (!_inPlayList(index))
  {
    addToPlaylist(songFile);
  }*/
  if (addToPlaylist(songFile))
  {
    //added by shao
    //switch to this song
    for(int i = 0; i<spl.songTotalNum; i++)
    {
      if (strcmp(songFile, spl.p_songFile[i]->name) == 0)
      {
        showString(PSTR("Seeked to "));
        Serial.println(songFile);
        spl.currentSongNum = i;
        playingState = PS_PRE_PLAY;
      }
    }
  }else
  {
    showString(PSTR("Failed to add.\r\n"));
  }
  

}

/**************************************************************/
void MusicPlayer::scanAndPlayAll(void)
{
  if (playlistIsEmpty())
  {
    dir_t p;
    uint32_t curPos;
    char name[13];
    uint16_t n = 0;

    showString(PSTR("---- songs in TF card (root dir) ----\r\n"));

    while (root.readDir(&p) >= 0 && n < MAX_SONG_TOTAL_NUM)
    {
      curPos = root.curPosition();

      // done if past last used entry
      if (p.name[0] == DIR_NAME_FREE)
      {
        //showString(PSTR("end");
        break;
      }

      // skip deleted entry and entries for . and  ..
      if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.')
      {
        //showString(PSTR("dots");
        continue;
      }

      // only list subdirectories and files
      if (!DIR_IS_FILE_OR_SUBDIR(&p))
      {
        //showString(PSTR("notafile");
        continue;
      }

      uint32_t sz = p.fileSize;
      if (sz < 8192)
      {
        //showString(PSTR("not a real music file");
        continue;
      }

      // print file name with possible blank fill
      SdFile::dirName(p, name);
      //Serial.print("try to open file ");
      //showString(PSTR(name);

      /* filter the song type */
      char *s = name;
      s = strupr(s);
      if (!strstr(s, ".MP3") && !strstr(s, ".WMA") && !strstr(s, ".WAV") && !strstr(s, ".ACC")
          && !strstr(s, ".MIDI") && !strstr(s, ".OGG") && !strstr(s, ".VORBIS"))
      {
        continue;
      }

      uint16_t index = (curPos - 32) >> 5;
      //Serial.print("file index: ");
      //showString(PSTR(index);

      /*if (f.open(&root, index, O_READ))
      {
        showString(PSTR(" OK!");
        f.close();
      } else
      {
        showString(PSTR("ugh, cant open it");
      }
      root.seekSet(curPos);*/
      //Serial.println(freeRam());
      _addToPlaylist(index, name);
      n++;
      Serial.print(n);
      showString(PSTR(". "));
      Serial.println(s);
    }  //end while

    spl.currentSongNum = 0;

    showString(PSTR("-------------------------------------\r\n"));
  }
}

/**************************************************************/
boolean MusicPlayer::_addToPlaylist(uint16_t index, char *songName) //add a song to current playlist
{
  if (spl.songTotalNum >= (MAX_SONG_TOTAL_NUM - 1)) return false;

  if (_inPlayList(index))
  {
    Serial.print(songName);
    showString(PSTR(" already exists in playlist.\r\n"));
    return true;
  }

  SdFile f;
  if (!f.open(&root, index, O_READ))
  {
    Serial.print(songName);
    showString(PSTR(" cant be opened.\r\n"));
    return false;
  }
  f.close();

  song_t *sd = (song_t *)malloc(sizeof(song_t));
  if (sd)
  {
    strncpy(sd->name, songName, 13);
    sd->index = index;
    spl.p_songFile[spl.songTotalNum] = sd;
    spl.songTotalNum++;
    return true;
  }else
  {
    showString(PSTR("run out of ram.\r\n"));
  }
  return false;
}

boolean MusicPlayer::addToPlaylist(char *songName) //add a song to current playlist
{
  SdFile f;
  if (!f.open(&root, songName, O_READ))
  {
    Serial.print(songName);
    showString(PSTR(" is not found.\r\n"));
    return false;
  }
  f.close();
  uint32_t pos = root.curPosition();
  return _addToPlaylist((pos>>5)-1,songName);
  Serial.print("Add to play list: index");
  Serial.print((pos>>5)-1);
  Serial.print("  songName:");
  Serial.println(songName);


}



/**************************************************************/
boolean MusicPlayer::deleteSong(char *songName)
{
  boolean found = false;
  for (unsigned char i = 0; i < MAX_SONG_TOTAL_NUM; i++)
  {
    if (!found)
    {
      if (spl.p_songFile[i] == 0) continue;
      if (strncmp(songName, spl.p_songFile[i]->name, 11) == 0)
      {
        found = true;
        free(spl.p_songFile[i]);
        spl.p_songFile[i] = 0;
	  Serial.print("Delete song:");
	  Serial.println(songName);
      }
    } else
    {
      spl.p_songFile[i - 1] = spl.p_songFile[i];
    }
  }
  if (found) --spl.songTotalNum;
  return found;
}


/**************************************************************/
void MusicPlayer::adjustVolume(boolean UpOrDown, unsigned char NumSteps)
{
  if (UpOrDown == 0)
  {
    if (_volume < NumSteps) _volume = SILENT;
    else _volume -= NumSteps;
  } else
  {
    if (_volume > MAXVOL - NumSteps) _volume = MAXVOL;
    else _volume += NumSteps;
  }
  setVolume(_volume);
}

/**************************************************************/
void MusicPlayer::attachDigitOperation(int pinNum, void (*userFunc)(void), int mode)
{
  pinMode(pinNum, INPUT);
  _DigitPinLeval[pinNum] = mode;
  _DigitPinNum[pinNum] = pinNum;
  digitCtrlFunc[pinNum] = userFunc;
}
/**************************************************************/
void MusicPlayer::attachAnalogOperation(int pinNum, void (*userFunc)(void))
{
  pinMode(pinNum, INPUT);
  _AnalogPinNum[pinNum - A4] = pinNum;
  analogCtrlFunc[pinNum - A4] = userFunc;
}

/**************************************************************/
boolean MusicPlayer::playlistIsEmpty(void)
{
  if (spl.songTotalNum == 0) return true;
  else return false;
}
void MusicPlayer::playlistInit(void)
{
  unsigned char i;
  for (i = 0; i < MAX_SONG_TOTAL_NUM; i++)
  {
    spl.p_songFile[i] = 0;
  }
  spl.songTotalNum = 0;
  spl.currentSongNum = 0;
}
/**************************************************************/
void MusicPlayer::scanKey(void)
{
  for (byte i = 0; i < 5; i++)
  {
    _keys[i].scan();
    int s = _keys[i].getPress();
    if (s >= 0)
    {
      ctrlState = (ctrlState_t)s;
      break;
    }
  }
}
/********************************************************/
void MusicPlayer::scanAnalogSensor(void)
{
  for (unsigned char i = 0; i < ENABLE_NUM_ANALOG; i++)
  {
    if (analogCtrlFunc[i]) analogCtrlFunc[i]();
  }
}
/********************************************************/
void MusicPlayer::scanDigitalSensor(void)
{
  for (unsigned char i = 0; i < ENABLE_NUM_DIGIT; i++)
  {
    if (digitCtrlFunc[i]) if (digitalRead(_DigitPinNum[i]) == _DigitPinLeval[i])
      {
        digitCtrlFunc[i]();
        while (digitalRead(_DigitPinNum[i]) == _DigitPinLeval[i]);
      }
  }
}
/**************************************************************/
void MusicPlayer::controlLED(void)
{
  if (playingState == PS_PAUSE) GREEN_LED_ON();
  else if (playingState == PS_IDLE) GREEN_LED_OFF();
  else if (--ledcount == 0)
  {
    ledcount = 50;
    if (ledflagOn == 1)
    {
      GREEN_LED_ON();
      ledflagOn = 0;
    } else
    {
      GREEN_LED_OFF();
      ledflagOn = 1;
    }
  }
}
boolean MusicPlayer::playlistIsFull(void)
{
  if (spl.songTotalNum < MAX_SONG_TOTAL_NUM) return false;
  else return true;
}

boolean MusicPlayer::_inPlayList(uint16_t index)
{
  for (unsigned char i = 0; i < MAX_SONG_TOTAL_NUM; i++)
  {
    if (spl.p_songFile[i] == 0) continue;
    if (spl.p_songFile[i]->index == index)
    {
      return true;
    }
  }
  return false;
}

#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)

void MusicPlayer::LoadUserCode(void)
{
  int i = 0;
  int length = sizeof(plugin);
  int dataSize = sizeof(plugin[0]);

  showString(PSTR("\r\nLoadUserCode ..."));
  while (i < length / dataSize)
  {
    unsigned short addr, n, val;
    addr = pgm_read_word(&plugin[i++]);
    n = pgm_read_word(&plugin[i++]);
    if (n & 0x8000U) /* RLE run, replicate n samples */
    {
      n &= 0x7FFF;
      val = pgm_read_word(&plugin[i++]);
      while (n--)
      {
        vs1053.writeRegister(addr, val >> 8, val & 0xff);
      }
    } else           /* Copy run, copy n samples */
    {
      while (n--)
      {
        val = pgm_read_word(&plugin[i++]);
        vs1053.writeRegister(addr, val >> 8, val & 0xff);
      }
    }
  }
  showString(PSTR("over!\r\n"));
}

void MusicPlayer::_preRecording(void)
{

  uint16_t i;

  /*  Now open output file. It's better to do this before activating
      recording so that even if opening the output file is slow,
      you will not lose audio data. */
  SdFile::remove(&root, "record.ogg");

  if (!cur_file.open(&root, "record.ogg", FILE_WRITE))
  {
    showString(PSTR("open record.ogg failed."));
    vs1053.softReset();
    return;
  }

  /* Clear SCI_BASS */
  vs1053.writeRegister(SPI_BASS, 0x00, 0x00);

  // Disable all interrupts except SCI
  vs1053.writeRegister(SPI_WRAMADDR, 0xC0, 0x1A);
  vs1053.writeRegister(SPI_WRAM, 0x2);

  /* Load the recorder application to VS1053
     This source code uses .img image files for loading.
     If you use .plg files, use the source as described
     in the VS1053b Ogg Vorbis Encoder application manual. */
  LoadUserCode();

  vs1053.writeRegister(SPI_MODE, 0x58, 0x00);
  while (!vs1053.readDREQ());

  vs1053.writeRegister(SPI_AICTRL0, 0x00, 0x00);
  vs1053.writeRegister(SPI_AICTRL1, 0x00, 0x00);
  vs1053.writeRegister(SPI_AICTRL2, 0x20, 0x00);
  vs1053.writeRegister(SPI_AICTRL3, 0x00, 0x00);

  /* Activate recording from the address we got. (In the case of the Ogg
     Vorbis Encoder application, pluginStartAddr = 0x34.) */
  vs1053.writeRegister(SPI_AIADDR, 0x34);
  while (!vs1053.readDREQ());
}

void MusicPlayer::_recording(void)
{
  while (recording_state < 3 && playingState == PS_RECORDING && cur_file.isOpen())
  {
    uint16_t wordsToRead;
    uint16_t wordsWaiting;

    if (ctrlState == CS_PLAYPAUSE)
    {
      ctrlState = CS_EMPTY;
      recording_state = 1;
      vs1053.writeRegister(SPI_AICTRL3, 1);
      showString(PSTR("Stop recording...\r\n"));
    }

    wordsWaiting = vs1053.readRegister(SPI_HDAT1);
    if (recording_state == 1 && vs1053.readRegister(SPI_AICTRL3) & (1 << 1))
    {
      recording_state = 2;
      /* It is important to reread the HDAT1 register once after
         VS1053 has stopped. Otherwise there is the chance that
         a few more words have just arrived although we just
         read this register. So, do NOT optimize the following
         line away! */
      wordsWaiting = vs1053.readRegister(SPI_HDAT1);
    }
    while (wordsWaiting >= ((recording_state < 2) ? 256 : 1))
    {
      wordsToRead = min(wordsWaiting, 256);
      wordsWaiting -= wordsToRead;

      /* If this is the very last block, read one 16-bit word less,
      because it will be handled later separately. */
      if (recording_state == 2 && wordsWaiting == 0) wordsToRead--;

      /* Transfer one full data block, or if this is the very last
      block, all data that's left except for the last word. */
      uint16_t t;
      uint16_t i;
      for (i = 0; i < wordsToRead; i++)
      {
        t = vs1053.readRegister(SPI_HDAT0);
        cur_file.write(t >> 8);
        cur_file.write(t & 0xFF);
      }

      /* If this is the last data block... */
      if (wordsToRead < 256)
      {
        uint16_t lastWord;
        recording_state = 3;

        /* ... read the very last word of the file */
        lastWord = vs1053.readRegister(SPI_HDAT0);

        /* Always write first half of the last word. */
        cur_file.write(lastWord >> 8);

        /* Read twice SPI_AICTRL3, then check bit 2 of latter read. */
        vs1053.readRegister(SPI_AICTRL3);
        if (!(vs1053.readRegister(SPI_AICTRL3) & (1 << 2)))
        {
          /* Write last half of the last word only if bit 2 is clear. */
          cur_file.write(lastWord & 0xFF);
        }
      } /* if (wordsToRead < 256) */
    }  //while (wordsWaiting >= ((recording_state < 2) ? 256 : 1))
    if (recording_state == 3)
    {
      playingState = PS_POST_RECORD;
    }
  }  //if recording_state < 3
}

#else

void MusicPlayer::_preRecording(void)
{
  showString(PSTR("Only MEGA2560 board can do recording.\r\n"));
  playingState = PS_IDLE;
}
void MusicPlayer::_recording(void)
{
  showString(PSTR("Only MEGA2560 board can do recording.\r\n"));
  playingState = PS_IDLE;
}
#endif


void Key::setPara(unsigned char io,
                  unsigned char mode,
                  unsigned int long_press_duration,
                  unsigned int return_value)
{
  _io = io;
  _mode = mode;
  _long_press_duration = long_press_duration;
  _return_value = return_value;

  _key_st = KS_IDLE;
  _st_to_return = 0;  //0-nothing, 1-short, 2-long
  _keycnt = 0;
}

void Key::scan(void)
{
  switch (_key_st)
  {
  case KS_IDLE:
    if (digitalRead(_io) == LOW) _key_st = KS_CFM;
    _keycnt = 0;
    _mode3_shoot = false;
    break;

  case KS_CFM:
    if (digitalRead(_io) == LOW) _key_st = KS_PRESS;
    else _key_st = KS_IDLE;
    break;
  case KS_PRESS:
    if (digitalRead(_io) == LOW)
    {
      _keycnt++;
      if (_mode == 2 && _keycnt > (_long_press_duration / 4))
      {
        _st_to_return = 1;
        _keycnt = 0;
      }
      if (_mode == 3 && _keycnt > _long_press_duration)
      {
        _st_to_return = 2;
        _mode3_shoot = true;
        _keycnt = 0;
      }
    } else
    {
      if (_mode == 1 && _keycnt > _long_press_duration) _st_to_return = 2;
      else if (!_mode3_shoot) _st_to_return = 1;
      _keycnt = 0;
      _key_st = KS_IDLE;
    }
    break;
  default:
    break;
  }
}

int Key::getPress(void)
{
  if (_st_to_return == 1)
  {
    _st_to_return = 0;
    return _return_value;
  }
  if (_st_to_return == 2)
  {
    _st_to_return = 0;
    return _return_value + 1;
  }
  return -1;
}

/**********************************Midi Player Part***************************/

/*
 **@ function:beginInMidiFmt
 **@ usage:Init the Midi Player
 **@ input:none
 **@ output:none
*/
void MusicPlayer::beginInMidiFmt(void)
{
  initIOForLED();

  //Init VS105B in Midi Format
  Serial.print("Init vs10xx in MIDI format...");
  vs1053.initForMidiFmt();
  Serial.print("done\r\n");

  pinMode(chipSelect, OUTPUT);
}

/*
 **@ function:midiWriteData
 **@ usage:Write data to Midi Player
 **@ input:
 **@ output:none
*/
void MusicPlayer::midiWriteData(byte cmd, byte high, byte low)
{
  while(!digitalRead(VS_DREQ));
  digitalWrite(VS_XDCS, LOW);
  midiSendByte(cmd);
  if((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0)
  {
    midiSendByte(high);
    midiSendByte(low);
  }
  else
  {
    midiSendByte(high);
  }
  digitalWrite(VS_XDCS, HIGH);
}


void MusicPlayer::midiNoteOn(byte channel, byte note, byte rate)
{
  midiWriteData((0x90 | channel), note, rate);
}

void MusicPlayer::midiNoteOff(byte channel, byte note, byte rate)
{
  midiWriteData((0x80 | channel), note, rate);
}

void MusicPlayer::midiSendByte(byte data)
{
  SPI.transfer(0x00);
  SPI.transfer(data);
}

/*
 **@ function:demoPlayer
 **@ usage:A Midi demo Player
 **@ input:none
 **@ output:none
*/
void MusicPlayer::midiDemoPlayer(void)
{
  delay(1000);
  midiWriteData(0xB0, 0x07, 120);

  //GM2 Mode
  Serial.print("Fancy Midi Sounds\r\n");
  midiWriteData(0xB0, 0, 0x78);
  for(int instrument = 30 ; instrument < 31 ; instrument++)
  {
    Serial.print(" Instrument: ");
    Serial.println(instrument, DEC);

    midiWriteData(0xC0, instrument, 0);    //Set instrument number. 0xC0 is a 1 data byte command

    //Play notes from F#-0 (30) to F#-5 (90):
    for (int note = 27 ; note < 87 ; note++)
    {
      Serial.print("N:");
      Serial.println(note, DEC);
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      midiNoteOn(0, note, 127);
      delay(50);

      //Turn off the note with a given off/release velocity
      midiNoteOff(0, note, 127);
      delay(50);
    }

    //delay 100ms between each instruments
    delay(100);
    }
}


