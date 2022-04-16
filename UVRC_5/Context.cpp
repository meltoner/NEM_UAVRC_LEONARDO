/*
  Context.h - Library for wraping the functions controling the Enviroment context
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Context.h"

Context::Context(int pin){  
  _pin = pin;
}

void Context::setup(){
    Serial.begin(9600);
}


void reflectSensor(float value, int precission){
  Serial.print(value, precission);
  Serial.print(" ");
}

void Context::apply(){
 
 
  reflectSensor(derivatives[1], 0);

  for(int i = 0; i < 4; i++)
    reflectSensor(positional[i], 1);


  reflectSensor(targets[0], 1);
  reflectSensor(targets[1], 1);

  reflectSensor(actuators[0], 0);
  reflectSensor(actuators[1], 0);
  
  reflectSensor(latlng[0], 6);
  reflectSensor(latlng[1], 6);


  for(int i = 0; i < EXT_SENSORS; i++)
    reflectSensor(ext_sensors[i], 0);


  
  Serial.println(""); 

}
