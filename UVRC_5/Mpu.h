/*
  Mpu.h - Library for wraping the functions controling the Mpunometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mpu_h
#define Mpu_h

#include "Arduino.h"
#include "Context.h"


class Mpu{
  public:
    Mpu(byte pin);
    void setup(Context &_context);
    void apply();
    void update();
  private:
    Context *context;
    byte _pin;
};

#endif
