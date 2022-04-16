/*
  Steer.h - Library for wraping the functions controling the Steer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Steer.h"
#include <Servo.h>
#include "Remote.h"
#include "Context.h"

Steer::Steer(int pin){  
  _pin = pin;
}

void Steer::setup(Context &_context, Remote &_remote){
  context = &_context;
  remote = &_remote;

  steer.attach(_pin); 
  setSteer(center);
  Serial.println("Steer ready.");

  target = context->targets[0];
}

void Steer::setSteer(int value){

  if(context->actuators[0] != value){
    context->actuators[0] = value;
    steer.write(value);  
  }  
}

int Steer::getDegreeDiff(){
     int diff = target - context->derivatives[1];

     if(diff > 180)
      diff = diff -360;

     if(diff < -180)
      diff = diff + 360;
            
     if(diff > 45)
      diff = 45;

     if(diff < -45)
      diff = -45;
    return diff;
}

void Steer::apply(){

   if(remote->isSwitchA()){
      setSteer( center - getDegreeDiff() );
   }else{
      int control = map(context->ext_sensors[0], 0, 255, -50, 50);
      setSteer( center - control );
   }

}

boolean Steer::hasNewDegree(){
//  if(remote->isSwitchA()){
//    int control = map(context->ext_sensors[0], 0, 255, 0, 40) - 20;
//    if(abs(control) > 2 && !(remote->isSwitchCHalf()))
//      target += control / 2;
//  }
  if(remote->isSwitchB())
    target = context->derivatives[1];
}