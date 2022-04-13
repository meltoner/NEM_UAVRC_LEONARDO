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
    Serial.begin(115200);
}


void Context::apply(){
 //Serial.print(Serial.print(-sensors[6]+mag.zoffset, 2));
 //Serial.print(" ");
 
  for(int i = 0; i < 2; i++){
      Serial.print(latlng[i], 6);
      Serial.print(" ");
  }

  for(int i = 0; i < SENSORS; i++){
      Serial.print(sensors[i], 2);
      Serial.print(" ");
  }

  for(int i = 0; i < EXT_SENSORS; i++){
   Serial.print(ext_sensors[i]);
   Serial.print(" ");
  }

  // Serial.print(throttle.throttleValue);
  // Serial.print(" ");
  // Serial.print(steer.steerValue);
  // Serial.print(" ");
  // Serial.print(steer.target); 
  Serial.println(""); 

}
