#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"

// DEFINES
// Stepper data
#define STP_RES_STOP 620
#define MICROSTEPS 8

// Stepper and stepper button pins
#define step_oxy    6
#define step_epu    7
#define step_hyd_b 8
#define dir_oxy 9
#define dir_epu 10
#define dir_hyd_b 11

// CALCULATIONS
// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_STOP * MICROSTEPS;

// DATA
unsigned int oxy = 0; // Needles offset a little - design error
unsigned int epu = 0; // Needles offset a little - design error
unsigned int hyd_b = 0;  // Not offset

// init stepper engine and stepper pointers
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper_oxy = NULL;
FastAccelStepper *stepper_epu = NULL;
FastAccelStepper *stepper_hyd_b = NULL;


// Update all when DCS bios has updated all data
void update() {  
  stepper_oxy->moveTo(oxy);
  stepper_epu->moveTo(epu);
  stepper_hyd_b->moveTo(hyd_b);
}

// DCS bios callback section
void onSysbPressureChange(unsigned int newValue) {hyd_b = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onHydrazinVolumeChange(unsigned int newValue) { epu = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onCockpitAlititudeChange(unsigned int newValue) {oxy = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onUpdateCounterChange(unsigned int newValue) { update(); }

DcsBios::IntegerBuffer sysbPressureBuffer(0x44ca, 0xffff, 0, onSysbPressureChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(0x44d4, 0xffff, 0, onHydrazinVolumeChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(0x44e6, 0xffff, 0, onCockpitAlititudeChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);



void setup() {
  engine.init();
  stepper_oxy = engine.stepperConnectToPin(step_oxy);
  stepper_epu = engine.stepperConnectToPin(step_epu);
  stepper_hyd_b = engine.stepperConnectToPin(step_hyd_b);
  stepper_oxy->setDirectionPin(dir_oxy);
  stepper_epu->setDirectionPin(dir_epu);
  stepper_hyd_b->setDirectionPin(dir_hyd_b);
  stepper_oxy->setSpeedInHz(STP_HZ);
  stepper_epu->setSpeedInHz(STP_HZ);
  stepper_hyd_b->setSpeedInHz(STP_HZ);
  stepper_oxy->setAcceleration(100000);
  stepper_epu->setAcceleration(100000);  
  stepper_hyd_b->setAcceleration(100000);  
  stepper_oxy->move(-STP_RES);
  stepper_epu->move(-STP_RES);
  stepper_hyd_b->move(-STP_RES);
  delay(2000);
  stepper_oxy->setCurrentPosition(0);  
  stepper_epu->setCurrentPosition(0);
  stepper_hyd_b->setCurrentPosition(0);  
  update();
  DcsBios::setup();
}

void loop() {    
  DcsBios::loop();  
}
