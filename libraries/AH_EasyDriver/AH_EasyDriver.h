
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


#ifndef AH_EasyDriver_h
#define AH_EasyDriver_h



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FORWARD   true
#define BACKWARD  false

class AH_EasyDriver {
  public:
    AH_EasyDriver(int RES, int DIR, int STEP);
    AH_EasyDriver(int RES, int DIR, int STEP, int MS1, int MS2, int SLP);
    AH_EasyDriver(int RES, int DIR, int STEP, int MS1, int MS2, int SLP, int ENABLE, int RST);

    void setSpeedRPM(int RPM);
    void setSpeedHz(int FREQ);
    void setMicrostepping(int MODE);

    void enableDriver();
    void disableDriver();
    void sleepON();
    void sleepOFF();
    void resetDriver();

    void move(int NUMBER_OF_STEPS);
    void move(int NUMBER_OF_STEPS, boolean DIRECTION);
    void rotate(float DEGREES);
    void revolve(float TIMES);

    long getMaxSpeedRPM();
    long getMaxSpeedHz();


  private:
    void stepMotor();
    void setDirection(boolean DIRECTION);
    void setEnable(boolean VALUE);
    void setSleep(boolean VALUE);

    long STEP_DELAY;
    long LAST_STEP_TIME;

    int INIT_MODE;
    int STEP_NUMBER;
    int MOTOR_RESOLUTION;
    int DIRECTION;
    int STEPPING_FACTOR;

    int DIR_pin;
    int STEP_pin;
    int MS1_pin;
    int MS2_pin;
    int SLP_pin;
    int ENABLE_pin;
    int RST_pin;
	

};

#endif

 
