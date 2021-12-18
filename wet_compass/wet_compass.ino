#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "Button2.h"


// Stepper
#define STP_RES_FULL 720
#define MICROSTEPS 8
const unsigned int STP_HZ  = 1200 * MICROSTEPS;
const int32_t STP_RES = STP_RES_FULL * MICROSTEPS;

// Pins, all here for easy reading
// ENC1 per breakout PCB
const unsigned int cmpStepPin = 10;
const unsigned int cmpDirPin = 12;
// ENC0 per breakout PCB
const unsigned int cmpSetPins[] = {2,3};
// ENC1 inner, ENC2 middle, STP0 middle pins
const unsigned int pwmPins[] = {5,6,9};

// A0 header on board
const unsigned int cmpButtonPin = A0;

bool isInitialized = false;

long cmpPos = 0;
unsigned int heading = 0;
long rotations = 0;
unsigned int primaryConsolesBrt = 175;

FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *cmpStepper = NULL;
ESPRotary cmpEncoder(cmpSetPins[0], cmpSetPins[1]);
Button2 cmpButton;


void setCmp() {
  if (not isInitialized) {   
    // Read new encoder position
    long newPos = cmpEncoder.getPosition();
    // Get diff
    long diff = newPos - cmpPos;
    // Move needle
    cmpStepper->move(diff * 8);
    // Remember new position
    cmpPos = newPos;
  }
}

void cmpButtonLongClick(Button2 button) {  
  if (not isInitialized) {
    isInitialized = true;
    cmpStepper->move(1000);
    delay(500);
    cmpStepper->move(-1000);
    delay(500);
    // Reset altitude and steppers    
    cmpPos = 0;
    cmpStepper->setCurrentPosition(0);
  } else {
    isInitialized = false;
    cmpStepper->move(-1000);
    delay(500);
    cmpStepper->move(1000);
    delay(500);
  }
}

void onUpdateCounterChange(unsigned int newValue) {
  if (isInitialized) {
    cmpStepper->moveTo(STP_RES * rotations + map(heading, 0, 360, 0, STP_RES));
  }
  for(int i = 0; i < 3; i++) {
    analogWrite(pwmPins[i], primaryConsolesBrt);  
  }
}

void onHdgDegChange(unsigned int newValue) {
  // Increasing zero cross?
  if(heading > 330 and newValue < 30) { rotations++; }
  // Decreasing zero cross?
  if(heading < 30 and newValue > 330) { rotations--; }
  heading = newValue;
}

void onPriConsolesBrtKnbChange(unsigned int newValue) { primaryConsolesBrt = map(newValue, 0, 65535, 128, 255); }

DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);
DcsBios::IntegerBuffer hdgDegBuffer(0x0436, 0x01ff, 0, onHdgDegChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  engine.init();

  cmpStepper = engine.stepperConnectToPin(cmpStepPin);
  cmpStepper->setDirectionPin(cmpDirPin);
  cmpStepper->setSpeedInHz(STP_HZ);
  cmpStepper->setAcceleration(10000);

  cmpPos = cmpEncoder.getPosition();
  cmpEncoder.setChangedHandler(setCmp);  

  cmpButton.begin(cmpButtonPin);
  cmpButton.setLongClickTime(2000);
  cmpButton.setLongClickDetectedHandler(cmpButtonLongClick);
  
  // Initialize PWM pins as output
  for(int i = 0; i < 3; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], primaryConsolesBrt);
  }
  
  DcsBios::setup();
}

void loop() {
  if (isInitialized) {
    DcsBios::loop();
  } else {
    cmpEncoder.loop();
  }
  cmpButton.loop();     
}
