#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT  32 // OLED display height, in pixels
#define OLED_RESET     -1

Adafruit_SSD1306 display_mach(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  display_mach.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_mach.clearDisplay();
  display_mach.setTextSize(3);      // Normal 1:1 pixel scale
  display_mach.setTextColor(WHITE); // Draw white text
  display_mach.setCursor(25, 0);
  display_mach.println(F("1.99"));
  display_mach.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
