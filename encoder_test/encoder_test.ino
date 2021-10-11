#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>

DcsBios::RotaryEncoder UHF_CHAN_KNB("UHF_CHAN_KNB", "DEC", "INC", 10, 11);

void setup() {  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
