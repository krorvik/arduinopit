#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"

// Stepper resolution
#define STP_RES_FULL 720
// Default microstepping easydriver
#define MICROSTEPS 8

// pins
#define RPM_STEP_PIN 9
#define FTIT_STEP_PIN 10
#define RPM_DIR_PIN 11
#define FTIT_DIR_PIN 12

// Steppper limits
#define RPM_LIMIT 4800
#define FTIT_LIMIT 5000

// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

unsigned int rpm_value = 0;
unsigned int ftit_value = 0;

// Stepper engine
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *rpm_stp = NULL;
FastAccelStepper *ftit_stp = NULL;

void onEngineTachometerChange(unsigned int newValue) { rpm_value = map(newValue, 0, 65536, 0, RPM_LIMIT); }
void onEngineFtitChange(unsigned int newValue) { ftit_value = map(newValue, 0, 65536, 0, FTIT_LIMIT); }

DcsBios::IntegerBuffer engineTachometerBuffer(0x44d0, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44d2, 0xffff, 0, onEngineFtitChange);

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
void onUpdateCounterChange(unsigned int newValue) {
  rpm_stp->moveTo(rpm_value);
  ftit_stp->moveTo(ftit_value);
}

DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);
 
void setup() {
  engine.init();
  rpm_stp = engine.stepperConnectToPin(RPM_STEP_PIN);
  rpm_stp->setDirectionPin(RPM_DIR_PIN);
  rpm_stp->setSpeedInHz(STP_HZ);    
  rpm_stp->setAcceleration(100000);
  rpm_stp->move(-STP_RES);
  delay(2000);
  rpm_stp->setCurrentPosition(0);
  ftit_stp = engine.stepperConnectToPin(FTIT_STEP_PIN);
  ftit_stp->setDirectionPin(FTIT_DIR_PIN);
  ftit_stp->setSpeedInHz(STP_HZ);    
  ftit_stp->setAcceleration(100000);
  ftit_stp->move(-STP_RES);
  delay(2000);
  ftit_stp->setCurrentPosition(0);
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
