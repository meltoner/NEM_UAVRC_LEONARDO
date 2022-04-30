/*
  Battery.h - Library for wraping the functions controling the Battery 
  Created Konstantinos Papageorgiou  
*/
#ifndef Battery_h
#define Battery_h

#include "Arduino.h" 
#include "Context.h"

class Battery{
  public:
    Battery(int pin);
    void setup(Context &_context);
    void apply();
  private:
    Context *context;
};

#endif