/*
  Home.h - Library for wraping the functions controling the Return to Home 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Home.h" 
#include "Context.h"
#include "Gps.h" 
#include "Remote.h"

Home::Home(int pin){}

void Home::setup(Context &_context, Remote &_remote, Gps &_gps){
  context = &_context;
  remote = &_remote;
  gps = &_gps;
}

void Home::apply(){

  // Update home
  if(remote->isSwitchC() && gps->isLocked ){
    context->intervals[5] = 503;
    gps->setTarget( context->latlng[0], context->latlng[1]);
    context->targets[0] = context->targets[1];
  }
  
  // set gps target heading and speed to target
  if(remote->isSwitchA() && remote->isSwitchD() && gps->isLocked){
    gps->processTarget();
    context->targets[0] = context->targets[1];

    if(context->targets[2] > 10)
      context->ext_sensors[2] = 80;
    else
      context->ext_sensors[2] = 0;
  }

}
