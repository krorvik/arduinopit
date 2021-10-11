#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#define MAX_STRING_LENGTH 25

// SPI Pins
#define CS 10
#define DC 9
#define RESET 8
// Vars to draw display, init so we see we're live
char* LINE1 = (char*) " ";
char* LINE2 = (char*) " ";
char* LINE3 = (char*) "F-16C block 50 DED READY";
char* LINE4 = (char*) " ";
char* LINE5 = (char*) " ";
char FUEL[5] = { '0', '0', '0', '0', '0'};

// Display
// Hardware SPI
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

// Callback functions
void draw_line1(char* value) { LINE1 = value; drawDED(); }
void draw_line2(char* value) { LINE2 = value; drawDED(); }
void draw_line3(char* value) { LINE3 = value; drawDED(); }
void draw_line4(char* value) { LINE4 = value; drawDED(); }
void draw_line5(char* value) { LINE5 = value; drawDED(); }

char drawFuel(unsigned int index, unsigned int value) {
  // ASCII, "0" is  int 48, and value here is 16 bits, so divide down for 0-9. 
  FUEL[index] = (char) (48 + (value / 6553));
  if (index == 2) {
    // We can set the last two digits based on the middle one. 
  }
  drawDED();
}

void draw_10k(unsigned int value) { 
  drawFuel(0, value);
}
void draw_1k(unsigned int value) { 
  drawFuel(1, value );
}

void draw_100(unsigned int value) { 
  drawFuel(2, value );
}

// Register callback functions
DcsBios::StringBuffer<25> callback_line1(0x44fc, draw_line1);
DcsBios::StringBuffer<25> callback_line2(0x4516, draw_line2);
DcsBios::StringBuffer<25> callback_line3(0x4530, draw_line3);
DcsBios::StringBuffer<25> callback_line4(0x454a, draw_line4);
DcsBios::StringBuffer<25> callback_line5(0x4564, draw_line5);
DcsBios::IntegerBuffer callback_fuel_10k(0x44d6, 0xffff, 0x0, draw_10k);
DcsBios::IntegerBuffer callback_fuel_1k(0x44d8, 0xffff, 0x0, draw_1k);
DcsBios::IntegerBuffer callback_fuel_100(0x44da, 0xffff, 0x0, draw_100);

void setup(void) {
  DED.begin();
  drawDED();
  DcsBios::setup();  
}

void loop(void) {
  DcsBios::loop();
}
