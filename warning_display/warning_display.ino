#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include <DcsBios.h>

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

// PWM outputs for int lighting controls
void set_primary_consoles_brightness(unsigned int value) { analogWrite(2, map(value, 0, 65535, 0, 255)); }
void set_primary_instrpanel_brightness(unsigned int value) { analogWrite(3, map(value, 0, 65535, 0, 255)); }
void set_flood_consoles_brightness(unsigned int value) { analogWrite(4, map(value, 0, 65535, 0, 255)); }
void set_flood_instrpanel_brightness(unsigned int value) { analogWrite(5, map(value, 0, 65535, 0, 255)); }
void onSysbPressureChange(unsigned int newValue) {hyd_b = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onHydrazinVolumeChange(unsigned int newValue) { epu = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onCockpitAlititudeChange(unsigned int newValue) {oxy = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onUpdateCounterChange(unsigned int newValue) { update(); }
// Register callback functions
DcsBios::IntegerBuffer callback_primary_consoles_brightness(0x440e, 0xffff, 0x0, set_primary_consoles_brightness);
DcsBios::IntegerBuffer callback_primary_instrpanel_brightness(0x4410, 0xffff, 0x0, set_primary_instrpanel_brightness);
DcsBios::IntegerBuffer callback_flood_consoles_brightness(0x4414, 0xffff, 0x0, set_flood_consoles_brightness);
DcsBios::IntegerBuffer callback_flood_instrpanel_brightness(0x4416, 0xffff, 0x0, set_flood_instrpanel_brightness);
DcsBios::IntegerBuffer sysbPressureBuffer(0x44ca, 0xffff, 0, onSysbPressureChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(0x44d4, 0xffff, 0, onHydrazinVolumeChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(0x44e6, 0xffff, 0, onCockpitAlititudeChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

// All controls are named as they are in the dcs-bios F-16C module:
// https://github.com/dcs-bios/module-f-16c-50/blob/master/F-16C_50.json

// Caution panel LEDs
DcsBios::LED LIGHT_AFT_FUEL_LOW ( 0x4472 ,  0x80 , 36);
DcsBios::LED LIGHT_ANTI_SKID ( 0x4470 ,  0x40 , 43);
DcsBios::LED LIGHT_ATF_NOT ( 0x4470 ,  0x8000 , 32);
DcsBios::LED LIGHT_AVIONICS_FAULT ( 0x446e ,  0x2000 , 23);
DcsBios::LED LIGHT_BUC ( 0x4472 ,  0x10 , 50);
DcsBios::LED LIGHT_CABIN_PRESS ( 0x4472 ,  0x4 , 49);
DcsBios::LED LIGHT_CADC ( 0x4470 ,  0x80 , 28);
//DcsBios::LED LIGHT_CAUTION_1 ( 0x4472 ,  0x2 , 52);
DcsBios::LED LIGHT_CAUTION_2 ( 0x4472 ,  0x20 , 33);
DcsBios::LED LIGHT_CAUTION_3 ( 0x4472 ,  0x40 , 35);
DcsBios::LED LIGHT_CAUTION_4 ( 0x4472 ,  0x100 , 37);
//DcsBios::LED LIGHT_CAUTION_5 ( 0x4472 ,  0x200 , 51);
//DcsBios::LED LIGHT_CAUTION_6 ( 0x4472 ,  0x400 , 53);
DcsBios::LED LIGHT_EEC ( 0x4472 ,  0x1 , 48);
DcsBios::LED LIGHT_ELEC_SYS ( 0x446e ,  0x8000 , 24);
DcsBios::LED LIGHT_ENGINE_FAULT ( 0x446e ,  0x1000 , 38);
DcsBios::LED LIGHT_EQUIP_HOT ( 0x4470 ,  0x2 , 25);
DcsBios::LED LIGHT_FLCS_FAULT ( 0x446e ,  0x800 , 22);
DcsBios::LED LIGHT_FUEL_OIL_HOT ( 0x4470 ,  0x10 , 42);
DcsBios::LED LIGHT_FWD_FUEL_LOW ( 0x4472 ,  0x8 , 34);
DcsBios::LED LIGHT_HOOK ( 0x4470 ,  0x400 , 45);
DcsBios::LED LIGHT_IFF ( 0x4470 ,  0x200 , 29);
DcsBios::LED LIGHT_INLET_ICING ( 0x4470 ,  0x100 , 44);
DcsBios::LED LIGHT_NUCLEAR ( 0x4470 ,  0x2000 , 31);
DcsBios::LED LIGHT_NWS_FAIL ( 0x4470 ,  0x4 , 41);
DcsBios::LED LIGHT_OBOGS ( 0x4470 ,  0x4000 , 47);
DcsBios::LED LIGHT_OVERHEAT ( 0x4470 ,  0x1000 , 46);
DcsBios::LED LIGHT_PROBE_HEAT ( 0x4470 ,  0x8 , 26);
DcsBios::LED LIGHT_RADAR_ALT ( 0x4470 ,  0x20 , 27);
DcsBios::LED LIGHT_SEAT_NOT ( 0x446e ,  0x4000 , 39);
DcsBios::LED LIGHT_SEC ( 0x4470 ,  0x1 , 40);
DcsBios::LED LIGHT_STORES_CONFIG ( 0x4470 ,  0x800 , 30);

void initialize_pins() {

  // Init PWM pins
  for (int i = 2; i <= 5; i++) {
    pinMode(i, OUTPUT);
    analogWrite(i, 0);
  }
  // Init LED pins
  for (int i = 22; i < 54; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 22; i < 54; i++) {
    digitalWrite(i, LOW);
  }
}

void setup() {
  initialize_pins();
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
