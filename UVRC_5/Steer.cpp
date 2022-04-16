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
}

int Steer::getHeadingDifference(){
  if(context->derivatives[2] > 30)
   return 30;
  if(context->derivatives[2] < -30)
   return -30;
  return (int)(context->derivatives[2] * 0.95);
}

void Steer::setSteer(int value){
  if(context->actuators[0] != value)
    steer.write(context->actuators[0] = value);  
}

void Steer::apply(){
   if(remote->isSwitchA())
      setSteer( center - getHeadingDifference() );
   else
      setSteer( center - map(context->ext_sensors[0], 0, 255, -50, 50) );
}

boolean Steer::hasNewDegree(){
  if(remote->isSwitchB())
    context->targets[0] = context->derivatives[1];
}