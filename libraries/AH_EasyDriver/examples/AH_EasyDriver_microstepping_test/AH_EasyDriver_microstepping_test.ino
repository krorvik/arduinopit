#include <AH_EasyDriver.h>

//AH_EasyDriver(int RES, int DIR, int STEP, int MS1, int MS2, int SLP);
AH_EasyDriver stepper(200,2,3,4,5,6);    // init w/o "enable" and "reset" functions

void setup() {
  stepper.setSpeedRPM(100);                  // set speed in RPM, rotations per minute
  stepper.sleepOFF();
  
  Serial.begin(57600);
}

void loop() {

  Serial.println("Full Step");
  stepper.setMicrostepping(0);  
  stepper.rotate(360);
  delay(500);

  Serial.println("Half Step");
  stepper.setMicrostepping(1);  
  stepper.rotate(360);
  delay(500);  

  Serial.println("Quarter Step");
  stepper.setMicrostepping(2);  
  stepper.rotate(360);
  delay(500);  

  Serial.println("Eight Step");
  stepper.setMicrostepping(3);  
  stepper.rotate(360);
  delay(500);  
  
}


