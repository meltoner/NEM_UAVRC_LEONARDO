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

    // Enviroment values
    double latlng[2] = {0, 0}; // gps position
    float positional[4] = {0,0,0,0}; // x, y, z, magz
    float derivatives[3] = {0, 0, 0}; // offset, headings,target heading diff
    float targets[3] = {0, 0, 0}; // heading target, target gps degree, target gps distance meters
    byte ext_sensors[10] = {0,0,0,0,0,0,0,0,0,0}; // flysky gradient, switches
    float voltage = 0;
    byte capacity = 0;
    byte actuators[2] = {0, 0}; // servo, speed
  
    // invoker variables
    int intervals[7] = {10, 52, 104, 506, 1008, 2003, 10000};
    unsigned long now = millis();
    unsigned long timers[7] = {now, now, now, now, now, now};

  private:
    int _pin;
};

#endif