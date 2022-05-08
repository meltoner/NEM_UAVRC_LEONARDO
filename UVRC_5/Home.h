/*
  Home.h - Library for wraping the functions controling the Homenometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Home_h
#define Home_h

#include "Arduino.h" 
#include "Context.h"

class Home{
  public:
    Home(int pin);
    void setup(Context &_context);
    void apply();
    void bursts();
  private:
    Context *context;
};

#endif