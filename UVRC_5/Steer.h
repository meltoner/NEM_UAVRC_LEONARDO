/*
  Steer.h - Library for wraping the functions controling Steer
  Created Konstantinos Papageorgiou  
*/
#ifndef Steer_h
#define Steer_h

#include "Arduino.h"
#include <Servo.h>
#include "Context.h"

class Steer{
  public:
    Steer(byte pin);
    void setup(Context &_context);
    void apply();
    void setSteer(int value);
    int getHeadingDifference();
    boolean hasNewDegree();

    int center = 86;
    //int target = 0; //vehicle target heading

  private:
    byte _pin;
    unsigned long applied = 0;
    boolean on = false;
    Context *context;
    Servo steer;
};

#endif