/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/

#ifndef DUINOEDU_TIME_H
#define DUINOEDU_TIME_H


class Duinoedu_Time
{
  public:
    Duinoedu_Time(){;}
int previousMillis = 0;

int Deltat()
{
  return ( millis() - previousMillis );
}
	
void resetDeltat()
{
  previousMillis = millis();
}	
	
	
 };

#endif