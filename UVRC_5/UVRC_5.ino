// Konstantinos Papageorgiou - 2022 - kp@rei.gr
// NEMA UVRC 
// ------------
// Software utilises an MPU6050, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
// Sensors and accuators are all in classes
// Considers five intervals of task execution
// Controls steering
// Boots esc motor, and applies a non liner transfer function to the speed remote input
// Computes degrees heading at boot time, used as an offcet for the mpu degree heading
// Derives distance and degree from a target gps LAT LOT
// ------------

#include "Mag.h"
#include "Gps.h" 
#include "Mpu.h" 
#include "Steer.h"
#include "Throttle.h"
#include "Remote.h"

Mag mag(7);
Gps gps(0);
Mpu mpu(4);
Steer steer(6);
Throttle throttle(9);
Remote remote(0);

// lat, lng, distance, degrees, x, y, z, mag // where z matches to degrees
#define SENSORS 8
#define EXT_SENSORS 10

float sensors[] = {0,0,0,0,0,0,0,0};
byte ext_sensors[] = {0,0,0,0,0,0,0,0,0,0};
 
unsigned long now = millis();
int intervals[] = {10, 50, 100, 500, 5000};
unsigned long timers[] = {now, now, now, now, now};

boolean isSwitchCHalf(){
   return ((int)ext_sensors[8]) == 127;
}

boolean isSwitchCFull(){
  return ext_sensors[8] == 255 ;
}

boolean updateGpsDegreeTarget(){
  if(isSwitchCHalf())
   steer.target = (int)sensors[3];

  if(isSwitchCFull() && gps.isLocked ){
    gps.setTarget( (double)sensors[0], (double)sensors[1]);
  }
}

//-----------------------------------------

void run_invoker(){
  mpu.update();
  now = millis();
  for(int i = 0; i < 4; i++){
   if ((now - timers[i]) > intervals[i]) {
    timers[i] = now;
    switch(i){
      case 0: apply_very_fast_invoker(); break;
      case 1: apply_fast_invoker(); break;
      case 2: apply_invoker(); break;
      case 3: apply_slow_invoker(); break;
      case 4: apply_very_slow_invoker(); break;
    }
   }
  }
}

void printSensors(){
 //Serial.print(Serial.print(-sensors[6]+mag.zoffset, 2));

 Serial.print(" ");
  for(int i = 0; i < SENSORS; i++){
    if(i < 2)
      Serial.print(sensors[i], 6);
    else
      Serial.print(sensors[i], 2);
   Serial.print(" ");
  }
  for(int i = 0; i < EXT_SENSORS; i++){
   Serial.print(ext_sensors[i]);
   Serial.print(" ");
  }

  Serial.print(throttle.throttleValue);
  Serial.print(" ");
  Serial.print(steer.steerValue);
  Serial.print(" ");
  Serial.print(steer.target); 
  Serial.println(""); 
}

void setup() {
  throttle.setup();
  Serial.begin(115200);
  delay(2000);
  Serial.println("Powering up"); 
  remote.setup();
  mag.setup(sensors);
  mpu.setup(sensors);  
  gps.setup(sensors);
  steer.setup();
  Serial.println("Setup done");
}

void apply_very_fast_invoker(){   
   mag.apply(sensors);
   mpu.apply(sensors);   
}

void apply_fast_invoker(){
   remote.apply(ext_sensors);
   steer.apply(sensors, ext_sensors, mpu.degree);
   throttle.apply(sensors, ext_sensors);
}

void apply_invoker(){
  gps.apply(sensors);
  steer.hasNewDegree(ext_sensors);
  updateGpsDegreeTarget();
}
void apply_slow_invoker(){
  printSensors();
}

void apply_very_slow_invoker(){
}

void loop(){ 
 run_invoker();
}
