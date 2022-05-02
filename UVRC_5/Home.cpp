/*
  Home.h - Library for wraping the functions controling the Return to Home 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Home.h" 
#include "Context.h" 

Home::Home(int pin){}

void Home::setup(Context &_context){
  context = &_context;
}

void Home::apply(){
  // Update home
  if(context->isSwitchC() && context->isGPSLocked ){
    context->intervals[5] = 503;
    context->setGPSTarget( context->latlng[0], context->latlng[1]);
    context->targets[0] = context->targets[1];
  }

  // set gps target heading and speed to target
  if(context->isSwitchA() && context->isSwitchD() && context->isGPSLocked){
    
    context->targets[0] = context->targets[1];

    if(context->targets[2] > 10)
      context->ext_sensors[2] = 80;
    else
      context->ext_sensors[2] = 0;
  }
}
