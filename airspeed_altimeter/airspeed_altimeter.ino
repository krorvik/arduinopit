#define DCSBIOS_IRQ_SERIAL
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "DcsBios.h"
#include "Button2.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define I2C_DISPLAY_ADDRESS 0x3c
#define OLED_RESET     -1

// Stepper
#define STP_RES_FULL 720
#define MICROSTEPS 8
const unsigned int STP_HZ  = 800 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Pins, all here for easy reading
const unsigned int altStepPin = 10;
const unsigned int airspeedStepPin = 9;
const unsigned int altDirPin = 12;
const unsigned int airspeedDirPin = 11;

const unsigned int altSetPins[] = {6, 7};
const unsigned int baroSetPins[] = {8, 13};
const unsigned int airspeedSetPins[] = {2, 3};
// const unsigned int fourthSetPins[] = {4,5};
// const unsigned int encButton1Pin = A0
// const unsigned int encButton1Pin = A1
// const unsigned int encButton1Pin = A2
// const unsigned int encButton1Pin = A3

// Useful
bool wow_nose = true;
bool wow_right = true;
bool wow_left = true;

bool isWow() {
  return wow_nose and wow_left and wow_right;
}

Adafruit_SSD1306 display_alt(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
FastAccelStepperEngine engine = FastAccelStepperEngine();

// END COMMON

// ALTIMETER
// Stepper
FastAccelStepper *altStepper = NULL;
// Encoder
ESPRotary altEncoder(altSetPins[0], altSetPins[1]);

long altPos = 0;
int32_t alt_100_steps = 0;
int32_t alt_1k_steps = 0;
int32_t alt_10k_steps = 0;


int16_t alt_100 = 0;
int16_t alt_1k = 0;
int16_t alt_10k = 0;

void setAlt() {
  if (isWow()) {
    // read alt reset encoder
    // Read new encoder position
    long newAltResetPos = altEncoder.getPosition();
    // Get diff
    long diff = newAltResetPos - altPos;
    // Move needle
    altStepper->move(diff * 8);
    // Remember new position
    altPos = newAltResetPos;
  }
}

void displayAlt() {
  display_alt.clearDisplay();
  display_alt.setTextSize(3);
  display_alt.setTextColor(WHITE);
  display_alt.setCursor(20, 7);
  char alt[5];
  sprintf(alt, "%05d", (10000 * alt_10k + 1000 * alt_1k + 100 * alt_100));
  display_alt.println(alt);
  display_alt.display();
}

int32_t translateDigit(unsigned int value) {
  if (value < 6553) {
    return 0;
  }
  if (value < 13107) {
    return 1;
  }
  if (value < 19660) {
    return 2;
  }
  if (value < 26214) {
    return 3;
  }
  if (value < 32767) {
    return 4;
  }
  if (value < 39321) {
    return 5;
  }
  if (value < 45874) {
    return 6;
  }
  if (value < 52428) {
    return 7;
  }
  if (value < 58981) {
    return 8;
  }
  return 9;
}

// AIRSPEED
FastAccelStepper *airspeedStepper = NULL;
ESPRotary airspeedEncoder(airspeedSetPins[0], airspeedSetPins[1]);

long airspeedPos = 0;
unsigned int airspeed = 0;

void setAirspeed() {
  if (isWow()) {
    // read top encoder
    // Read new encoder position
    long newSpeedPos = airspeedEncoder.getPosition();
    // Get diff
    long diff = newSpeedPos - airspeedPos;
    // Move needle
    airspeedStepper->move(diff * 8);
    // Remember new position
    airspeedPos = newSpeedPos;
  }

}

// The air speed indicator is not linear, but consists of several ranges where motion is linear.
// These are translated to real positions here.
int32_t translate_ias(unsigned int value) {
  if (value <  6553) {
    return (int32_t) map(value,     0,  6553,     0,   460);  // up to 1
  }
  if (value < 13107) {
    return (int32_t) map(value,  6553, 13107,   460,  2280);  // up to 2
  }
  if (value < 16625) {
    return (int32_t) map(value, 13107, 16625,  2280,  2790);  // up to 2.5
  }
  if (value < 19797) {
    return (int32_t) map(value, 16625, 19797,  2790,  3140);  // up to 3
  }
  if (value < 26071) {
    return (int32_t) map(value, 19797, 26071,  3140,  3750);  // up to 4
  }
  if (value < 32975) {
    return (int32_t) map(value, 26071, 32975,  3750,  4210);  // up to 5
  }
  if (value < 39647) {
    return (int32_t) map(value, 32975, 39647,  4210,  4620);  // up to 6
  }
  return (int32_t) map(value, 39647, 58981,  4620,  5760); // up to 9,
}

// DCS bios callbacks
// Weight on wheels is useful information
void onExtWowLeftChange(unsigned int newValue) {
  wow_left = newValue;
}
void onExtWowNoseChange(unsigned int newValue) {
  wow_nose = newValue;
}
void onExtWowRightChange(unsigned int newValue) {
  wow_right = newValue;
}
void onAirspeedChange(unsigned int newValue) {
  airspeed = newValue;
}

void onAlt100FtCntChange(unsigned int newValue) {
  alt_100 = translateDigit(newValue);
  alt_100_steps = (int32_t) map(newValue, 0, 65535, 0, STP_RES);

}
void onAlt1000FtCntChange(unsigned int newValue) {
  alt_1k = translateDigit(newValue);
  alt_1k_steps = translateDigit(newValue) * STP_RES;
}
void onAlt10000FtCntChange(unsigned int newValue) {
  alt_10k = translateDigit(newValue);
  alt_10k_steps = translateDigit(newValue) * STP_RES * 10;
}

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
void onUpdateCounterChange(unsigned int newValue) {
  airspeedStepper->moveTo(translate_ias(airspeed));
  altStepper->moveTo((int32_t) (alt_100_steps + alt_1k_steps  + alt_10k_steps));
  displayAlt();
}

DcsBios::IntegerBuffer extWowLeftBuffer(0x4514, 0x0800, 11, onExtWowLeftChange);
DcsBios::IntegerBuffer extWowNoseBuffer(0x4514, 0x0200, 9, onExtWowNoseChange);
DcsBios::IntegerBuffer extWowRightBuffer(0x4514, 0x0400, 10, onExtWowRightChange);
DcsBios::IntegerBuffer alt100FtCntBuffer(0x448c, 0xffff, 0, onAlt100FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer(0x448a, 0xffff, 0, onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer(0x4488, 0xffff, 0, onAlt10000FtCntChange);
DcsBios::IntegerBuffer airspeedBuffer(0x4498, 0xffff, 0, onAirspeedChange);
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", baroSetPins[0], baroSetPins[1]);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  engine.init();

  // Init displays
  Wire.begin();
  display_alt.begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY_ADDRESS);

  altStepper = engine.stepperConnectToPin(altStepPin);
  altStepper->setDirectionPin(altDirPin);
  altStepper->setSpeedInHz(STP_HZ);
  altStepper->setAcceleration(5000);
  altStepper->setCurrentPosition(0);

  airspeedStepper = engine.stepperConnectToPin(airspeedStepPin);
  airspeedStepper->setDirectionPin(airspeedDirPin);
  airspeedStepper->setSpeedInHz(STP_HZ);
  airspeedStepper->setAcceleration(5000);
  airspeedStepper->setCurrentPosition(0);

  altPos = altEncoder.getPosition();
  altEncoder.setChangedHandler(setAlt);

  airspeedPos = airspeedEncoder.getPosition();
  airspeedEncoder.setChangedHandler(setAirspeed);

  displayAlt();
  DcsBios::setup();
}

void loop() {    
  //Loop all encoders if dcs has not started yet
  altEncoder.loop();
  airspeedEncoder.loop();
  DcsBios::loop();
}
