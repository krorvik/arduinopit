#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// Seems there will be issues with 4+ encoders, so we stick to 3 per nano :(
/*DcsBios::RotaryEncoder uhfChanKnb("UHF_CHAN_KNB", "DEC", "INC", 4,5);
DcsBios::RotaryEncoder uhfFreq0025Knb("UHF_FREQ_0025_KNB", "DEC", "INC", 6,7);
DcsBios::RotaryEncoder uhfFreq01Knb("UHF_FREQ_01_KNB", "DEC", "INC", 8,9);*/
DcsBios::RotaryEncoder uhfFreq1Knb("UHF_FREQ_1_KNB", "DEC", "INC", 4,5);
DcsBios::RotaryEncoder uhfFreq10Knb("UHF_FREQ_10_KNB", "DEC", "INC", 6,7);
DcsBios::RotaryEncoder uhfFreq100Knb("UHF_FREQ_100_KNB", "DEC", "INC", 8,9);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
