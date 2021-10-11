#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>

// Caution panel LEDs
DcsBios::LED LIGHT_AOA_DN ( 0x4474 ,  0x100 , A0);
DcsBios::LED LIGHT_AOA_MID ( 0x4474 ,  0x80 , A1);
DcsBios::LED LIGHT_AOA_UP ( 0x4474 ,  0x40 , A2);
DcsBios::LED LIGHT_DISC ( 0x4474 ,  0x8000 , A3);
DcsBios::LED LIGHT_AR_NWS ( 0x4474 ,  0x4000 , A4);
DcsBios::LED LIGHT_RDY ( 0x4474 ,  0x2000 , A5);

// Right eyebrow lights
DcsBios::LED LIGHT_ENG_FIRE ( 0x4472 ,  0x4000 , 2);
DcsBios::LED LIGHT_ENGINE ( 0x4472 ,  0x8000 , 3);
DcsBios::LED LIGHT_HYD_OIL_PRESS ( 0x4474 ,  0x1 , 4);
DcsBios::LED LIGHT_FLCS ( 0x4474 ,  0x2 , 5);
DcsBios::LED LIGHT_DBU_ON ( 0x4474 ,  0x4 , 6);
DcsBios::LED LIGHT_TO_LDG_CONFIG ( 0x4474 ,  0x8 , 7);
DcsBios::LED LIGHT_CANOPY ( 0x4474 ,  0x10 , 8);
DcsBios::LED LIGHT_OXY_LOW ( 0x4474 ,  0x20 , 9);

// Left eyebrow - four LEDS for large master caution
DcsBios::LED LIGHT_MASTER_CAUTION0 ( 0x4472 ,  0x800 , 10);
DcsBios::LED LIGHT_MASTER_CAUTION1 ( 0x4472 ,  0x800 , 11);
DcsBios::LED LIGHT_MASTER_CAUTION2 ( 0x4472 ,  0x800 , 12);
DcsBios::LED LIGHT_MASTER_CAUTION3 ( 0x4472 ,  0x800 , 13);
//DcsBios::LED LIGHT_TF_FAIL ( 0x4472 ,  0x2000 , 11);

// TWP
// R row
// Upper
DcsBios::LED LIGHT_RWR_HANDOFF_H( 0x4478 ,  0x1000 , 22);
// Mid
DcsBios::LED LIGHT_RWR_MODE_OPEN( 0x4478 ,  0x8000 ,  24);
DcsBios::LED LIGHT_RWR_MODE_PRI( 0x4478 ,  0x4000 ,  26);
// low
DcsBios::LED LIGHT_RWR_TGTSEP_UP( 0x447a ,  0x4 ,  28);
DcsBios::LED LIGHT_RWR_TGTSEP_DN( 0x447a ,  0x8 ,  30);
// Mid row
// Upper 
DcsBios::LED LIGHT_RWR_MSL_LAUNCH_UP( 0x4478 ,  0x2000 ,  23);
DcsBios::LED LIGHT_RWR_MSL_LAUNCH_DN( 0x4478 ,  0x2000 ,  25);
// Lower
DcsBios::LED LIGHT_RWR_SYSTEST_UP(  0x447a ,  0x2 ,  27);
DcsBios::LED LIGHT_RWR_SYSTEST_DN(  0x447a ,  0x2 ,  29);

// Left button
DcsBios::LED LIGHT_RWR_SHIP_UNK_UP( 0x447a ,  0x1 ,  31);
DcsBios::LED LIGHT_RWR_SHIP_UNK_DN( 0x447a ,  0x1 ,  33);

void initialize_pins() {
  // Init pins
  for (int i = 2; i <= 53; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = A0; i <= A5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 2; i <= 53; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = A0; i <= A5; i++) {
    digitalWrite(i, LOW);
  }
}

void setup() {
  initialize_pins();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
