#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include "TM1637_6D.h"
#include "TM1637Display.h"

const unsigned int FIRST_LED_PIN = 13;
const unsigned int LAST_LED_PIN = 53;

#define CLK 2 //pins definitions for TM1637 and can be changed to other ports
#define DIO 3

#define PCLK 4
#define PDIO 5

TM1637_6D tm1637_6D(CLK, DIO);
TM1637Display tm1637_4D(PCLK, PDIO);

// Predef data for 7seg displays
const uint8_t SEG_STARS_4[] = {
  SEG_G,           // -
  SEG_G            // -
};
int8_t initdisp_6[6] = {8,8,8,8,8,8};
int8_t initpoint_6[6] = {(int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON, (int8_t) POINT_ON};

/*
// CMDS counters, ALL STRINGS
CMDS_CH_Amount 0x4586 ,  0x4
CMDS_FL_Amount 0x458a ,  0x4
CMDS_O1_Amount 0x457e ,  0x4
CMDS_O2_Amount 0x4582 ,  0x4   # length 
*/

// Use analog pins too

// CMDS 
DcsBios::LED LIGHT_CMDS_DISP ( 0x447a,  0x40, A0);
DcsBios::LED LIGHT_CMDS_GO ( 0x447a,  0x20, A1);
DcsBios::LED LIGHT_CMDS_NO_GO ( 0x447a,  0x10, A2);
DcsBios::LED LIGHT_CMDS_RDY ( 0x447a,  0x80 , A3);

// rem eyebrow
DcsBios::LED LIGHT_TF_FAIL ( 0x4472 ,  0x2000 ,  A5 );
DcsBios::LED LIGHT_IFF ( 0x4470 ,  0x200 ,  A6 );

// A7-17 are available for LEDS (or GND.... ;))

// Then digital, large array pins
// ELEC
DcsBios::LED LIGHT_FLCS_PMG ( 0x4476 ,  0x10 ,  A7);
DcsBios::LED LIGHT_MAIN_GEN ( 0x4476 ,  0x20 ,  A8);

DcsBios::LED LIGHT_STBY_GEN ( 0x4476 ,  0x40 ,  A9);

DcsBios::LED LIGHT_EPU_GEN ( 0x4476 ,  0x100 ,  A10);
DcsBios::LED LIGHT_EPU_PMG ( 0x4476 ,  0x200 ,  A11);

// Single light in one indicator, use two LEDs
DcsBios::LED LIGHT_ACFT_BATT_FAIL1 ( 0x4476 ,  0x1000 ,  A12);
DcsBios::LED LIGHT_ACFT_BATT_FAIL2 ( 0x4476 ,  0x1000 ,  A13);

DcsBios::LED LIGHT_TO_FLCS ( 0x4476 ,  0x400 ,  A14);
DcsBios::LED LIGHT_FLCS_RLY ( 0x4476 ,  0x800 ,  A15);

// EPU
DcsBios::LED LIGHT_AIR ( 0x4476 ,  0x4 ,  14 );
DcsBios::LED LIGHT_HYDRAZN ( 0x4476 ,  0x2 ,  15 );


// Switch to digital array pins
// FLCS TEST
DcsBios::LED LIGHT_FLCS_PWR_A ( 0x4478 ,  0x2 ,  17 );
DcsBios::LED LIGHT_FLCS_PWR_B ( 0x4478 ,  0x4 ,  18) ;
DcsBios::LED LIGHT_FLCS_PWR_C ( 0x4478 ,  0x8 ,  19 );
DcsBios::LED LIGHT_FLCS_PWR_D ( 0x4478 ,  0x10 ,  20 );
 
// FLCS BIT
DcsBios::LED LIGHT_FL_RUN ( 0x4476 ,  0x8000 ,  34);
DcsBios::LED LIGHT_FL_FAIL ( 0x4478 ,  0x1 ,  35);

// TWA
DcsBios::LED LIGHT_RWR_ALT_LOW_1 (0x4478 ,  0x100 ,  36);   // x2
DcsBios::LED LIGHT_RWR_ALT_LOW_2 (0x4478 ,  0x100 ,  37);

DcsBios::LED LIGHT_RWR_SEARCH (0x4478 ,  0x20 ,  38);
// skip 39 for nice connector symmetry ;)

DcsBios::LED LIGHT_RWR_POWER_1 (0x4478 ,  0x400 ,  40);   // x2
DcsBios::LED LIGHT_RWR_POWER_2 (0x4478 ,  0x400 ,  41);

DcsBios::LED LIGHT_RWR_ACT_POWER_1 (0x4478 ,  0x80 ,  42);   // x2
DcsBios::LED LIGHT_RWR_ACT_POWER_2 (0x4478 ,  0x80 ,  43);

// Trim servos: 12-13
DcsBios::ServoOutput PITCHTRIMIND (0x44c4, 12, 500, 2500);
DcsBios::ServoOutput ROLLTRIMIND (0x44c2, 13, 500, 2500);

// UHF encoder knobs
DcsBios::RotaryEncoder UHF_FREQ_0025_KNB("UHF_FREQ_0025_KNB", "DEC", "INC", 22,23);
DcsBios::RotaryEncoder UHF_FREQ_01_KNB("UHF_FREQ_01_KNB", "DEC", "INC", 24,25);
DcsBios::RotaryEncoder UHF_FREQ_1_KNB("UHF_FREQ_1_KNB", "DEC", "INC", 26,27);
DcsBios::RotaryEncoder UHF_FREQ_10_KNB("UHF_FREQ_10_KNB", "DEC", "INC", 28,29);
DcsBios::RotaryEncoder UHF_FREQ_100_KNB("UHF_FREQ_100_KNB", "DEC", "INC", 30,31);
DcsBios::RotaryEncoder UHF_CHAN_KNB("UHF_CHAN_KNB", "DEC", "INC", 32,33);

// Callback functions for UHF displays
void onUhfChanDispChange(char* newValue) {
    // Known non-decimal values
    char stars[] = "**";
    if (strcmp(newValue, stars) == 0) { 
      tm1637_4D.setSegments(SEG_STARS_4, 2); 
    } else if (strcmp(newValue, "  ") == 0) { 
      tm1637_4D.clear(); 
    } else {
      // Else should be a number. Trust!
      tm1637_4D.showNumberDec( atoi(newValue), 2);
    }
}

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

// Landing gear LEDS are exposed, so we need to dim them a bit
void onLightGearWarnChange(unsigned int newValue) { analogWrite(8, newValue * 255); }
void onLightGearRChange(unsigned int newValue) { analogWrite(9, newValue * 16); }
void onLightGearNChange(unsigned int newValue) { analogWrite(10, newValue * 16); }
void onLightGearLChange(unsigned int newValue) { analogWrite(11, newValue * 16); }
void onLightJfsRunChange(unsigned int newValue) { analogWrite(A4, newValue * 128); }
void onLightEpuChange(unsigned int newValue) { analogWrite(16, newValue * 128); }

// Callbacks for radio and gear lights
DcsBios::StringBuffer<7> uhfFreqDispBuffer(0x4590, onUhfFreqDispChange);
DcsBios::StringBuffer<2> uhfChanDispBuffer(0x458e, onUhfChanDispChange);
DcsBios::IntegerBuffer lightGearRBuffer(0x4474, 0x0800, 11, onLightGearRChange);
DcsBios::IntegerBuffer lightGearNBuffer(0x4474, 0x0200, 9, onLightGearNChange);
DcsBios::IntegerBuffer lightGearLBuffer(0x4474, 0x0400, 10, onLightGearLChange);
DcsBios::IntegerBuffer lightGearWarnBuffer(0x4474, 0x1000, 12, onLightGearWarnChange);
DcsBios::IntegerBuffer lightJfsRunBuffer(0x4476, 0x0001, 0, onLightJfsRunChange);
DcsBios::IntegerBuffer lightEpuBuffer(0x4476, 0x0008, 3, onLightEpuChange);

void initialize() {
  // Init pins
  for (int i = FIRST_LED_PIN; i <= LAST_LED_PIN; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = A0; i <= A15; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  // Light up displays
  tm1637_6D.display(initdisp_6, initpoint_6);
  tm1637_4D.showNumberDecEx(88, 0b11100000, false, 2, 2);
  
  onLightGearRChange(1);
  onLightGearNChange(1);
  onLightGearLChange(1);
  onLightGearWarnChange(1);
  onLightJfsRunChange(1);
  
  delay(2000);
  for (int i = FIRST_LED_PIN; i <= LAST_LED_PIN; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = A0; i <= A15; i++) {
    digitalWrite(i, LOW);
  }
  // Clear displays
  tm1637_6D.clearDisplay(); 
  tm1637_4D.clear(); 
  // Gear LEDs, separate sequence
  
  onLightGearRChange(0);
  onLightGearNChange(0);
  onLightGearLChange(0);
  onLightGearWarnChange(0);
  onLightJfsRunChange(0);
}

void setup() {  
  tm1637_6D.init();
  tm1637_6D.set(BRIGHTEST); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637_4D.setBrightness(0x0f); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  initialize();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
