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
  // Update return to home gps target
  if( context->isSwitchC() && context->isGPSLocked ){    
    context->setGPSTarget( context->latlng[0], context->latlng[1]);
    context->targets[0] = context->targets[1];
  }

  context->toHomeActive = context->isSwitchA() && context->isSwitchD() && context->isGPSLocked;
  
  // Derive target heading and speed from gps target
  if(context->toHomeActive){
    context->targets[0] = context->targets[1];

    if(context->targets[2] > 25){

      context->toHomeSpeedWeight = 0.9;
      if(context->toHomeWait){
        context->toHomeSpeedWeight = 0;
        context->intervals[7] = 5000;
      }else{
        context->intervals[7] = 5000;
      }

    }else{
      context->toHomeSpeedWeight = 0.7;
      if(context->toHomeWait){
        context->toHomeSpeedWeight = 0;
        context->intervals[7] = 8000;
      }else{
        context->intervals[7] = 2500;
      }
    }


  }

}

void Home::bursts(){
  context->toHomeWait = !context->toHomeWait;
}