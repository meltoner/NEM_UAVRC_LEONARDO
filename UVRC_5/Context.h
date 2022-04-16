/*
  Context.h - Library for wraping the functions controling the Contextiroment context
  Created Konstantinos Papageorgiou  
*/

#ifndef Context_h
#define Context_h

#include "Arduino.h"

#define SENSORS 8
#define EXT_SENSORS 10

class Context{
  public:
    Context(int pin);
    void setup();
    void apply();

    double latlng[2] = {0, 0}; // gps position
    float positional[4] = {0,0,0,0}; // x, y, z, magz
    float derivatives[2] = {0, 0}; // ofset,headings
    float targets[2] = {0,0}; // degrees difference, distance meters
    byte ext_sensors[EXT_SENSORS] = {0,0,0,0,0,0,0,0,0,0}; // flysky gradient, switches
    byte actuators[2] = {0, 0}; // servo, speed

  private:
    int _pin;
};

#endif