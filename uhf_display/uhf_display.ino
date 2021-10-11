#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include "TM1637_6D.h"

#define CLK 2 //pins definitions for TM1637 and can be changed to other ports
#define DIO 3


TM1637_6D tm1637_6D(CLK, DIO);

int8_t initdisp_6[6] = {8,8,8,8,8,8};
int8_t initpoint_6[6] = {(int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON};

void onUhfFreqDispChange(char* newValue) {
    // Known non-decimal values
    char stars[] = "***.***";
    if (strcmp(newValue, stars) == 0) { 
      tm1637_6D.displayError();
    } else if (strcmp(newValue, "       ") == 0) { 
      tm1637_6D.clearDisplay(); 
    } else {
      // Float, display it
      float value = atof(newValue);
      tm1637_6D.displayFloat(value);
    }
}

// Callbacks for radio and gear lights
DcsBios::StringBuffer<7> uhfFreqDispBuffer(0x4590, onUhfFreqDispChange);

void initialize() {
  // Light up displays
  tm1637_6D.display(initdisp_6, initpoint_6);
  delay(2000);
  tm1637_6D.clearDisplay(); 
}

void setup() {  
  tm1637_6D.init();
  tm1637_6D.set(BRIGHTEST); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  initialize();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
