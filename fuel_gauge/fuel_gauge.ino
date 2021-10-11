#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "DcsBios.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DEFINES
// Screen data
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1
// Stepper data
#define STP_RES_FULL 720
#define STP_RES_STOP 620
#define MICROSTEPS 8

// Stepper and stepper button pins
#define step_fr    6
#define step_al    7
#define step_hyd_a 8
#define dir_fr 9
#define dir_al 10
#define dir_hyd_a 11

const unsigned int fuel_fr_limit = 4350;
const unsigned int fuel_al_limit = 4450;

// CALCULATIONS
// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// DATA
unsigned int fuel_fr = 300; // Needles offset a little - design error
unsigned int fuel_al = 425; // Needles offset a little - design error
unsigned int hyd_a = 0;  // Not offset
char fuel_totalizer[5] = {'0','0','0','0','0'};

// init Display
Adafruit_SSD1306 display_totalizer(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// init stepper engine and stepper pointers
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper_fr = NULL;
FastAccelStepper *stepper_al = NULL;
FastAccelStepper *stepper_hyd_a = NULL;

// Update the fuel totalizer
void displayFuel() {
  display_totalizer.clearDisplay();
  display_totalizer.setTextSize(3);
  display_totalizer.setTextColor(WHITE);
  display_totalizer.setCursor(25, 0);
  display_totalizer.println(fuel_totalizer);
  display_totalizer.display();
}

// Update all when DCS bios has updated all data
void update() {  
  stepper_fr->moveTo(fuel_fr);
  stepper_al->moveTo(fuel_al);
  stepper_hyd_a->moveTo(hyd_a);
  displayFuel();
}

// DCS bios callback section
void onFuelAlChange(unsigned int newValue) {fuel_al = map(newValue, 0, 65535, 425, fuel_al_limit); } // Needles offset a little - design error
void onFuelFrChange(unsigned int newValue) {fuel_fr = map(newValue, 0, 65535, 300, fuel_fr_limit); } // Needles offset a little - design error
void onFueltotalizer100Change(unsigned int newValue) {fuel_totalizer[2] = (char) (48 + (newValue / 6553));}
void onFueltotalizer1kChange(unsigned int newValue) {fuel_totalizer[1] = (char) (48 + (newValue / 6553));}
void onFueltotalizer10kChange(unsigned int newValue) {fuel_totalizer[0] = (char) (48 + (newValue / 6553));}
void onSysaPressureChange(unsigned int newValue) { hyd_a = map(newValue, 0, 65535, 0, STP_RES_STOP * MICROSTEPS);}
void onUpdateCounterChange(unsigned int newValue) { update(); }

DcsBios::IntegerBuffer fuelAlBuffer(0x44dc, 0xffff, 0, onFuelAlChange);
DcsBios::IntegerBuffer fuelFrBuffer(0x44de, 0xffff, 0, onFuelFrChange);
DcsBios::IntegerBuffer fueltotalizer100Buffer(0x44e4, 0xffff, 0, onFueltotalizer100Change);
DcsBios::IntegerBuffer fueltotalizer1kBuffer(0x44e2, 0xffff, 0, onFueltotalizer1kChange);
DcsBios::IntegerBuffer fueltotalizer10kBuffer(0x44e0, 0xffff, 0, onFueltotalizer10kChange);
DcsBios::IntegerBuffer sysaPressureBuffer(0x44c8, 0xffff, 0, onSysaPressureChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);


void setup() {
  engine.init();
  stepper_fr = engine.stepperConnectToPin(step_fr);
  stepper_al = engine.stepperConnectToPin(step_al);
  stepper_hyd_a = engine.stepperConnectToPin(step_hyd_a);
  stepper_fr->setDirectionPin(dir_fr);
  stepper_al->setDirectionPin(dir_al);
  stepper_hyd_a->setDirectionPin(dir_hyd_a);
  stepper_fr->setSpeedInHz(STP_HZ);
  stepper_al->setSpeedInHz(STP_HZ);
  stepper_hyd_a->setSpeedInHz(STP_HZ);
  stepper_fr->setAcceleration(100000);
  stepper_al->setAcceleration(100000);  
  stepper_hyd_a->setAcceleration(100000);  
  stepper_fr->move(-STP_RES);
  stepper_al->move(-STP_RES);
  stepper_hyd_a->move(-STP_RES);
  delay(2000);
  stepper_fr->setCurrentPosition(0);  
  stepper_al->setCurrentPosition(0);
  stepper_hyd_a->setCurrentPosition(0);
  // Init alt display
  display_totalizer.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  update();
  DcsBios::setup();
}

void loop() {    
  DcsBios::loop();  
}
