# NEM Unmanned Vehicle Remote controlled - UAVRC

## Software Features :

- Software utilises an MPU, GPS, Servo, ESC, magnetometer and a remote control input, in an arduino leonardo.
- Sensors and accuators are all in classes
- Considers seven interval / frequencies of task execution
- Controls steering based on target 
- Boots esc motor, and applies a non liner transfer function to the speed remote input
- Computes degrees heading at boot time, used as an offcet for the mpu degree heading
- Derives distance and degree from a target gps LAT LOT
- Return to home on signal loss
- pending-Detection of low battery and return to home

## Hardware

Sensors :

- Gps : M8n
- Magmetometer : M8n's HMC5883
- Receiver : flysky F9-ia10b
- MPU : MPU6050
- Voltage sensor 0-25v MH-electronic

Other: 
- Transmitter : flysky FS-I6X
- Arduino Leonardo
- Sparkfun Logic Level Converter Bi-Directional


