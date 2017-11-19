/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/

#ifndef DUINOEDU_COUNT_H
#define DUINOEDU_COUNT_H


class Duinoedu_Count
{
  public:
    Duinoedu_Count(){;}
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


int CountPush(int buttonPin)
{
	buttonState = digitalRead(buttonPin);
	if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
	return ( buttonPushCounter );
}
	
void resetCountPush()
{
	buttonPushCounter = 0;
}	
	
	
 };

#endif