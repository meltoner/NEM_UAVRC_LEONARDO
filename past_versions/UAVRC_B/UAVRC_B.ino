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
int samples[]={3,3,4,4,10,10,10};

long now = millis();

unsigned long timer = 0;
unsigned long lastInvocationTime = 0;
unsigned long invocationDelay = 500;

unsigned long lastFastInvocationTime = 0;
unsigned long fastInvocationDelay = 50;

unsigned long lastVeryFastInvocationTime = 0;
unsigned long veryFastInvocationDelay = 10;

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
  steer.write(0);
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
  mpu.update();
  if((millis()-timer)>10){
    sensors[4].add(mpu.getAngleX());
    sensors[5].add(mpu.getAngleY());
    sensors[6].add(mpu.getAngleZ());
    timer = millis();  
  }
}

void updateSteer(){
   float current = sensors[6].get();
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
     sensors[2].add(gps.altitude.meters());
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
  //updateMPU();
  now = millis();
  
  if ((now - lastFastInvocationTime) > fastInvocationDelay) {
    lastFastInvocationTime = now;
    apply_fast_invoker();    
  }

  if ((now - lastInvocationTime) > invocationDelay) {
    lastInvocationTime = now;
    apply_slow_invoker();    
  }
}

void printSensors(){
  for(int i = 0; i < SENSORS; i++){
   Serial.print(sensors[i].get());
   Serial.print(" ");
  }
  Serial.println(" ");
}

void apply_fast_invoker(){
   updateSteer();
}

void apply_slow_invoker(){
  updateGPS();
  printSensors(); 
}

void loop(){ 
 run_invoker();
}
