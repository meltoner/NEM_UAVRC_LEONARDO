/*
  Invoker.h - Library for wraping the functions controling the Invoker
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Invoker.h"

Invoker::Invoker(int length){  
}

void Invoker::setup(){
}

int Invoker::apply(){
  now = millis();
  for(int i = 0; i < _length; i++){
   if ((now - timers[i]) > intervals[i]) {
    timers[i] = now;
    return i;
   }   
  }
  return 100;
}
