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

#define CMDS_CLK 6
#define CMDS_DIO 7

TM1637_6D tm1637_freq(CLK, DIO);
TM1637_6D tm1637_cmds(CMDS_CLK, CMDS_DIO);
TM1637Display tm1637_chan(PCLK, PDIO);

// Predef data for 7seg displays
const uint8_t SEG_STARS_4[] = {
  SEG_G,           // -
  SEG_G            // -
};

int8_t initdisp_freq[6] = {8,8,8,8,8,8};
int8_t initdisp_cmds[6] = {8,8,8,8,8,8};
int8_t digits_cmds[6] =     {10, 10, 10, 10, 10, 10 };
int8_t points_cmds[6] = { POINT_OFF, POINT_OFF, POINT_OFF, POINT_OFF, POINT_OFF, POINT_OFF };
int8_t initpoint_freq[6] = {POINT_ON, POINT_ON, POINT_ON, POINT_ON, POINT_ON, POINT_ON};
int8_t initpoint_cmds[6] = {POINT_ON, POINT_ON, POINT_ON, POINT_ON, POINT_ON, POINT_ON};

// CMDS 

DcsBios::LED LIGHT_CMDS_NO_GO_1 ( 0x447a,  0x10, A0);
DcsBios::LED LIGHT_CMDS_NO_GO_2 ( 0x447a,  0x10, A1);
DcsBios::LED LIGHT_CMDS_GO ( 0x447a,  0x20, A2);
DcsBios::LED LIGHT_CMDS_DISP_1 ( 0x447a,  0x40, A3);
DcsBios::LED LIGHT_CMDS_DISP_2 ( 0x447a,  0x40, A4);
DcsBios::LED LIGHT_CMDS_RDY ( 0x447a,  0x80 , A5);

// Then digital pins
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

// Callback functions for UHF displays
void onUhfChanDispChange(char* newValue) {
  // Known non-decimal values
  char stars[] = "**";
  if (strcmp(newValue, stars) == 0) { 
    tm1637_chan.setSegments(SEG_STARS_4, 2); 
  } else if (strcmp(newValue, "  ") == 0) { 
    tm1637_chan.clear(); 
  } else {
    // Else should be a number. Trust!
    tm1637_chan.showNumberDec( atoi(newValue), 2);
  }
}

void onUhfFreqDispChange(char* newValue) {
  // Known non-decimal values
  char stars[] = "***.***";
  if (strcmp(newValue, stars) == 0) { 
    tm1637_freq.displayError();
  } else if (strcmp(newValue, "       ") == 0) { 
    tm1637_freq.clearDisplay(); 
  } else {
    // Float, display it
    float value = atof(newValue);
    tm1637_freq.displayFloat(value);
  }
}

void displayCMDS() {
  tm1637_cmds.display(digits_cmds, points_cmds);
}

void onCmdsChAmountChange(char* newValue) {
  // Digits 1,2
  // First char is ignored for now, thousands not possible - can be "L" - and if it is, and second is "o", we set the decimal point for warning
  // Second char is ignored, if "o" we set 
  if (newValue[0] == 'L' and newValue[1] == 'o') { points_cmds[1] = POINT_ON; } else { points_cmds[1] = POINT_OFF; }
  // third char, first case is unparseable - atoi still returns 0, so check for diff, else set to atoi value
  if (newValue[2] != ' ') { digits_cmds[4] = newValue[2] - '0'; } else { digits_cmds[4] = 10; }
  if (newValue[3] != ' ') { digits_cmds[3] = newValue[3] - '0'; } else { digits_cmds[3] = 10; }
  displayCMDS();
}

void onCmdsFlAmountChange(char* newValue) {
  // Digits 4,5
  // First char is ignored for now, thousands not possible - can be "L" - and if it is, and second is "o", we set the decimal point for warning
  // Second char is ignored, if "o" we set 
  if (newValue[0] == 'L' and newValue[1] == 'o') { points_cmds[4] = POINT_ON; } else { points_cmds[4] = POINT_OFF; }
  // third char, first case is unparseable - atoi still returns 0, so check for diff, else set to atoi value
  if (newValue[2] != ' ') { digits_cmds[1] = newValue[2] - '0'; } else { digits_cmds[1] = 10; }
  if (newValue[3] != ' ') { digits_cmds[0] = newValue[3] - '0'; } else { digits_cmds[0] = 10; }
  displayCMDS();
}

// Landing gear LEDS are exposed, so we need to dim them "a bit"
void onLightGearWarnChange(unsigned int newValue) { analogWrite(8, newValue * 255); }
void onLightGearRChange(unsigned int newValue) { analogWrite(9, newValue * 16); }
void onLightGearNChange(unsigned int newValue) { analogWrite(10, newValue * 16); }
void onLightGearLChange(unsigned int newValue) { analogWrite(11, newValue * 16); }
void onLightJfsRunChange(unsigned int newValue) { analogWrite(12, newValue * 32); }
void onLightEpuChange(unsigned int newValue) { analogWrite(16, newValue * 32); }

// Callbacks for radio and gear lights
DcsBios::StringBuffer<7> uhfFreqDispBuffer(0x4590, onUhfFreqDispChange);
DcsBios::StringBuffer<2> uhfChanDispBuffer(0x458e, onUhfChanDispChange);
DcsBios::IntegerBuffer lightGearRBuffer(0x4474, 0x0800, 11, onLightGearRChange);
DcsBios::IntegerBuffer lightGearNBuffer(0x4474, 0x0200, 9, onLightGearNChange);
DcsBios::IntegerBuffer lightGearLBuffer(0x4474, 0x0400, 10, onLightGearLChange);
DcsBios::IntegerBuffer lightGearWarnBuffer(0x4474, 0x1000, 12, onLightGearWarnChange);
DcsBios::IntegerBuffer lightJfsRunBuffer(0x4476, 0x0001, 0, onLightJfsRunChange);
DcsBios::IntegerBuffer lightEpuBuffer(0x4476, 0x0008, 3, onLightEpuChange);
DcsBios::StringBuffer<4> cmdsChAmountBuffer(0x4586, onCmdsChAmountChange);
DcsBios::StringBuffer<4> cmdsFlAmountBuffer(0x458a, onCmdsFlAmountChange);

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
  tm1637_freq.display(initdisp_freq, initpoint_freq);
  tm1637_cmds.display(initdisp_cmds, initpoint_cmds);
  tm1637_chan.showNumberDecEx(88, 0b11100000, false, 2, 2);
  
  onLightGearRChange(1);
  onLightGearNChange(1);
  onLightGearLChange(1);
  onLightGearWarnChange(1);
  //onLightJfsRunChange(1);
  
  delay(2000);
  for (int i = FIRST_LED_PIN; i <= LAST_LED_PIN; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = A0; i <= A15; i++) {
    digitalWrite(i, LOW);
  }
  // Clear displays
  tm1637_freq.clearDisplay(); 
  tm1637_cmds.clearDisplay(); 
  tm1637_chan.clear(); 
  // Gear LEDs, separate sequence
  
  onLightGearRChange(0);
  onLightGearNChange(0);
  onLightGearLChange(0);
  onLightGearWarnChange(0);
  onLightJfsRunChange(0);
}

void setup() {  
  tm1637_freq.init();
  tm1637_freq.set(5); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637_cmds.init();
  tm1637_cmds.set(5); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637_chan.setBrightness(0x0f); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  initialize();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
