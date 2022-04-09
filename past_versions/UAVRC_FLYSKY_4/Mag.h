/*
  Mag.h - Library for wraping the functions controling the Magnometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Mag_h
#define Mag_h

#include "Arduino.h"

class Mag{

  public:
    Mag(int pin);
    void setup();
    void apply(float sensors[]);
  private:
    int _pin;
    Adafruit_HMC5883_Unified mag;
    sensors_event_t event; 
};

#endif