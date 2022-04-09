/*
  Remote.h - Library for wraping the functions Remote control i/o
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Remote.h"
#include <IBusBM.h>

#define EXT_SENSORS 10

Remote::Remote(int pin){
  
}

void Remote::setup(){
  IBus.begin(Serial1);
  delay(200);
  Serial.println("IBUS ready.");
}

void Remote::apply(byte ext_sensors[]){
  for(int i = 0; i < EXT_SENSORS; i++)
   ext_sensors[i] = (byte)readChannel(i, 0, 255, 0);  
}

int Remote::readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){ 
  //https://www.youtube.com/watch?v=JZQoHmLqZYg
  uint16_t ch = IBus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

