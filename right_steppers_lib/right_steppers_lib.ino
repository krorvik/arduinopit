#include <AH_EasyDriver.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// START CONFIG SECTION

// Directions for clarity
// Please note that swapping motor drive cables around can reverse this. 
// Swap values here if you have your motors wired the other way...
#define UP true
#define DOWN false
 
#define MICROSTEPS 8

const unsigned int STP_HZ  = 800 * MICROSTEPS;  // 900 seems magic....
const unsigned int STP_RES_FULL = 720;
const unsigned int STP_RES = STP_RES_FULL * MICROSTEPS;

// Make sure these arrays are all the same size.
// stepper #                           1     2    3    4    5    6    7    8   9    10 
/*
const unsigned int max_step_map[] = {630,  630, 630, 630, 630, 630, 630, 630, 630, 630};
const unsigned int dir_pins[] =     {  4,    6,   8,  10,  12,  14,  16,  18,  20,  22};
const unsigned int step_pins[] =    {  5,    7,   9,  11,  13,  15,  17,  19,  21,  23};
unsigned int current_positions[] =          {  0,    0,   0,   0,   0,   0,   0,   0,   0,   0};
*/
const unsigned int max_step_map[] = {610 * MICROSTEPS,  510 * MICROSTEPS, 610 * MICROSTEPS, 610 * MICROSTEPS};
const unsigned int dir_pins[] =     {  4,    6,   8,  10};
const unsigned int step_pins[] =    {  5,    7,   9,  11};
unsigned int current_positions[] =  {  0,    0,   0,   0};
unsigned int wanted_positions[] =  {  0,    0,   0,   0};
 
// END CONFIG SECTION

// Calculate how many steppers we have for init
// Assumes all arrays above are the same size. 
const unsigned int num_steppers = sizeof(current_positions) / sizeof(current_positions[0]);

// Array of easydriver objects, init'ed in setup
AH_EasyDriver *steppers[num_steppers];

// Reset function. Set output modes,
// and do a sweep up and down on all steppers if so desired (reset_sweep bool) 
void reset() {
  // Set full steps, all pins low
  for(unsigned int i = 0; i < num_steppers; i++) {
    pinMode(dir_pins[i], OUTPUT);
    pinMode(step_pins[i], OUTPUT);      
    steppers[i]->setSpeedHz((STP_HZ));
    steppers[i]->move(STP_RES, DOWN);    
  }  
}

// Callback, only sets the wanted position in wanted_positions. 
// Actual move will be done on dcs bios metadata end
void set_wanted_dial_position(unsigned int index, unsigned int value) {
  wanted_positions[index] = map(value, 0, 65535, 0, max_step_map[index]);
}

void set_dial_position(unsigned int index) {
  bool direction;
  unsigned int diff = 0;
  // Don't move if not needed ;)
  if (wanted_positions[index] == current_positions[index])  { return; }
  // Determine direction and positive diff
  else if (wanted_positions[index] > current_positions[index]) { 
    direction = UP;
    diff = abs(wanted_positions[index] - current_positions[index]);
  }
  else if (wanted_positions[index] < current_positions[index]) { 
    direction = DOWN; 
    diff = abs(current_positions[index] - wanted_positions[index]);
  }
  // Move needle
  steppers[index]->move(diff, direction);
  // And remember position
  current_positions[index] = wanted_positions[index];
}

// DCS bios part
void onEngineOilPressureChange(unsigned int newValue) { set_wanted_dial_position(0, newValue); }
void onEngineNozzlePositionChange(unsigned int newValue) { set_wanted_dial_position(1, newValue); }
void onEngineTachometerChange(unsigned int newValue) { set_wanted_dial_position(2, newValue); }
void onEngineFtitChange(unsigned int newValue) { set_wanted_dial_position(3, newValue);}
/*void onFuelAlChange(unsigned int newValue) { set_wanted_dial_position(4, newValue); }
void onFuelFrChange(unsigned int newValue) { set_wanted_dial_position(5, newValue); }
void onHydrazinVolumeChange(unsigned int newValue) { set_wanted_dial_position(6, newValue); }
void onSysaPressureChange(unsigned int newValue) { set_wanted_dial_position(7, newValue); }
void onSysbPressureChange(unsigned int newValue) { set_wanted_dial_position(8, newValue); }
void onCockpitAlititudeChange(unsigned int newValue) { set_wanted_dial_position(9, newValue); }
*/
DcsBios::IntegerBuffer engineOilPressureBuffer(0x44cc, 0xffff, 0, onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44ce, 0xffff, 0, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer(0x44d0, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44d2, 0xffff, 0, onEngineFtitChange);
/*
DcsBios::IntegerBuffer fuelAlBuffer(0x44dc, 0xffff, 0, onFuelAlChange);
DcsBios::IntegerBuffer fuelFrBuffer(0x44de, 0xffff, 0, onFuelFrChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(0x44d4, 0xffff, 0, onHydrazinVolumeChange);
DcsBios::IntegerBuffer sysaPressureBuffer(0x44c8, 0xffff, 0, onSysaPressureChange);
DcsBios::IntegerBuffer sysbPressureBuffer(0x44ca, 0xffff, 0, onSysbPressureChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(0x44e6, 0xffff, 0, onCockpitAlititudeChange);
*/

/* Stuff to fire on each loop() end below */
void onUpdateCounterChange(unsigned int newValue) { 
  for(unsigned int i = 0; i < num_steppers; i++) {
    set_dial_position(i);
  }
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);
// END DCS bios part

void setup() {
  // create stepper objects
  for (unsigned int i=0; i < num_steppers; i++) {
    steppers[i] = new AH_EasyDriver(STP_RES, dir_pins[i], step_pins[i]);
    steppers[i]->setSpeedHz(STP_HZ);
  }
  reset();
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
