#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Stepper.h>

// Global constants
const float STEPS = 720.0;
const float LIMITS = 630.0;
const int STEPPER_COUNT = 1;
const int SPEED = 80;
const float BIOS_MAX = 65535.0;

// Make stepper objects and hookup to pins
/*Stepper stp_SYSA_PRESSURE = Stepper(STEPS, 2, 3, 4, 5);
Stepper stp_SYSB_PRESSURE = Stepper(STEPS, 6, 7, 8, 9);
Stepper stp_HYDRAZIN_VOLUME = Stepper(STEPS, 10, 11, 12, 13);
Stepper stp_COCKPIT_ALTITUDE = Stepper(STEPS, 14, 15, 16, 17);
Stepper stp_ENGINE_OIL_PRESSURE = Stepper(STEPS, 18, 19, 20, 21);
Stepper stp_ENGINE_NOZZLE_POSITION = Stepper(STEPS, 22, 24, 26, 28);*/
//Stepper stp_ENGINE_TACHOMETER = Stepper(STEPS, 30, 32, 34, 36);
//Stepper stp_ENGINE_FTIT = Stepper(STEPS, 38, 40, 42, 44);

// So we can loop them
Stepper steppers[STEPPER_COUNT] = {
/*  
 stp_SYSA_PRESSURE,
  stp_SYSB_PRESSURE,
  stp_HYDRAZIN_VOLUME,
  stp_COCKPIT_ALTITUDE,
  stp_ENGINE_OIL_PRESSURE,
  stp_ENGINE_NOZZLE_POSITION,
  */
  stp_ENGINE_TACHOMETER,
  //stp_ENGINE_FTIT
};

unsigned int positions[STEPPER_COUNT];

void set_stepper(int index, float value) {
  float map_value = value * 630.0 / 65535.0;
  int diff = map_value - positions[index];  
  steppers[index].setSpeed(SPEED);
  steppers[index].step(-diff);
  positions[index] = map_value;
}

// Helpers to connect callbacks to generic function set_stepper
/*void set_SYSA_PRESSURE(unsigned int value) { set_stepper(0, value); }
void set_SYSB_PRESSURE(unsigned int value) { set_stepper(1, value); }
void set_HYDRAZIN_VOLUME(unsigned int value) { set_stepper(2, value); }
void set_COCKPIT_ALTITUDE(unsigned int value) { set_stepper(3, value); }
void set_ENGINE_OIL_PRESSURE(unsigned int value) { set_stepper(4, value); }
void set_ENGINE_NOZZLE_POSITION(unsigned int value) { set_stepper(5, value); }*/
void set_ENGINE_TACHOMETER(unsigned int value) { set_stepper(6, value); }
//void set_ENGINE_FTIT(unsigned int value) { set_stepper(7, value); }


// Register callbacks to integerbuffers
/*DcsBios::IntegerBuffer hookup_SYSA_PRESSURE(0x44c8, 0xffff, 0, set_SYSA_PRESSURE);
DcsBios::IntegerBuffer hookup_SYSB_PRESSURE(0x44ca, 0xffff, 0, set_SYSB_PRESSURE);
DcsBios::IntegerBuffer hookup_HYDRAZIN_VOLUME(0x44d4, 0xffff, 0, set_HYDRAZIN_VOLUME);
DcsBios::IntegerBuffer hookup_COCKPIT_ALTITUDE(0x44e6, 0xffff, 0, set_COCKPIT_ALTITUDE);
DcsBios::IntegerBuffer hookup_ENGINE_OIL_PRESSURE(0x44cc, 0xffff, 0, set_ENGINE_OIL_PRESSURE);
DcsBios::IntegerBuffer hookup_ENGINE_NOZZLE_POSITION(0x44ce, 0xffff, 0, set_ENGINE_NOZZLE_POSITION);*/
DcsBios::IntegerBuffer hookup_ENGINE_TACHOMETER(0x44d0, 0xffff, 0, set_ENGINE_TACHOMETER);
//DcsBios::IntegerBuffer hookup_ENGINE_FTIT(0x44d2, 0xffff, 0, set_ENGINE_FTIT);

// Simple init helper
void resetSteppers() {
  for (int i = 0; i < STEPPER_COUNT; i++) {
    steppers[i].setSpeed(SPEED / 2);
    steppers[i].step(LIMITS - 50);
    steppers[i].setSpeed(SPEED / 10);
    steppers[i].step(50);
    steppers[i].setSpeed(SPEED);
    positions[i] = 0;
  }
}

void setup() {
  resetSteppers();
  DcsBios::setup();
}


void loop() {  
  DcsBios::loop();
}
