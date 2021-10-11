#include <AH_EasyDriver.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// START CONFIG SECTION

// Directions for clarity
// Please note that swapping motor drive cables around can reverse this. 
// Swap them here if you have your motors wired the other way...
#define UP HIGH
#define DOWN LOW
// Pulse width sent to easydriver, need microsecond resolution
// Microseconds, 500 minimum. Slows down above that. 
#define PULSE_WIDTH 500 

// set if gauges should sweep on reset
boolean reset_sweep = true;

// Make sure these arrays are all the same size.
// stepper #                      1     2    3    4    5    6    7    8   9    10 
unsigned int max_step_map[] = {630,  630, 630, 630, 630, 630, 630, 630, 630, 630};
unsigned int dir_pins[] =    {  4,    6,   8,  10,  12,  14,  16,  18,  20,  22};
unsigned int step_pins[] =   {  5,    7,   9,  11,  13,  15,  17,  19,  21,  23};
unsigned int positions[] =   {  0,    0,   0,   0,   0,   0,   0,   0,   0,   0};

// END CONFIG SECTION

// Calculate how many steppers we have for init
// Assumes all arrays above are the same size. 
unsigned int num_steppers = sizeof(positions) / sizeof(positions[0]);

// Sweep one of the motors (pin_index refers to the stepper # in arrays above)
// direction is UP or DOWN
// count is number of steps. 
// Any microstepping is set by jumpers on easydriver
void sweep(unsigned int pin_index, unsigned int direction, unsigned int count) {
  digitalWrite(dir_pins[pin_index], direction);
  for(int i = 0; i < count; i++) {
    digitalWrite(step_pins[pin_index], HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(step_pins[pin_index], LOW);
    delayMicroseconds(PULSE_WIDTH);
  }
}

// Reset function. Set output modes,
// and do a sweep up and down on all steppers if so desired (reset_sweep bool) 
void reset() {
  // Set full steps, all pins low
  for(int i = 0; i <= num_steppers; i++) {
    pinMode(dir_pins[i], OUTPUT);
    pinMode(step_pins[i], OUTPUT);
  }
  if (reset_sweep) {
    for(int dial = 0; dial < num_steppers; dial++){    
      sweep(dial, DOWN, max_step_map[dial]);
      delay(30);
      sweep(dial, UP, max_step_map[dial]);
      delay(30);
      sweep(dial, DOWN, max_step_map[dial]);
      delay(30);
    }
  }
}

void set_dial(unsigned int index, unsigned int value) {
  unsigned int mapped_value = map(value, 0, 65535, 0, max_step_map[index]);
  unsigned int direction;
  unsigned int diff = 0;
  if (mapped_value == positions[index])  { return; }
  else if (mapped_value > positions[index]) { 
    direction = UP;
    diff = abs(mapped_value - positions[index]);
  }
  else if (mapped_value < positions[index]) { 
    direction = DOWN; 
    diff = abs(positions[index] - mapped_value);
  }
  sweep(index, direction, diff);
  positions[index] = mapped_value;
}

void onEngineOilPressureChange(unsigned int newValue) { set_dial(0, newValue); }
void onEngineNozzlePositionChange(unsigned int newValue) { set_dial(1, newValue); }
void onEngineTachometerChange(unsigned int newValue) { set_dial(2, newValue); }
void onEngineFtitChange(unsigned int newValue) { set_dial(3, newValue);}
void onFuelAlChange(unsigned int newValue) { set_dial(4, newValue); }
void onFuelFrChange(unsigned int newValue) { set_dial(5, newValue); }
void onHydrazinVolumeChange(unsigned int newValue) { set_dial(6, newValue); }
void onSysaPressureChange(unsigned int newValue) { set_dial(7, newValue); }
void onSysbPressureChange(unsigned int newValue) { set_dial(8, newValue); }
void onCockpitAlititudeChange(unsigned int newValue) { set_dial(9, newValue); }

DcsBios::IntegerBuffer engineOilPressureBuffer(0x44cc, 0xffff, 0, onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44ce, 0xffff, 0, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer(0x44d0, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44d2, 0xffff, 0, onEngineFtitChange);
DcsBios::IntegerBuffer fuelAlBuffer(0x44dc, 0xffff, 0, onFuelAlChange);
DcsBios::IntegerBuffer fuelFrBuffer(0x44de, 0xffff, 0, onFuelFrChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(0x44d4, 0xffff, 0, onHydrazinVolumeChange);
DcsBios::IntegerBuffer sysaPressureBuffer(0x44c8, 0xffff, 0, onSysaPressureChange);
DcsBios::IntegerBuffer sysbPressureBuffer(0x44ca, 0xffff, 0, onSysbPressureChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(0x44e6, 0xffff, 0, onCockpitAlititudeChange);

void setup() {
  reset();
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
