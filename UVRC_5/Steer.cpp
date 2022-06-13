/*
  Steer.h - Library for wraping the functions controling the Steer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Steer.h"
#include <Servo.h>
#include "Context.h"

Steer::Steer(byte pin){  
  _pin = pin;
}

void Steer::setup(Context &_context){
  context = &_context;
  steer.attach(_pin);
  on = true;
  setSteer(center);
  //Serial.println(F("Steer ready."));
}

int Steer::getHeadingDifference(){

  if(context->derivatives[2] > 30)
   return 30;

  if(context->derivatives[2] < -30)
   return -30;

  return (int)(context->derivatives[2] * (abs(context->derivatives[2])/30) );
}

void Steer::setSteer(int value){
  if(context->actuators[0] != value){
    applied = context->now;
    steer.write(context->actuators[0] = value);  
  }
}

void Steer::apply(){

  // steering angle derivation
  int sensorValue = map(context->ext_sensors[0], 0, 255, -30, 30);
  
  sensorValue = context->transferFunction(sensorValue, 3, 6, 30);

  //sensorValue = (int)((float)sensorValue * (abs((float)sensorValue)/30));
  int value = center - sensorValue;

  if(context->isSwitchA())
    if(abs(sensorValue) < 5)
      value = center - getHeadingDifference();
    else
      context->targets[0] = context->derivatives[1] + sensorValue*1.2;      
    
  // save power when reached center
  if(context->actuators[0] == value && abs(value - center) < 4 ){
    if(on && (context->now - applied) > 1000  ){    
      steer.detach();
      on = false;      
    }
  }else{
    if(!on){
      steer.attach(_pin);
      on = true;
    }
  }

  if(on)
    setSteer( value );

}

//boolean Steer::hasNewDegree(){
//  if(context->isSwitchB())
//    context->targets[0] = context->derivatives[1];
//}