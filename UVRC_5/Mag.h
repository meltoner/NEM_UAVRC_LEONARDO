/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mag_h
#define Mag_h

#include "Arduino.h"
#include "Context.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Mag{
  public:
    Mag(int pin);
    void setup(Context &_context);
    void apply();
    int pruneDegrees(float value);
    float getOffset();
    void initMagOffset();
    void updateMagOffset();
  private:
    Context *context;
    int _pin;
    Adafruit_HMC5883_Unified mag;
    sensors_event_t event; 
};

#endif