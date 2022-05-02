/*
  Gps.h - Library for wraping the functions controling the Gpsnometer
  Created Konstantinos Papageorgiou  
*/
#ifndef Gps_h
#define Gps_h

#include "Arduino.h" 
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include "Context.h"

class Gps{
  public:
    Gps(int pin);
    void setup(Context &_context);
    void apply();
    void processTarget();    

  private:
    Context *context;
    TinyGPSPlus gps;
    AltSoftSerial gpsPort; //5 ,13    
};

#endif