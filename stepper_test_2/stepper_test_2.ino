#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"
// Directions for clarity
#define UP HIGH
#define DOWN LOW
// Pulse width sent to easydriver, need microsecond resolution
#define PULSE_WIDTH 500 //microseconds, 500 minimum. Slows down above that. 

// stepper #                     0    1     2    3
unsigned int max_rpm_map[4] = {630,  630, 480, 600};
unsigned int dir_pins[4] =    {  3,   7,   11,  A2};
unsigned int step_pins[4] =   {  4,   8,   12,  A3};
unsigned int ms1_pins[4] =    {  5,   9,   A0,  A4};
unsigned int ms2_pins[4] =    {  6,  10,   A1,  A5};
unsigned int positions[4] =   {  0,   0,    0,   0};

void sweep(unsigned int pin_index, unsigned int direction, unsigned int count) {
  digitalWrite(dir_pins[pin_index], direction);
  for(int i = 0; i < count; i++) {
    digitalWrite(step_pins[pin_index], HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(step_pins[pin_index], LOW);
    delayMicroseconds(PULSE_WIDTH);
  }
}

void reset() {
  // Set full steps, all pins low
  for(int i = 0; i <= 4; i++) {
    pinMode(ms1_pins[i], OUTPUT);
    pinMode(ms2_pins[i], OUTPUT);
    pinMode(dir_pins[i], OUTPUT);
    pinMode(step_pins[i], OUTPUT);
    digitalWrite(ms1_pins[i], LOW);
    digitalWrite(ms2_pins[i], LOW);
  }
  for(int dial = 0; dial < 4; dial++){    
    sweep(dial, DOWN, max_rpm_map[dial]);
    delay(30);
    sweep(dial, UP, 300);
    delay(30);
    sweep(dial, DOWN, 300);
    delay(30);
  }
}

void set_dial(unsigned int index, unsigned int value) {
  unsigned int mapped_value = map(value, 0, 65535, 0, max_rpm_map[index]);
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

void onEngineOilPressureChange(unsigned int newValue) { set_dial(3, newValue); }
void onEngineNozzlePositionChange(unsigned int newValue) { set_dial(2, newValue); }
void onEngineTachometerChange(unsigned int newValue) { set_dial(1, newValue); }
void onEngineFtitChange(unsigned int newValue) { set_dial(0, newValue);}

DcsBios::IntegerBuffer engineOilPressureBuffer(0x44cc, 0xffff, 0, onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44ce, 0xffff, 0, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer(0x44d0, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44d2, 0xffff, 0, onEngineFtitChange);

void setup() {
  reset();
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
