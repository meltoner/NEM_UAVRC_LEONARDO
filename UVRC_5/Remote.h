/*
  Remote.h - Library for wraping the functions controling the Remote control i/o
  Created Konstantinos Papageorgiou  
*/
#ifndef Remote_h
#define Remote_h

#include "Arduino.h"
#include <IBusBM.h>

class Remote{
  public:
    Remote(int pin);
    void setup();
    void apply(byte ext_sensors[]);
    int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue);
  private:
    IBusBM IBus; // IBus object 
};

#endif