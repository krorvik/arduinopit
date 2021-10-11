#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"

// Stepper resolution
#define STP_RES_FULL 720
// Default microstepping easydriver
#define MICROSTEPS 8

// pins
#define RPM_STEP_PIN 7
#define FTIT_STEP_PIN 8
#define NOZ_DIR_PIN 9
#define RPM_DIR_PIN 10
#define FTIT_DIR_PIN 11

// Steppper limits
#define RPM_LIMIT 5760
#define FTIT_LIMIT 5760

// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Stepper engine
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *rpm_stp = NULL;
FastAccelStepper *ftit_stp = NULL;

void init_stepper(FastAccelStepper *stepper, unsigned int step_pin, unsigned int dir_pin) {
  stepper = engine.stepperConnectToPin(step_pin);
  stepper->setDirectionPin(dir_pin);
  stepper->setSpeedInHz(STP_HZ);    
  stepper->setAcceleration(100000);
  stepper->move(-STP_RES);
  stepper->setCurrentPosition(0);
}

void move_stepper(FastAccelStepper *stepper, unsigned int value, int32_t limit) {
  stepper->move(map(value, 0,65535, 0, limit));
}

void onEngineTachometerChange(unsigned int newValue) { move_stepper(rpm_stp, newValue, RPM_LIMIT); }
void onEngineFtitChange(unsigned int newValue) { move_stepper(ftit_stp, newValue, FTIT_LIMIT); }

DcsBios::IntegerBuffer engineTachometerBuffer(0x44d0, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44d2, 0xffff, 0, onEngineFtitChange);

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
//void onUpdateCounterChange(unsigned int newValue) {}
//DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  engine.init();
  init_stepper(rpm_stp, RPM_STEP_PIN, RPM_DIR_PIN);
  init_stepper(ftit_stp, FTIT_STEP_PIN, FTIT_DIR_PIN);
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
