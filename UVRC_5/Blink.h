/*
  Blink.h - Library for wraping the functions controling the Blink 
  Created Konstantinos Papageorgiou  
*/
#ifndef Blink_h
#define Blink_h

#include "Arduino.h" 
#include "Context.h"

class Blink{
  public:
    Blink(int pin);
    void setup(Context &_context);
    void apply();
  private:
    Context *context;
};

#endif