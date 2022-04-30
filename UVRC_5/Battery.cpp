/*
  Battery.h - Library for wraping the functions controling the Battery power levels
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Battery.h" 
#include "Context.h"

Battery::Battery(int pin){}

void Battery::setup(Context &_context){
  context = &_context;
  pinMode(12, OUTPUT);
  apply();
}

void Battery::apply(){
  context->voltage=((((int)(analogRead(1)/4.092)))/10);
}
