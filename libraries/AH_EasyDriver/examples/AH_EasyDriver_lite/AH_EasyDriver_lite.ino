#include <AH_EasyDriver.h>

//AH_EasyDriver(int RES, int DIR, int STEP);
// RES -> RESOLUTION per full revolve
// DIR -> DIRECTION PIN
// STEP -> STEPPING PIN
AH_EasyDriver stepper(200,2,3);   // Initialisation

void setup() {
//  Serial.begin(57600);
  stepper.setSpeedRPM(100);          // RPM , rotations per minute
//stepper.setSpeedHz(100);           // Hz, steps per second
}

void loop() {

  stepper.move(1000);             // move 1000 steps
  delay(1000);

  stepper.move(-1000);            // move 1000 steps
  delay(1000);
  
  stepper.revolve(2.0);             // revolve 2 times
  delay(1000);
  
  stepper.rotate(180.0);            // rotate 180° 
  delay(1000);  
  
  
}


