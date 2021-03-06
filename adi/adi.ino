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
#define STP_RES_FULL 200
#define MICROSTEPS 8

const unsigned int STP_HZ  = 200 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Pins, all here for easy reading
const unsigned int pitchStepPin = 9;
const unsigned int bankStepPin = 10;
const unsigned int pitchDirPin = 11;
const unsigned int bankDirPin = 12;

// Other assigmments matching PCB, but must rename
const unsigned int pitchSetPins[] = {2,3};
const unsigned int bankSetPins[] = {4,5};
//const unsigned int thirdSetPins[] = {6,7};
//const unsigned int fourthSetPins[] = {8,13};
const unsigned int startButtonPin = A0;
const unsigned int resetButtonPin = A1;
//const unsigned int button3pin = A2;
//const unsigned int button4pin = A3;

char pneu_char = ' ';
char baro_digits[4] = { '2', '9', '9', '2',};

bool isInitialized = false;

Adafruit_SSD1306 display_baro(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *pitchStepper = NULL;
FastAccelStepper *bankStepper = NULL;
ESPRotary pitchEncoder(pitchSetPins[0], pitchSetPins[1]);
ESPRotary bankEncoder(bankSetPins[0], bankSetPins[1]);
Button2 startButton;
Button2 resetButton;

void displayInit() {
  if (not isInitialized) {       
    display_baro.clearDisplay();
    display_baro.setTextSize(2);
    display_baro.setTextColor(WHITE);
    display_baro.setCursor(0, 6);
    display_baro.println(F("X XXXX"));
    display_baro.display();
  }
}

void displayBaro() {
  if (isInitialized) {
    display_baro.clearDisplay();
    display_baro.setTextSize(2);
    display_baro.setTextColor(WHITE);
    display_baro.setCursor(0, 6);
    display_baro.println(String(pneu_char));
    display_baro.setCursor(40, 6);
    display_baro.println(String(baro_digits).substring(0,4));
    display_baro.display();
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

// This are the raw values for horizontal at startup, 0 is vertical down/full left down, 65535 is vertical up/full right down.
long rawPitch = 32768;
long rawBank = 32768;

// Encoder positions
long pitchEncPos;
long bankEncPos;

// Use these to get a full positional value for stepper,
// Which we want to rotate mulitple in bank dir (not pitch)
long bankRotations = 0;

void onAdiBankChange(unsigned int newValue) {
  // Increasing zero cross?
  if(rawBank > 55535 and newValue < 10000) { bankRotations++; }
  // Decreasing zero cross?
  if(rawBank < 10000 and newValue > 55535) { bankRotations--; }
  rawBank = newValue;
}

void moveBankStepper() {
  if (isInitialized) {
    bankStepper->moveTo(-1 * STP_RES * bankRotations + map(rawBank, 0, 65535, 800, -800));
  }
}

void setBank() {
  if (not isInitialized) {
    long newPos = bankEncoder.getPosition();
    long diff = newPos - bankEncPos;
    bankStepper->move(diff * 2);
    bankEncPos = newPos;    
  }
}

void setPitch() {
  if (not isInitialized) {
    long newPos = pitchEncoder.getPosition();
    long diff = newPos - pitchEncPos;
    pitchStepper->move(diff * 20);
    pitchEncPos = newPos;    
  }
}

void onAdiPitchChange(unsigned int newValue) {
  rawPitch = newValue;
}

void movePitchStepper() {
  if (isInitialized) {
    // Sliiiiightly different scales... ouch.
    if (rawPitch <= 32768) {
      pitchStepper->moveTo(map(rawPitch, 0, 32768, -6000, 0));
    } else {
      pitchStepper->moveTo(map(rawPitch, 32768, 65535, 0, 5500));
    }
  }
}

void startButtonLongClick(Button2 button) {
  if (not isInitialized) {
    isInitialized = true;
    // Reset positions for steppers
    bankStepper->setCurrentPosition(0);
    pitchStepper->setCurrentPosition(0);
  }
}

void resetButtonLongClick(Button2 button) {
  if (isInitialized) {
    // Reinit requested
    isInitialized = false;
    displayInit();
  }
}

// DCS bios callbacks
void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[3] = translateDigit(newValue) + '0'; }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[2] = translateDigit(newValue) + '0'; }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[1] = translateDigit(newValue) + '0'; }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[0] = translateDigit(newValue) + '0'; }
void onAltPneuFlagChange(unsigned int newValue) { if(newValue > 0) { pneu_char = 'P'; } else { pneu_char = ' '; } }

DcsBios::IntegerBuffer adiBankBuffer(0x44ae, 0xffff, 0, onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer(0x44ac, 0xffff, 0, onAdiPitchChange);
DcsBios::IntegerBuffer altPressureDrum0CntBuffer(0x448e, 0xffff, 0, onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(0x4490, 0xffff, 0, onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(0x4494, 0xffff, 0, onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(0x4492, 0xffff, 0, onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer altPneuFlagBuffer(0x4496, 0xffff, 0, onAltPneuFlagChange);

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
// Discard the value here, not needed.
void onUpdateCounterChange(unsigned int newValue) {
  if (isInitialized) {
    moveBankStepper();    
    movePitchStepper();    
  }
}

void steppers_init() {
  engine.init();
  pitchStepper = engine.stepperConnectToPin(pitchStepPin);
  pitchStepper->setDirectionPin(pitchDirPin);
  // Pitchstepper can work on a higher freq, many more steps.
  pitchStepper->setSpeedInHz(STP_HZ * 10);
  // Heavy axes requires acceleration, same for both makes'em smooth
  pitchStepper->setAcceleration(2000);  
  bankStepper = engine.stepperConnectToPin(bankStepPin);
  bankStepper->setDirectionPin(bankDirPin);
  bankStepper->setSpeedInHz(STP_HZ);
  bankStepper->setAcceleration(2000);
}

void display_init() {
  display_baro.begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY_ADDRESS);
  displayInit();
}

void encoders_init() {
  pitchEncPos = pitchEncoder.getPosition();
  pitchEncoder.setChangedHandler(setPitch);
  bankEncPos = bankEncoder.getPosition();
  bankEncoder.setChangedHandler(setBank);
}

void buttons_init() {
  startButton.begin(startButtonPin);
  startButton.setLongClickTime(2000);
  startButton.setLongClickDetectedHandler(startButtonLongClick);

  resetButton.begin(resetButtonPin);
  resetButton.setLongClickTime(2000);
  resetButton.setLongClickDetectedHandler(resetButtonLongClick);
}

DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  display_init();
  steppers_init();
  encoders_init();
  buttons_init();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  resetButton.loop();
  startButton.loop();
  bankEncoder.loop();
  pitchEncoder.loop();
  displayBaro();
}
