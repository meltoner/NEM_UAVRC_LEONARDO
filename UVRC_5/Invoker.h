/*
  Invoker.h - Library for wraping the functions controling the Invoker
  Created Konstantinos Papageorgiou  
*/

#ifndef Invoker_h
#define Invoker_h

#include "Arduino.h"
#include "Context.h"

class Invoker{
  public:
    Invoker(byte length);
    void setup(Context &_context);
    int apply();
  private:
    Context *context;
    
};

#endif