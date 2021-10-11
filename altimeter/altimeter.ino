#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "DcsBios.h"
#include "Button2.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Directions for clarity
// Please note that swapping motor drive cables around can reverse this. 
// Swap values here if you have your motors wired the other way...
#define UP true
#define DOWN false
#define MICROSTEPS 8
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const unsigned int STP_RES_FULL = 720;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

#define dirPinStepper    10
#define stepPinStepper   9
#define topButtonPin 7
#define bottomButtonPin 6

// Screen data
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

ESPRotary topEncoder(5,4);
Button2 topButton = Button2(topButtonPin);
Button2 bottomButton = Button2(bottomButtonPin);

//Encoder positions
long topPos = 0;
char baro_digits[] = {'2', '9', '9', '2'};
// Start as if we're on ground
bool wow_nose = true;
bool wow_right = true;
bool wow_left = true;

// altimeter multipliers, long because summung these may result in > 16bit values
int32_t alt_100_steps = 0;
int32_t alt_1k_steps = 0;
int32_t alt_10k_steps = 0;

bool isWow() {
  return wow_nose and wow_left and wow_right;
}

Adafruit_SSD1306 display_baro(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setAlt() {
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

int32_t translateDigit(unsigned int value) {
  if (value < 6553) { return 0;}
  if (value < 13107) { return 1;}
  if (value < 19660) { return 2;}
  if (value < 26214) { return 3;}
  if (value < 32767) { return 4;}
  if (value < 39321) { return 5;}
  if (value < 45874) { return 6;}
  if (value < 52428) { return 7;}
  if (value < 58981) { return 8;}
  return 9;
}

void topClick() {
  if (isWow()) {
    stepper->move(STP_RES / 10);
  }
}

// DCS bios callback section

void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[3] = translateDigit(newValue) + '0'; }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[2] = translateDigit(newValue) + '0'; }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[1] = translateDigit(newValue) + '0'; }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[0] = translateDigit(newValue) + '0'; }
// Weight on wheels is useful information
void onExtWowLeftChange(unsigned int newValue) {wow_left = newValue;}
void onExtWowNoseChange(unsigned int newValue) {wow_nose = newValue;}
void onExtWowRightChange(unsigned int newValue) {wow_right = newValue; }
// Alt pointer, use counters as ref
void onAlt100FtCntChange(unsigned int newValue) {alt_100_steps = (int32_t) map(newValue, 0, 65535, 0, STP_RES); }
void onAlt1000FtCntChange(unsigned int newValue) {alt_1k_steps = translateDigit(newValue) * STP_RES; }
void onAlt10000FtCntChange(unsigned int newValue) {alt_10k_steps = translateDigit(newValue) * STP_RES * 10;}

DcsBios::IntegerBuffer altPressureDrum0CntBuffer(0x448e, 0xffff, 0, onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(0x4490, 0xffff, 0, onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(0x4494, 0xffff, 0, onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(0x4492, 0xffff, 0, onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer alt100FtCntBuffer(0x448c, 0xffff, 0, onAlt100FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer(0x448a, 0xffff, 0, onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer(0x4488, 0xffff, 0, onAlt10000FtCntChange);
DcsBios::IntegerBuffer extWowLeftBuffer(0x4514, 0x0800, 11, onExtWowLeftChange);
DcsBios::IntegerBuffer extWowNoseBuffer(0x4514, 0x0200, 9, onExtWowNoseChange);
DcsBios::IntegerBuffer extWowRightBuffer(0x4514, 0x0400, 10, onExtWowRightChange);
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", 3, 2);

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
void onUpdateCounterChange(unsigned int newValue) { 
  stepper->moveTo((int32_t) (alt_100_steps + alt_1k_steps  + alt_10k_steps));
  displayBaro();
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void displayBaro() {
  display_baro.clearDisplay();
  display_baro.setTextSize(3);
  display_baro.setTextColor(WHITE);
  display_baro.setCursor(25, 0);
  display_baro.println(baro_digits);
  display_baro.display();
}

void setup() {
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  stepper->setDirectionPin(dirPinStepper);
  stepper->setSpeedInHz(STP_HZ);    
  stepper->setAcceleration(100000);
  topPos = topEncoder.getPosition();
  topEncoder.setChangedHandler(setAlt);
  topButton.setClickHandler(topClick);
  // Init alt display
  display_baro.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  displayBaro();
  DcsBios::setup();
}

void loop() {  
  topButton.loop();
  topEncoder.loop();  
  DcsBios::loop();  
}
