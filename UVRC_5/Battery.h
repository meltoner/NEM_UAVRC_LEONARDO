/*
  Battery.h - Library for wraping the functions controling the Battery power levels
  Created Konstantinos Papageorgiou  
*/
#ifndef Battery_h
#define Battery_h

#include "Arduino.h" 
#include "Context.h"

class Battery{
  public:
    Battery(byte pin);
    void setup(Context &_context);
    void apply();
  private:
    Context *context;
};

#endif