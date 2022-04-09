/*
  Steer.h - Library for wraping the functions controling Steer
  Created Konstantinos Papageorgiou  
*/
#ifndef Steer_h
#define Steer_h

#include "Arduino.h"
#include <Servo.h>

class Steer{
  public:
    Steer(int pin);
    void setup();
    void apply(float sensors[], byte ext_sensors[], int current_degree);
    void setSteer(int value);
    boolean hasTarget(byte ext_sensors[]);
    int getDegreeDiff();
    boolean isSwitchCHalf(byte ext_sensors[]);
    boolean hasNewDegree(byte ext_sensors[]);

    int center = 94;  
    int steerValue = center;

    int target = 0; //vehicle target heading
    int degree = 0; //vehicle heading

  private:
    int _pin;
    Servo steer;
};

#endif