/*
  Throttle.h - Library for wraping the functions controling the Throttlenometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Throttle_h
#define Throttle_h

#include "Arduino.h"
#include <Servo.h>
#include "Context.h"

class Throttle{
  public:
    Throttle(byte pin);
    void setup(Context &_context);
    void apply();
    void setThrottle(int value, int limiter);
    int transferFunction(int value, int theshold, int add, int divider);
  private:
    Context *context;
    Servo throttle;
    byte _pin;
};

#endif