// Konstantinos Papageorgiou - 2022 - kp@rei.gr
// NEMA UVRC 
// ------------
// Software utilises an MPU, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
// Sensors and accuators are all in classes
// Considers five intervals of task execution
// Controls steering
// Boots esc motor, and applies a non liner transfer function to the speed remote input
// Computes degrees heading at boot time, used as an offcet for the mpu degree heading
// Derives distance and degree from a target gps LAT LOT
// ------------
// gps : M8n
// magmetometer : M8n's HMC5883
// remote control: flysky F9-ia10b
// mpu : MPU6050

#include "Mag.h"
#include "Gps.h" 
#include "Mpu.h" 
#include "Steer.h"
#include "Throttle.h"
#include "Remote.h"
#include "Invoker.h"
#include "Context.h"

Mag mag(7);
Gps gps(0);
Mpu mpu(4);
Steer steer(6);
Throttle throttle(9);
Remote remote(0);
Invoker invoker(0);
Context context(0);

//-----------------------------------------

boolean isSwitchCHalf(){
   return ((int)context.ext_sensors[8]) == 127;
}

boolean isSwitchCFull(){
  return context.ext_sensors[8] == 255 ;
}

boolean updateGpsDegreeTarget(){
  if(isSwitchCHalf())
   steer.target = (int)context.sensors[3];

  if(isSwitchCFull() && gps.isLocked ){
    gps.setTarget( (double)context.sensors[0], (double)context.sensors[1]);
  }
}

//-----------------------------------------


void setup() {
  context.setup();
  throttle.setup();
  delay(2000);
  Serial.println("Powering up"); 
  remote.setup();
  mag.setup(context.sensors);
  mpu.setup(context.sensors);  
  gps.setup(context.sensors);
  steer.setup();
  invoker.setup();
  Serial.println("Setup done");
}

void apply_very_fast_invoker(){   
   mag.apply(context.sensors);
   mpu.apply(context.sensors);   
}

void apply_fast_invoker(){
   remote.apply(context.ext_sensors);
   steer.apply(context.sensors, context.ext_sensors, mpu.degree);
   throttle.apply(context.sensors, context.ext_sensors);
}

void apply_invoker(){
  gps.apply(context.sensors);
  steer.hasNewDegree(context.ext_sensors);
  updateGpsDegreeTarget();
}

void apply_slow_invoker(){
  context.apply();
}

void apply_very_slow_invoker(){
}

void run_invoker(int i){
    switch(i){
      case 0: apply_very_fast_invoker(); break;
      case 1: apply_fast_invoker(); break;
      case 2: apply_invoker(); break;
      case 3: apply_slow_invoker(); break;
      case 4: apply_very_slow_invoker(); break;
    }
}

void loop(){
 mpu.update();
 run_invoker(invoker.apply());
}
