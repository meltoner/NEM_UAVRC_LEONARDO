/*
  Throttle.h - Library for wraping the functions controling the Throttlenometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Throttle_h
#define Throttle_h

#include "Arduino.h"
#include <Servo.h>

class Throttle{
  public:
    Throttle(int pin);
    void setup();
    void apply(float sensors[], byte ext_sensors[]);
    void setThrottle(int value, int limiter);
    int transferFunction(int value, int theshold, int add, int divider);
    int throttleValue = 0;
  private:
    Servo throttle;
    int _pin;
};

#endif