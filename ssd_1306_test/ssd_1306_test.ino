#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen data
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

Adafruit_SSD1306 display_alt(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char alt_digits[] = {'0', '0', '0', '0', '0'};
int32_t alt_100_steps = 0;
int32_t alt_1k_steps = 0;
int32_t alt_10k_steps = 0;

void displayAlt() {
  display_alt.clearDisplay();
  display_alt.setTextSize(3);
  display_alt.setTextColor(WHITE);
  display_alt.setCursor(20, 5);
  display_alt.println(alt_digits);
  display_alt.display();
  delay(50);
}

void setup() {
  display_alt.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop() {
  displayAlt();
}
  
