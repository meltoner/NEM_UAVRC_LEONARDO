// Konstantinos Papageorgiou - 2022 - kp@rei.gr
// Code utilising MPU6050, GPS, Servo
// Considers four time intervalas - 1ms - 10, 50 , 500 and 5000
// Uses Smotthing with dynamic number of points per sensor value
// Able to derive distance and degrees from a target LAT LOT
// Controls steering throttle to achieve 30 degrees roll
// Computes actual degree of the earth, by checking the gps consequent points - when on straight line - to derive the mpu offset from time of start.
// 

#include <Smoothed.h>

#include <Wire.h>
#include <MPU6050_light.h>

#include <Servo.h>

#include <AltSoftSerial.h>
#include <TinyGPS++.h>

MPU6050 mpu(Wire);

Servo steer;

TinyGPSPlus gps;
AltSoftSerial gpsPort; //5 ,13

#define SENSORS 7
// lat, lng, distance, degrees, x, y, z // where z matches to degres
Smoothed <float> sensors[SENSORS];
int samples[] = {3,3,4,4,10,10,10};

unsigned long now = millis();
int intervals[] = {10,50,500,5000};
unsigned long timers[] = {now,now,now,now};

boolean gpsLock = false;

void setupSensorValues(){
   for(int i = 0; i < SENSORS; i++){
    sensors[i].begin(SMOOTHED_AVERAGE, samples[i]);
    sensors[i].add(0.0);
  } 
  Serial.println("Smoothed sensor values ready.");
}

void setupMPU(){
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){
    Serial.println("MPU6050 not detected ... Check your wiring!");
    delay(500);
  } 
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);  
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("MPU ready.");
}

void setupSTEER(){
  steer.attach(6); 
  steer.write(90);
  Serial.println("Steer ready.");
}

void setupGPS(){
  gpsPort.begin(9600);
  Serial.println("GPS ready.");
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  setupSensorValues();  
  setupMPU();
  setupSTEER();
  setupGPS();
  Serial.println("Setup done");
}

void updateMPU() {  
    sensors[4].add(mpu.getAngleX());
    sensors[5].add(mpu.getAngleY());
    sensors[6].add(mpu.getAngleZ());    
}

void updateSteer(){
   int current = 90 - ((int)sensors[6].get()%360);
   steer.write(current);
}

void updateGPS(){
   while (gpsPort.available()){
      gps.encode(gpsPort.read());
   }

   if(!gpsLock && gps.location.lat() != 0.0)
     gpsLock = true;
   
   if(gpsLock){
     sensors[0].add(gps.location.lat());
     sensors[1].add(gps.location.lng());
     returnHome();
   }
}

//-----------------------------------------

double TARGET_LAT = 37.9584512;
double TARGET_LNG = 23.7502464;
  
void returnHome(){    
    double distanceKm =
      gps.distanceBetween(
        sensors[0].get(),
        sensors[1].get(),
        TARGET_LAT,
        TARGET_LNG) / 1000.0;
    
    double courseTo =
      gps.courseTo(
        sensors[0].get(),
        sensors[1].get(),
        TARGET_LAT,
        TARGET_LNG);
    
    sensors[2].add(distanceKm);
    sensors[3].add(courseTo);

}

void run_invoker(){  
  mpu.update();  
  now = millis();
  for(int i = 0; i < 4; i++){
   if ((now - timers[i]) > intervals[i]) {
    timers[i] = now;
    switch(i){
      case 0: apply_very_fast_invoker(); break;
      case 1: apply_fast_invoker(); break;
      case 2: apply_slow_invoker(); break;
      case 3: apply_very_slow_invoker(); break;
    }
   }
  }
}

void printSensors(){
  for(int i = 0; i < SENSORS; i++){
   Serial.print(sensors[i].get(), 4);
   Serial.print(" ");
  }
  Serial.println(" ");
}

void apply_very_fast_invoker(){
   updateMPU();
}

void apply_fast_invoker(){
   updateSteer();
}

void apply_slow_invoker(){
  updateGPS();
  printSensors(); 
}

void apply_very_slow_invoker(){
}

void loop(){ 
 run_invoker();
}
