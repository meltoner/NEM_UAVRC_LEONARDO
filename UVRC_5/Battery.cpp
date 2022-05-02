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
  context->voltage =  analogRead(0) / 40.92; 
  // abstract to % based on range 6v to 7.4v    
  context->capacity = (byte)(context->voltage<6 ? 0:(context->voltage - 6) * 71.428);
  
  context->isLowBattery = context->capacity < 30;
  if(context->isLowBattery)
    context->intervals[5] = 100;

}
