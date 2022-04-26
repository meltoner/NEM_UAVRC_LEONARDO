/*
  Invoker.h - Library for wraping the functions controling the Invoker
  Created Konstantinos Papageorgiou  
*/

#ifndef Invoker_h
#define Invoker_h

#include "Arduino.h"

class Invoker{
  public:
    Invoker(int length);
    void setup();
    int apply();
  private:
    int _length = 6;
    int intervals[6] = {10, 52, 104, 506, 1008, 10000};
    unsigned long now = millis();
    unsigned long timers[6] = {now, now, now, now, now}; 
};

#endif