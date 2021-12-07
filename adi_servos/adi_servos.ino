#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Adafruit_PWMServoDriver.h>

// servo positions
const unsigned int flag_servos_hidden[4] = {360, 365, 340, 335};
const unsigned int flag_servos_visible[4] = {220, 530, 180, 500};

// servo data
unsigned int servo_positions[4] = { 
  flag_servos_hidden[0], 
  flag_servos_visible[1], 
  flag_servos_visible[2], 
  flag_servos_hidden[3]
};

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

void setServoPositions() {
  for (int index = 0; index < 4; index++) {
    servos.setPWM(index, 0, servo_positions[index]);
  }
}

void servos_init() {
  servos.begin();
  servos.setPWMFreq(60);
  // servo flags
  servos.setPWM(0, 0, flag_servos_hidden[0]);
  servos.setPWM(1, 0, flag_servos_hidden[1]);
  servos.setPWM(2, 0, flag_servos_hidden[2]);
  servos.setPWM(3, 0, flag_servos_hidden[3]);
}


// DCS bios callbacks
//Servos/flags:
void onAdiLocFlagChange(unsigned int newValue) { servo_positions[0] = map(newValue, 0,65535, flag_servos_hidden[0], flag_servos_visible[0]); }
void onAdiAuxFlagChange(unsigned int newValue) { servo_positions[1] = map(newValue, 0,65535, flag_servos_hidden[1], flag_servos_visible[2]); }
void onAdiOffFlagChange(unsigned int newValue) { servo_positions[2] = map(newValue, 0,65535, flag_servos_hidden[2], flag_servos_visible[3]); }
void onAdiGsFlagChange(unsigned int newValue)  { servo_positions[3] = map(newValue, 0,65535, flag_servos_hidden[3], flag_servos_visible[4]); }

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
// Discard the value here, not needed.
void onUpdateCounterChange(unsigned int newValue) {
  setServoPositions();
}

DcsBios::IntegerBuffer adiAuxFlagBuffer(0x44b4, 0xffff, 0, onAdiAuxFlagChange);
DcsBios::IntegerBuffer adiGsFlagBuffer(0x44b6, 0xffff, 0, onAdiGsFlagChange);
DcsBios::IntegerBuffer adiLocFlagBuffer(0x44b2, 0xffff, 0, onAdiLocFlagChange);
DcsBios::IntegerBuffer adiOffFlagBuffer(0x44b0, 0xffff, 0, onAdiOffFlagChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {  
  servos_init();  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();    
}
