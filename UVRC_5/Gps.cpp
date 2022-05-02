/*
  Gps.h - Library for wraping the functions controling the Gps 
  Created Konstantinos Papageorgiou  
*/

#include "Arduino.h"
#include "Gps.h" 
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include "Context.h"

Gps::Gps(int pin){}

void Gps::setup(Context &_context){
  context = &_context;
  gpsPort.begin(9600);
  Serial.println("Gps ready.");
}

void Gps::apply(){
  while (gpsPort.available())
    gps.encode(gpsPort.read());   

  if(!context->isGPSLocked && gps.location.lat() != 0.0){
   context->intervals[5] = 1003;
   context->isGPSLocked = true;
  }

  if(context->isGPSLocked){
   context->latlng[0] = gps.location.lat();
   context->latlng[1] = gps.location.lng();
   processTarget();
  }

}

void Gps::processTarget(){ 
  context->targets[1] = gps.courseTo(context->latlng[0], context->latlng[1], context->TARGET_LAT, context->TARGET_LNG) ;  
  context->targets[2] = gps.distanceBetween(context->latlng[0], context->latlng[1], context->TARGET_LAT, context->TARGET_LNG) ;  
}