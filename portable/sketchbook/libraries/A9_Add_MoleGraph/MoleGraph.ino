#include "MoleGraph.h"

MoleGraph moleGraph;

void UpdateGraphChannels(void)
{ 
  //NOTE: All callback functions are called from a timer interupt so they should take as short time as possible 
  //to they don't block another interrupts or a graph drawing. 
  moleGraph.SetChannelValue(1, moleGraph.GetChannelValue(1) == 20 ? 0 : moleGraph.GetChannelValue(1) + 1);
  moleGraph.SetChannelValue(2, moleGraph.GetChannelValue(2) / 2);
}

void MeasurementStartedCallback(void)
{
  digitalWrite(13, HIGH);
}

void MeasurementStoppedCallback(void)
{
  digitalWrite(13, LOW);
}

void MeasurementPausedCallback(void)
{
  digitalWrite(13, LOW);
}

void MeasurementContinuedCallback(void)
{
  digitalWrite(13, HIGH);
}

void setup() 
{
  //It must not be here if you use hardware hannels
  for (int i = 0; i < 8; i++)
    moleGraph.SetChannelValue(i,i);

  //initialize moleGraph module
  moleGraph.Setup();

  //Set callback functions which you will use  
  moleGraph.SetSendingCallback(&UpdateGraphChannels);
  moleGraph.SetMeasurementStartedCallback(&MeasurementStartedCallback);
  moleGraph.SetMeasurementStoppedCallback(&MeasurementStoppedCallback);
  moleGraph.SetMeasurementPausedCallback(&MeasurementPausedCallback);
  moleGraph.SetMeasurementContinuedCallback(&MeasurementContinuedCallback);

  //initialize pin 13 for measurement state initialization 
  pinMode(13, OUTPUT);
}

void loop() 
{
  moleGraph.CheckInput();
}
