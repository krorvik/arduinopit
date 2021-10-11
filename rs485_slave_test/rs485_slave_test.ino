// USB
/*
#define DCSBIOS_IRQ_SERIAL 
#include "DcsBios.h"
*/
// RRS485

#define DCSBIOS_RS485_SLAVE 2
#define TXENABLE_PIN 2
#include "DcsBios.h"


/* paste code snippets from the reference documentation here */
DcsBios::LED lightMasterCaution(0x4472, 0x0800, 13);

void setup() {
  DcsBios::setup();
}

void loop() {
 DcsBios::loop();
}
