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
    double latlng[2]={0,0};
    // free, free, distance, degrees, x, y, z, mag // where z matches to degrees
    float sensors[SENSORS] = {0,0,0,0,0,0,0,0};
    byte ext_sensors[EXT_SENSORS] = {0,0,0,0,0,0,0,0,0,0};
 
  private:
    int _pin;
};

#endif