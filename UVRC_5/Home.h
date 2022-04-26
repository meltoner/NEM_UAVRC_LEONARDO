/*
  Home.h - Library for wraping the functions controling the Homenometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Home_h
#define Home_h

#include "Arduino.h" 
#include "Context.h"
#include "Gps.h" 
#include "Remote.h"

class Home{
  public:
    Home(int pin);
    void setup(Context &_context, Remote &_remote, Gps &_gps);
    void apply();
  private:
    Context *context;
    Remote *remote;
    Gps *gps;
};

#endif