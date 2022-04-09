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
    int _length = 5;
    int intervals[5] = {10, 50, 100, 500, 5000};
    unsigned long now = millis();
    unsigned long timers[5] = {now, now, now, now, now}; 
};

#endif