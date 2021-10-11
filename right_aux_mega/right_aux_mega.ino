#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>

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
DcsBios::LED LIGHT_CAUTION_1 ( 0x4472 ,  0x2 , 52);
DcsBios::LED LIGHT_CAUTION_2 ( 0x4472 ,  0x20 , 33);
DcsBios::LED LIGHT_CAUTION_3 ( 0x4472 ,  0x40 , 35);
DcsBios::LED LIGHT_CAUTION_4 ( 0x4472 ,  0x100 , 37);
DcsBios::LED LIGHT_CAUTION_5 ( 0x4472 ,  0x200 , 51);
DcsBios::LED LIGHT_CAUTION_6 ( 0x4472 ,  0x400 , 53);
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
  // Init pins
  for (int i = 22; i <= 53; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i = A0; i <= A11; i++) {
    pinMode(i, OUTPUT);
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
