#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "DcsBios.h"
#include "Button2.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen data
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1
// Stepper data
#define STP_RES_FULL 720
#define MICROSTEPS 8
// Mach limits as guessed by playing and watching dcs bios reference...
#define m15_limit 49000
#define m1_limit 60413
#define on_limit 62782

// Stepper and stepper button pins
#define dirPinStepper    10
#define stepPinStepper   9
#define topButtonPin 7
#define bottomButtonPin 6

// Stepper calculations
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Mach ratios calculated
const unsigned int m1_ratio = (m1_limit - m15_limit) * 2;
const unsigned int on_ratio = (on_limit - m1_limit) * 2;
char mach_chars[] = {'0', '.', '0', '0'};
unsigned int airspeed = 0;

// Top encoder position
long topPos = 0;

// WOW, Start as if we're on ground
bool wow_nose = true;
bool wow_right = true;
bool wow_left = true;

// Display
Adafruit_SSD1306 display_mach(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Stepperengine
FastAccelStepperEngine engine = FastAccelStepperEngine();
// Stepper
FastAccelStepper *stepper = NULL;
// encoders and buttons
ESPRotary topEncoder(2,3);
Button2 topButton = Button2(topButtonPin);
Button2 bottomButton = Button2(bottomButtonPin);

float calculateMach(unsigned int value) {  
  if (value >= on_limit) {
    return 0.5;
  } else if (value >= m1_limit) {
    return 0.5 + (on_limit - (float) value) / on_ratio;
  } else {
    return 1 + (m1_limit - (float) value) / m1_ratio;
  }
}

bool isWow() {
  return wow_nose and wow_left and wow_right;
}

void setSpeed() {
  if (isWow()) {
    // read top encoder
    // Read new encoder position
    long newTopPos = topEncoder.getPosition();
    // Get diff
    long diff = newTopPos - topPos;
    // Move needle
    stepper->move(diff * 10);
    // Remember new position
    topPos = newTopPos;
    stepper->setCurrentPosition(0);  
  }
}

void topClick() {
  if (isWow()) {
    stepper->move(STP_RES / 10);
  }
}

// The air speed indicator is not linear, but consists of several ranges where motion is linear. 
// These are translated to real positions here.
int32_t translate_ias(unsigned int value) {
  if (value <  6553) { return (int32_t) map(value,     0,  6553,     0,   460); } // up to 1
  if (value < 13107) { return (int32_t) map(value,  6553, 13107,   460,  2280); } // up to 2
  if (value < 16625) { return (int32_t) map(value, 13107, 16625,  2280,  2790); } // up to 2.5
  if (value < 19797) { return (int32_t) map(value, 16625, 19797,  2790,  3140); } // up to 3
  if (value < 26071) { return (int32_t) map(value, 19797, 26071,  3140,  3750); } // up to 4
  if (value < 32975) { return (int32_t) map(value, 26071, 32975,  3750,  4210); } // up to 5
  if (value < 39647) { return (int32_t) map(value, 32975, 39647,  4210,  4620); } // up to 6
  return (int32_t) map(value, 39647, 58981,  4620,  5760); // up to 9,
}

// DCS bios callback section
// Weight on wheels is useful information
void onExtWowLeftChange(unsigned int newValue) {wow_left = newValue;}
void onExtWowNoseChange(unsigned int newValue) {wow_nose = newValue;}
void onExtWowRightChange(unsigned int newValue) {wow_right = newValue; }
void onAirspeedChange(unsigned int newValue) {airspeed = newValue; }

void onMachIndicatorChange(unsigned int newValue) {
  dtostrf(calculateMach(newValue), 4, 2, mach_chars);
}

DcsBios::IntegerBuffer extWowLeftBuffer(0x4514, 0x0800, 11, onExtWowLeftChange);
DcsBios::IntegerBuffer extWowNoseBuffer(0x4514, 0x0200, 9, onExtWowNoseChange);
DcsBios::IntegerBuffer extWowRightBuffer(0x4514, 0x0400, 10, onExtWowRightChange);
DcsBios::IntegerBuffer airspeedBuffer(0x4498, 0xffff, 0, onAirspeedChange);
DcsBios::IntegerBuffer machIndicatorBuffer(0x449e, 0xffff, 0, onMachIndicatorChange);



// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
// Discard the value here, not needed.
void onUpdateCounterChange(unsigned int newValue) {
  stepper->moveTo(translate_ias(airspeed));
  displaymach();
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void displaymach() {
  display_mach.clearDisplay();
  display_mach.setTextSize(3);
  display_mach.setTextColor(WHITE);
  display_mach.setCursor(25, 0);
  display_mach.println(mach_chars);  
  display_mach.display();
}

void setup() {
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  stepper->setDirectionPin(dirPinStepper);
  stepper->setSpeedInHz(STP_HZ);    
  stepper->setAcceleration(100000);
  stepper->setCurrentPosition(0);
  topPos = topEncoder.getPosition();
  topEncoder.setChangedHandler(setSpeed);
  topButton.setClickHandler(topClick);
  // Init alt display
  display_mach.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  displaymach();
  DcsBios::setup();
}

void loop() {  
  topButton.loop();
  topEncoder.loop();  
  DcsBios::loop();  
}
