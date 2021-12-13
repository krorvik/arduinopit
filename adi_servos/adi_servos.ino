#include <Adafruit_PWMServoDriver.h>
#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

// servo positions
const unsigned int flag_servos_hidden[4] = {360, 365, 340, 335};
const unsigned int flag_servos_visible[4] = {220, 530, 180, 500};

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

// DCS bios callbacks
//Servos/flags:
void onAdiLocFlagChange(unsigned int newValue) {
  servos.setPWM(0, 0, map(newValue, 0, 65535, flag_servos_hidden[0], flag_servos_visible[0]));
}
void onAdiAuxFlagChange(unsigned int newValue) {
  servos.setPWM(1, 0, map(newValue, 0, 65535, flag_servos_hidden[1], flag_servos_visible[1]));
}
void onAdiOffFlagChange(unsigned int newValue) {
  servos.setPWM(2, 0, map(newValue, 0, 65535, flag_servos_hidden[2], flag_servos_visible[2]));
}
void onAdiGsFlagChange(unsigned int newValue)  {
  servos.setPWM(3, 0, map(newValue, 0, 65535, flag_servos_hidden[3], flag_servos_visible[3]));
}

DcsBios::Switch2Pos ehsiMode("EHSI_MODE", A0);
DcsBios::Switch2Pos ehsiHdgSetBtn("EHSI_HDG_SET_BTN", A2);
DcsBios::RotaryEncoder ehsiCrsSetKnb("EHSI_CRS_SET_KNB", "DEC", "INC", 2, 3);
DcsBios::RotaryEncoder ehsiHdgSetKnb("EHSI_HDG_SET_KNB", "DEC", "INC", 4, 5);
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", 6, 7);
DcsBios::IntegerBuffer adiAuxFlagBuffer(0x44b4, 0xffff, 0, onAdiAuxFlagChange);
DcsBios::IntegerBuffer adiGsFlagBuffer(0x44b6, 0xffff, 0, onAdiGsFlagChange);
DcsBios::IntegerBuffer adiLocFlagBuffer(0x44b2, 0xffff, 0, onAdiLocFlagChange);
DcsBios::IntegerBuffer adiOffFlagBuffer(0x44b0, 0xffff, 0, onAdiOffFlagChange);

void setup() {
  servos.begin();
  servos.setPWMFreq(60);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
