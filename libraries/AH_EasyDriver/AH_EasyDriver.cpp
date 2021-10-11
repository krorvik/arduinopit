
/*************************************************************************
**  Device: EasyDriver 4.3                                          	**
**  File:   AH_EasyDriver.h - Stepper motor Driver		    	**
**			  		     				**
**  Created by A. Hinkel 2012-05-05                                 	**
**  Modification         2013-03-21  		   			**
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
*************************************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AH_EasyDriver.h"

//*******************************************************************
//*** INIT ****
//*******************************************************************
AH_EasyDriver::AH_EasyDriver(int RES, int DIR, int STEP)
{ 
  this->INIT_MODE = 0;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;

  pinMode(this->DIR_pin, OUTPUT);
  pinMode(this->STEP_pin, OUTPUT);

  this->STEPPING_FACTOR = 8;
  setSpeedRPM(60);   		// default velocity
  
}

//*******************************************************************
AH_EasyDriver::AH_EasyDriver(int RES, int DIR, int STEP, int MS1, int MS2, int SLP)
{
  this->INIT_MODE = 1;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;
  this->MS1_pin = MS1;
  this->MS2_pin = MS2;
  this->SLP_pin = SLP;

  pinMode(this->DIR_pin,    OUTPUT);
  pinMode(this->STEP_pin,   OUTPUT);
  pinMode(this->MS1_pin,    OUTPUT);
  pinMode(this->MS2_pin,    OUTPUT);
  pinMode(this->SLP_pin,    OUTPUT);

  setMicrostepping(3);   	// 1/8 microstep as default setting
  setSpeedRPM(60);   		// default velocity
  
}

//*******************************************************************
AH_EasyDriver::AH_EasyDriver(int RES, int DIR, int STEP, int MS1, int MS2, int SLP, int ENABLE, int RST)
{
  this->INIT_MODE = 2;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;
  this->MS1_pin = MS1;
  this->MS2_pin = MS2;
  this->SLP_pin = SLP;
  this->ENABLE_pin = ENABLE;
  this->RST_pin = RST;

  pinMode(this->DIR_pin,    OUTPUT);
  pinMode(this->STEP_pin,   OUTPUT);
  pinMode(this->MS1_pin,    OUTPUT);
  pinMode(this->MS2_pin,    OUTPUT);
  pinMode(this->SLP_pin,    OUTPUT);
  pinMode(this->ENABLE_pin, OUTPUT);
  pinMode(this->RST_pin,    OUTPUT);

  setMicrostepping(3);   	// 1/8 microstep as default setting
  setSpeedRPM(60);   		// default velocity
  
}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::resetDriver(){

  if (this->INIT_MODE>1){
    digitalWrite(this->RST_pin, LOW);
    delay(1);
    digitalWrite(this->RST_pin, HIGH);
    delay(1);
  }

}

//*******************************************************************
//*******************************************************************

void AH_EasyDriver::setSpeedRPM(int RPM)
{
  this->STEP_DELAY = 60L*1000000L/(RPM*(this->MOTOR_RESOLUTION)*(this->STEPPING_FACTOR));
}


//*******************************************************************

void AH_EasyDriver::setSpeedHz(int FREQ)
{
  this->STEP_DELAY = 1000000/FREQ;
}


//*******************************************************************
//*******************************************************************

long AH_EasyDriver::getMaxSpeedRPM()
{
  return (60*1000000)/(this->MOTOR_RESOLUTION*this->STEPPING_FACTOR);
}


//*******************************************************************

long AH_EasyDriver::getMaxSpeedHz()
{
  return 1000000;
}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::setDirection(boolean DIRECTION){

  digitalWrite(this->DIR_pin, DIRECTION);
  delayMicroseconds(100);

}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::enableDriver(){

  setEnable(LOW);
}

//*********************************************

void AH_EasyDriver::disableDriver(){

  setEnable(HIGH);
}

//*********************************************

void AH_EasyDriver::setEnable(boolean VALUE){

  if (this->INIT_MODE>0){
    digitalWrite(this->ENABLE_pin, VALUE);
    delayMicroseconds(100);
  }
}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::sleepON(){

   setSleep(LOW);
}

//********************************************

void AH_EasyDriver::sleepOFF(){

   setSleep(HIGH);
}

//********************************************
void AH_EasyDriver::setSleep(boolean VALUE){

  if (this->INIT_MODE>0){
    digitalWrite(this->SLP_pin, VALUE);
    delayMicroseconds(100);

  }

}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::setMicrostepping(int MODE){

  switch (MODE) {
    case 0:  					// 1/1 Vollschritt
      digitalWrite(this->MS1_pin, LOW);
      digitalWrite(this->MS2_pin, LOW);
      this->STEPPING_FACTOR = 1;
      break;
    case 1:					// 1/2 Halbschritt
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, LOW);
      this->STEPPING_FACTOR = 2;
      break;
    case 2:					// 1/4 Viertelschritt
      digitalWrite(this->MS1_pin, LOW);
      digitalWrite(this->MS2_pin, HIGH);
      this->STEPPING_FACTOR = 4;
      break;
    case 3:					// 1/8 Achtelschritt
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, HIGH);
      this->STEPPING_FACTOR = 8;
      break;
    default: 					// 1/8 
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, HIGH);
      this->STEPPING_FACTOR = 8;
  }

}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::move(int NUMBER_OF_STEPS)
{  
  int STEPS = abs(NUMBER_OF_STEPS);  

  if (NUMBER_OF_STEPS > 0) {setDirection(FORWARD); }
  if (NUMBER_OF_STEPS < 0) {setDirection(BACKWARD);}    
 
  while(STEPS > 0) {
   if (micros() - this->LAST_STEP_TIME >= this->STEP_DELAY) {
      this->LAST_STEP_TIME = micros();
      STEPS--;
      stepMotor();
    }
  }

}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::move(int NUMBER_OF_STEPS, boolean DIRECTION)
{  
  int STEPS = abs(NUMBER_OF_STEPS);   
  setDirection(DIRECTION);
   
  while(STEPS > 0) {
    if (micros() - this->LAST_STEP_TIME >= this->STEP_DELAY) {
      this->LAST_STEP_TIME = micros();
      STEPS--;
      stepMotor();
    }
  }
}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::rotate(float DEGREES){

  int steps_to_move = (int)((DEGREES * this->MOTOR_RESOLUTION * this->STEPPING_FACTOR)/360);
  move(steps_to_move);

}


//*******************************************************************
//*******************************************************************

void AH_EasyDriver::revolve(float TIMES){

  int steps_to_move = (int)(TIMES * this->MOTOR_RESOLUTION * this->STEPPING_FACTOR);
  move(steps_to_move);

}

//*******************************************************************
//*******************************************************************

void AH_EasyDriver::stepMotor()
{
    digitalWrite(this->STEP_pin, HIGH);
    digitalWrite(this->STEP_pin, LOW);
}

 