/*
  Context.h - Library for wraping the functions controling the Contextiroment context
  Created Konstantinos Papageorgiou  
*/

#ifndef Context_h
#define Context_h

#include "Arduino.h" 

class Context{
  public:
    Context(int pin);
    void setup();
    void apply();
    void reflectSensor(float value, int precission);
    int pruneDegrees(float value);

    double latlng[2] = {0, 0}; // gps position
    float positional[4] = {0,0,0,0}; // x, y, z, magz
    float derivatives[3] = {0, 0, 0}; // ofset, headings,target heading diff
    float targets[3] = {0, 0, 0}; // heading target, target gps degree, target gps distance meters
    byte ext_sensors[10] = {0,0,0,0,0,0,0,0,0,0}; // flysky gradient, switches
    byte actuators[2] = {0, 0}; // servo, speed
 
  private:
    int _pin;
};

#endif