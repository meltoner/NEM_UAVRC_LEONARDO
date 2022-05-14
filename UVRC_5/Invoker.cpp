/*
  Invoker.h - Library for wraping the functions controling the Invoker
  Created Konstantinos Papageorgiou
  Determines which periodicity function is to be invoked, through time.
*/

#include "Arduino.h"
#include "Invoker.h"


Invoker::Invoker(byte length){  
}

void Invoker::setup(Context &_context){
    context = &_context;
}

int Invoker::apply(){
  context->now = millis();
  for(int i = 0; i < 8; i++)
   if ((context->now - context->timers[i]) > context->intervals[i]) {
    context->timers[i] = context->now;
    return i;
   }  
  return 100;
}
