/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mag_h
#define Mag_h

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Mag{
  public:
    Mag(int pin);
    void setup(float sensors[]);
    void apply(float sensors[]);
    float zoffset;
  private:
    int _pin;
    Adafruit_HMC5883_Unified mag;
    sensors_event_t event; 
};

#endif