/*
  Mpu.h - Library for wraping the functions controling the Mpunometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mpu_h
#define Mpu_h

#include "Arduino.h"
#include <Wire.h>
#include <MPU6050_light.h>
#include "Context.h"

class Mpu{
  public:
    Mpu(int pin);
    void setup(Context &_context);
    void apply();
    void update();
  private:
    Context *context;
    int _pin;
};

#endif
