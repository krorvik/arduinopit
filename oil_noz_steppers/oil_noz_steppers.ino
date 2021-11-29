#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"

// Stepper resolution
#define STP_RES_FULL 720
// Default microstepping easydriver
#define MICROSTEPS 8

// pins
#define OIL_STEP_PIN 9
#define NOZ_STEP_PIN 10
#define OIL_DIR_PIN 11
#define NOZ_DIR_PIN 12

// Steppper limits
#define OIL_LIMIT 4800
#define NOZ_LIMIT 3800

// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

unsigned int oil_value = 0;
unsigned int noz_value = 0;

// Stepper engine
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *oil_stp = NULL;
FastAccelStepper *noz_stp = NULL;

void onEngineOilPressureChange(unsigned int newValue) { oil_value = map(newValue, 0, 65535, 0, OIL_LIMIT); }
void onEngineNozzlePositionChange(unsigned int newValue) { noz_value = map(newValue, 0, 65535, 0, NOZ_LIMIT); }
DcsBios::IntegerBuffer engineOilPressureBuffer(0x44cc, 0xffff, 0, onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44ce, 0xffff, 0, onEngineNozzlePositionChange);

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
void onUpdateCounterChange(unsigned int newValue) {
  oil_stp->moveTo(oil_value);
  noz_stp->moveTo(noz_value);
}

DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);
 
void setup() {
  engine.init();
  oil_stp = engine.stepperConnectToPin(OIL_STEP_PIN);
  oil_stp->setDirectionPin(OIL_DIR_PIN);
  oil_stp->setSpeedInHz(STP_HZ);    
  oil_stp->setAcceleration(100000);
  oil_stp->move(-STP_RES);
  delay(2000);
  oil_stp->setCurrentPosition(0);
  noz_stp = engine.stepperConnectToPin(NOZ_STEP_PIN);
  noz_stp->setDirectionPin(NOZ_DIR_PIN);
  noz_stp->setSpeedInHz(STP_HZ);    
  noz_stp->setAcceleration(100000);
  noz_stp->move(-STP_RES);
  delay(2000);
  noz_stp->setCurrentPosition(0);

  // Sweep both
  oil_stp->moveTo(OIL_LIMIT);
  noz_stp->moveTo(NOZ_LIMIT);

  delay(2000);

  oil_stp->moveTo(0);
  noz_stp->moveTo(0);
  
  DcsBios::setup();
}

void loop() {  
  DcsBios::loop();
}
