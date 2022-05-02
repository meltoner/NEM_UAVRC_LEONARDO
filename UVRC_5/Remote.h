/*
  Remote.h - Library for wraping the functions controling the remote control i/o
  Created Konstantinos Papageorgiou  
*/

#ifndef Remote_h
#define Remote_h

#include "Arduino.h"
#include <IBusBM.h>
#include "Context.h"

class Remote{
  public:
    Remote(int pin);
    void setup(Context &_context);
    void apply();
    int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue);
    Context *context;
  private:
    IBusBM IBus; // IBus object 
};

#endif