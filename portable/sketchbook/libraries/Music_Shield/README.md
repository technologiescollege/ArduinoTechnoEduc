Music Shield V2.0
-------------

<img src=https://statics3.seeedstudio.com/seeed/img/2016-10/8QVKbEtetX6Vot3RsGFDInE4.jpg width=400><img src=https://statics3.seeedstudio.com/seeed/img/2016-10/WQMT4JY83Grqzo7Ge5aDlZIu.jpg width=400>


[Music Shield V2.0](https://www.seeedstudio.com/s/Music-Shield-V2.0-p-1372.html)

Music Shield is an audio encoder/decoder compatible with Arduino, Seeeduino,<br>
Seeeduino Mega and Arduino Mega. It is based on the VC1053B chip, which enabled <br>
it to play sound files from SD card and do short-time recording as well.

**This library is for MusicShield hardware version 2.0.**

For hardware version 1.0, please visit the [wiki page](http://wiki.seeedstudio.com/Music_Shield_V2.2/) for demo codes.

The music player reads data from SD card and then feed into the VS1053 chip via SPI bus. This is done in TIMER1's
ISR with a interval of 2ms. This job has a high priority and uses the cpu exclusively. In the spare cpu time, player
does some process control, like play/pause.

### Usage:
1. Downlaod this library. Two way to do this:<br>
a. click the "Download ZIP" link right down at this page;<br>
b. use a git client to make a clone of "https://github.com/Seeed-Studio/Music_Shield.git"; <br>

2. Copy files(unzip if you downloaded a zip) into a dir named "MusicPlayer";

3. Copy dir "MusicPlayer" into Arduino IDE's library dir;

4. Coding guide:<br>
(1) make sure SD lib and SPI lib was installed;<br>
(2) include header files like example do;<br>
(3) call player.begin() in setup() function;<br>
(4) call other setup APIs or config APIs in setup() function;<br>
(5) call player.play() in loop() function;<br>
(6) call other config APIs in loop() function;<br>
(7) then forget the music player and insert your own routine into setup() or loop() function, your own routine
    will be executed in available cpu time.<br>
    
5. Three different ways to play music:<br>
a. play a song with its name by calling the function player.playOne(songfile);<br>
b. add some songs into the default empty playlist(player.addToPlaylist());<br>
c. play all the songs in SD card root dir by calling the function player.scanAndPlayAll().<br>
    
### Notice PLS:
a. Libs dependency: SD lib, SPI lib;<br>
b. TIMER1 is used;<br>
c. As the player need handling some control event(e.g.: key control, playlist processing ) circularly,
   you should call player.play() in the arduino loop function. Otherwise, the player will not play;<br>
d. ALL THE SONGS TO PLAY SHOULD BE IN THE ROOT DIRECTORY AND SONG NAME CAN NOT CONTAIN WIDE CHAR;<br>
e. setup APIs can only be called in setup() function.<br>

### Control:
short press - toggle play and pause<br>
long press (more than 2 seconds)  - stop <br>
long press in stop mode - record
short press in record mode - stop recording<br>
short press left in play mode - shift to previous song<br>
long press left in play mode - fast rewind<br>
short press right in play mode - shift to next song<br>
long press right in play mode - fast forward<br>
short press up - volume plus 6<br>
press up and hold - volume up continuously<br>
short press up - volume minus 6<br>
press down and hold - volume down continuously<br>

Default setting:<br>
play mode = NORMAL play<br>
keys      = ENBALE	  <br>
scan available digit pins = DISABLE	 <br>
scan available analog pins= DISABLE <br>
create an empty playlist<br>

### API:
    
    -- global variables --
    MusicPlayer player                          -- MusicPlayer class instance
    
    -- setup APIs --
    void        MusicPlayer::begin(void)        -- Music player initialization routine
    void        MusicPlayer::playOne(char *songName)
                                                -- Add a song to play list , equivalent to call addToPlaylist once
    boolean     addToPlaylist(char *songName)   -- Add a song to play list
    void        scanAndPlayAll(void)            -- Scan all the songs in TF card and add them to play list
    void        attachDigitOperation(int pinNum, void (*userFunc)(void), int mode)
                                                -- attach a callback to digital pin event
    void        attachAnalogOperation(int pinNum, void (*userFunc)(void))
                                                -- attach a callback to analog pin event
    
    -- config APIs --
    void        setVolume(unsigned char volume) -- setup volume (range: 0x00 - 0xfe )
    void        adjustVolume(boolean UpOrDown, unsigned char NumSteps = 6)
                                                -- adjust volume up or down by step (UpOrDown=true for up)
    void        setPlayMode(playMode_t playmode)-- set play mode (modes: refer to keywords.txt Constants)
    boolean     deleteSong(char *songName)      -- delete a song from play list
    void        keyEnable(void)                 
    void        keyDisable(void)
    void        analogControlEnable(void)
    void        digitalControlEnable(void)
  
    -- operate APIs --
    void        play(void)
    void        opPlay(void)  
    void        opPause(void) 
    void        opResume(void)
    void        opStop(void)  
    void        opVolumeUp(void)  
    void        opVolumeDown(void)  
    void        opNextSong(void)    
    void        opPreviousSong(void)
    void        opFastForward(void)
    void        opFastRewind(void) 




For more information, please refer to [wiki page](http://www.seeedstudio.com/wiki/Music_Shield_V2.0).

    
----

This software is written by Jack Shao (jacky.shaoxg@gmail.com) for seeed studio<br>
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>




[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Music_Shield)](https://github.com/igrigorik/ga-beacon)


