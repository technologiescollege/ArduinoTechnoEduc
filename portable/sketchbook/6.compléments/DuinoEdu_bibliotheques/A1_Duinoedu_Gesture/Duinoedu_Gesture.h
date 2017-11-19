/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/

#ifndef DUINOEDU_GESTURE_H
#define DUINOEDU_GESTURE_H

#define GES_REACTION_TIME       500 
#define GES_ENTRY_TIME          800 
#define GES_QUIT_TIME           1000

#include <paj7620.h>

class Duinoedu_Gesture
{
  public:
    Duinoedu_Gesture(){;}

    void brancher()
    {paj7620Init();}
    
	
 boolean detecter(char Duinoedu_Gesture_Geste)
    {
      uint8_t data = 0, data1 = 0, error;

      error = paj7620ReadReg(0x43, 1, &data);             // Read Bank_0_Reg_0x43/0x44 for gesture result.

 if (!error) 
 {
    switch (data)                   // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
    {
      case GES_RIGHT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Forward");
          if ((Duinoedu_Gesture_Geste)==('Forward') ) {return true;}else{return false;}
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Backward");
          if ((Duinoedu_Gesture_Geste)==('Backward') ) {return true;}else{return false;}
        }
        else
        {
          Serial.println("Right");
          if ((Duinoedu_Gesture_Geste)==('Right') ) {return true;}else{return false;}
        }          
        break;
      case GES_LEFT_FLAG: 
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Forward");
          if ((Duinoedu_Gesture_Geste)==('Forward') ) {return true;}else{return false;}
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Backward");
          if ((Duinoedu_Gesture_Geste)==('Backward') ) {return true;}else{return false;}
        }
        else
        {
          Serial.println("Left");
          if ((Duinoedu_Gesture_Geste)==('Left') ) {return true;}else{return false;}
        }          
        break;
      case GES_UP_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Forward");
          if ((Duinoedu_Gesture_Geste)==('Forward') ) {return true;}else{return false;}
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Backward");
          if ((Duinoedu_Gesture_Geste)==('Backward') ) {return true;}else{return false;}
        }
        else
        {
          Serial.println("Up");
          if ((Duinoedu_Gesture_Geste)==('Up') ) {return true;}else{return false;}
        }          
        break;
      case GES_DOWN_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Forward");
          if ((Duinoedu_Gesture_Geste)==('Forward') ) {return true;}else{return false;}
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          delay(GES_QUIT_TIME);
          Serial.println("Backward");
          if ((Duinoedu_Gesture_Geste)==('Backward') ) {return true;}else{return false;}
        }
        else
        {
          Serial.println("Down");
          if ((Duinoedu_Gesture_Geste)==('Down') ) {return true;}else{return false;}
        }          
        break;
      case GES_FORWARD_FLAG:
        delay(GES_QUIT_TIME);
        Serial.println("Forward");
        if ((Duinoedu_Gesture_Geste)==('Forward') ) {return true;}else{return false;}
        break;
      case GES_BACKWARD_FLAG:  
        delay(GES_QUIT_TIME);   
        Serial.println("Backward");
        if ((Duinoedu_Gesture_Geste)==('Backward') ) {return true;}else{return false;}
        break;
      case GES_CLOCKWISE_FLAG:
        Serial.println("Clockwise");
        if ((Duinoedu_Gesture_Geste)==('Clockwise') ) {return true;}else{return false;}
        break;
      case GES_COUNT_CLOCKWISE_FLAG:
        Serial.println("anti-clockwise");
        if ((Duinoedu_Gesture_Geste)==('anti-clockwise') ) {return true;}else{return false;}
        break;  
      default:
        paj7620ReadReg(0x44, 1, &data1);
        if (data1 == GES_WAVE_FLAG) 
        {
          Serial.println("wave");
          if ((Duinoedu_Gesture_Geste)==('wave') ) {return true;}else{return false;}
        }
        break;
        }
        return false;
      }
    }	
	
    
};

#endif