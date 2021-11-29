#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h> 

// DED
#define MAX_STRING_LENGTH 25

// SPI Pins DED 
#define CS 16
#define DC 15
#define RESET 14

// Vars to draw DED display, init so we see we're live
char* LINE1 = (char*) " ";
char* LINE2 = (char*) " ";
char* LINE3 = (char*) "DCS WORLD F-16C block 50";
char* LINE4 = (char*) " ";
char* LINE5 = (char*) " ";
char FUEL[5] = { '0', '0', '0', '0', '0'};

// DED Display
// Hardware SPI, 4 wire, full buffer
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI DED(U8G2_R0, CS, DC, RESET);
 
void drawDED() {
  DED.clearBuffer();
  DED.drawLine(178, 0, 178, 64);
  DED.setFont(u8g2_font_7x14B_mf);
  DED.drawStr(0, 12, LINE1);
  DED.drawStr(0, 25, LINE2);
  DED.drawStr(0, 38, LINE3);
  DED.drawStr(0, 51, LINE4);
  DED.drawStr(0, 64, LINE5);
  DED.drawStr(185 , 12, "FUELFLOW");
  DED.drawStr(185 + 18, 60, "PPH");    
  DED.setFont(u8g2_font_VCR_OSD_mn);
  DED.drawStr(180, 40, (char*) FUEL);  
  DED.sendBuffer();
}

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

// DED fuel helper
char update_fuel(unsigned int index, unsigned int value) {
  // ASCII, "0" is  int 48, and value here is 16 bits, so divide down for 0-9. 
  FUEL[index] = (char) (48 + (value / 6553));
  if (index == 2) {
    // We can set the last two digits based on the middle one. 
  }
}

// Callbacks for DED
void update_line1(char* value) { LINE1 = value; }
void update_line2(char* value) { LINE2 = value; }
void update_line3(char* value) { LINE3 = value; }
void update_line4(char* value) { LINE4 = value; }
void update_line5(char* value) { LINE5 = value; }
void draw_10k(unsigned int value) { update_fuel(0, value); }
void draw_1k(unsigned int value) { update_fuel(1, value ); }
void draw_100(unsigned int value) { update_fuel(2, value ); }
void set_ded_brightness(unsigned int value) { DED.setContrast(map(value, 0, 65535, 0, 255)); }
void onUpdateCounterChange(unsigned int newValue) { drawDED(); }

// DED and fuel callback register
DcsBios::StringBuffer<25> callback_line1(0x44fc, update_line1);
DcsBios::StringBuffer<25> callback_line2(0x4516, update_line2);
DcsBios::StringBuffer<25> callback_line3(0x4530, update_line3);
DcsBios::StringBuffer<25> callback_line4(0x454a, update_line4);
DcsBios::StringBuffer<25> callback_line5(0x4564, update_line5);

DcsBios::IntegerBuffer callback_fuel_10k(0x44d6, 0xffff, 0x0, draw_10k);
DcsBios::IntegerBuffer callback_fuel_1k(0x44d8, 0xffff, 0x0, draw_1k);
DcsBios::IntegerBuffer callback_fuel_100(0x44da, 0xffff, 0x0, draw_100);
DcsBios::IntegerBuffer callback_ded_brightness(0x4412, 0xffff, 0x0, set_ded_brightness);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

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

// Gear indicators
DcsBios::LED LIGHT_GEAR_WARN(0x4474 ,  0x1000 , 35);
DcsBios::LED LIGHT_GEAR_R(0x4474 ,  0x800 ,  37);
DcsBios::LED LIGHT_GEAR_N(0x4474 ,  0x200 , 39);
DcsBios::LED LIGHT_GEAR_L(0x4474 ,  0x400 , 41);
 
void initialize_pins() {
  // Init pins
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = A0; i <= A5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = A0; i <= A5; i++) {
    digitalWrite(i, LOW);
  }
}

void setup() {  
  initialize_pins();  
  // Clear and draw DED
  DED.begin();
  drawDED();  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
