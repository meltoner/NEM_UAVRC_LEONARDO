/*
  Context.h - Library for wraping the functions controling the Context / enviroment information
  Created Konstantinos Papageorgiou  
*/

#ifndef Context_h
#define Context_h

#include "Arduino.h" 

class Context{
  public:
    Context(int pin);
    void setup();
    void apply();
    void reflectSensor(float value, int precission);
    int pruneDegrees(float value);

    boolean isSwitchA();
    boolean isSwitchB();
    boolean isSwitchCHalf();
    boolean isSwitchC();
    boolean isSwitchD();   
    void setGPSTarget(double LAT, double LNG);

    // Enviroment values
    //-----------------------
    // GPS related
    boolean isGPSLocked = false;
    double latlng[2] = {0, 0}; // gps position
    double TARGET_LAT = 37.9584512; //Akropolis
    double TARGET_LNG = 23.7502464;
    
    // Positional related values
    float positional[4] = {0,0,0,0}; // x, y, z, magz
    float derivatives[3] = {0, 0, 0}; // offset, headings,target heading diff
    float targets[3] = {0, 0, 0}; // heading target, target gps degree, target gps distance meters

    // Remote controler
    byte ext_sensors[10] = {0,0,0,0,0,0,0,0,0,0}; // flysky gradient, switches

    // Power related
    float voltage = 0;
    byte capacity = 0;

    // Server and motor values
    byte actuators[2] = {0, 0}; // servo, speed
  
    // Invoker variables - execution functions frequencies
    int intervals[7] = {10, 52, 104, 506, 1008, 2003, 10000};
    unsigned long now = millis();
    unsigned long timers[7] = {now, now, now, now, now, now};

  private:
    int _pin;
};

#endif