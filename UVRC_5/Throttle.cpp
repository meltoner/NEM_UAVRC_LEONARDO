/*
  Throttle.h - Library for wraping the functions controling the Throttlenometer
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Throttle.h"
#include <Servo.h>
#include "Context.h"

Throttle::Throttle(int pin){  
  _pin = pin;
}

void Throttle::setup(Context &_context){
  context = &_context;

  throttle.attach(_pin); 
  throttle.writeMicroseconds(900); // send "stop" signal to ESC. Also necessary to arm the ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal.
  Serial.println("Throttle ready.");
}

void Throttle::apply(){
  setThrottle(transferFunction(context->ext_sensors[2], 15, 20, 300), context->ext_sensors[4]); 
}

void Throttle::setThrottle(int value, int limiter){
  if(value > limiter)
    value = limiter;

  if(throttleValue != value){
    throttleValue = value;
    int pwmVal = map(value, 0, 255, 1000, 2000); // maps to PWM value.
    throttle.writeMicroseconds(pwmVal); // Send signal to ESC.  
  }  
}

int Throttle::transferFunction(int value, int theshold, int add, int divider){
   float x = (float)value;    
    if(x<theshold){
      x = 0;
    }else{
      x = x+add;
      x = (x * (x/divider))+(10/2);
    }
    return (int)x;
}

 
