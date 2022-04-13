/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mag_h
#define Mag_h

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "Context.h"

class Mag{
  public:
    Mag(int pin);
    void setup(Context &_context);
    void apply();
    float zoffset;
  private:
    Context *context;
    int _pin;
    Adafruit_HMC5883_Unified mag;
    sensors_event_t event; 
};

#endif