#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include "FastAccelStepper.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define STP_RES_FULL 1020
#define MICROSTEPS 8
// Screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1
#define I2C_DISPLAY_ADDRESS 0x3c


const unsigned int STP_HZ  = 3000 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Pins, all here for easy reading
const unsigned int aoaStepPin = 9;
const unsigned int vviStepPin = 10;
const unsigned int aoaDirPin = 11;
const unsigned int vviDirPin = 12;

// Stepper engine and pointers
FastAccelStepperEngine engine = FastAccelStepperEngine();
Adafruit_SSD1306 display_mach(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Mach limits as guessed by playing and watching dcs bios reference...
#define m15_limit 49000
#define m1_limit 60413
#define on_limit 62782
const unsigned int m1_ratio = (m1_limit - m15_limit) * 2;
const unsigned int on_ratio = (on_limit - m1_limit) * 2;
char mach_chars[] = {'0', '.', '0', '0'};

float calculateMach(unsigned int value) {  
  if (value >= on_limit) {
    return 0.5;
  } else if (value >= m1_limit) {
    return 0.5 + (on_limit - (float) value) / on_ratio;
  } else {
    return 1 + (m1_limit - (float) value) / m1_ratio;
  }
}

void displayMach() {
  display_mach.clearDisplay();
  display_mach.setTextSize(3);
  display_mach.setTextColor(WHITE);
  display_mach.setCursor(25, 0);
  display_mach.println(mach_chars);  
  display_mach.display();
}

FastAccelStepper *aoaStepper = NULL;
FastAccelStepper *vviStepper = NULL;

// These are the stepper motor positions for low/high value limits and zero
// They are dependent on physical installation, and need calibration.
const unsigned int common_off = 0;
const unsigned int common_low = 1000;
const unsigned int vvi_high = STP_RES;
const unsigned int aoa_high = STP_RES + 900;
const unsigned int aoa_zero = 2790;
const unsigned int vvi_zero = 4580;

// These are the DCS positions:
const unsigned int dcs_off = 0;
const unsigned int dcs_low = 7536;
const unsigned int dcs_zero = 32768;
const unsigned int dcs_high = 65535;

// Keepers for index position
unsigned int aoa = 0;
unsigned int vvi = 0;

// DCS callbacks
void onAoaValueChange(unsigned int newValue) {  
  if (newValue == 0) {
    aoa = common_off;
  } else if (newValue < dcs_zero) {
    aoa = map(newValue, dcs_low, dcs_zero, common_low / 5 , aoa_zero);
  } else if (newValue >= dcs_zero) {
    aoa = map(newValue, dcs_zero, dcs_high, aoa_zero, aoa_high);
  }  
}

void onVviChange(unsigned int newValue) {
  if (newValue == 0) {
    vvi = common_off;
  } else if (newValue < dcs_zero) {
    vvi = map(newValue, dcs_low, dcs_zero, common_low, vvi_zero);
  } else if (newValue >= dcs_zero) {
    vvi = map(newValue, dcs_zero, dcs_high, vvi_zero, vvi_high);
  }
}

void onUpdateCounterChange(unsigned int newValue) {
  vviStepper->moveTo(vvi);
  aoaStepper->moveTo(aoa);
  displayMach();
}

void onMachIndicatorChange(unsigned int newValue) { dtostrf(calculateMach(newValue), 4, 2, mach_chars); }

DcsBios::IntegerBuffer aoaValueBuffer(0x4484, 0xffff, 0, onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer(0x44aa, 0xffff, 0, onVviChange);
DcsBios::IntegerBuffer machIndicatorBuffer(0x449e, 0xffff, 0, onMachIndicatorChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

// The usual ;)
void setup() {
  engine.init();
  // Init steppers  
  aoaStepper = engine.stepperConnectToPin(aoaStepPin);
  aoaStepper->setDirectionPin(aoaDirPin);
  aoaStepper->setSpeedInHz(STP_HZ);    
  aoaStepper->setAcceleration(10000);
  
  vviStepper = engine.stepperConnectToPin(vviStepPin);
  vviStepper->setDirectionPin(vviDirPin);
  vviStepper->setSpeedInHz(STP_HZ);    
  vviStepper->setAcceleration(10000);  
  aoaStepper->move(-STP_RES);
  vviStepper->move(-STP_RES);
  delay(4000);
  aoaStepper->move(50);
  vviStepper->move(50);
  delay(1000);
  aoaStepper->setCurrentPosition(0);
  vviStepper->setCurrentPosition(0);
  /*
  aoaStepper->moveTo(1000);
  vviStepper->moveTo(1000);
  delay(2000);
  aoaStepper->moveTo(STP_RES);
  vviStepper->moveTo(STP_RES);
  delay(2000);
  aoaStepper->moveTo(4580);
  vviStepper->moveTo(4580);
  delay(2000);
  aoaStepper->moveTo(2790);
  vviStepper->moveTo(2790);
  delay(2000);
  aoaStepper->moveTo(0);
  vviStepper->moveTo(0);
  */
  display_mach.begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY_ADDRESS);
  displayMach();
  DcsBios::setup();
}

void loop() {    
  DcsBios::loop();  
}
