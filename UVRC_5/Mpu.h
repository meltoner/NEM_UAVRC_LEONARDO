/*
  Mpu.h - Library for wraping the functions controling the Mpunometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mpu_h
#define Mpu_h

#include "Arduino.h"
#include <Wire.h>
#include <MPU6050_light.h>

class Mpu{
  public:
    Mpu(int pin);
    void setup(float sensors[]);
    void apply(float sensors[]);
    void update();
    int degree = 0;
  private:
    int _pin;
};

#endif
