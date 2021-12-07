#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PWMServoDriver.h>

// Screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define I2C_DISPLAY_ADDRESS 0x3c
#define OLED_RESET     -1

// servo positions 200-400
const unsigned int flag_servos_hidden[4] = {360, 365, 340, 335};
const unsigned int flag_servos_visible[4] = {220, 530, 180, 500};

unsigned int loopcounter = 0;

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

Adafruit_SSD1306 display_baro(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();


void display(unsigned int value) {
  display_baro.clearDisplay();
  display_baro.setTextSize(3);
  display_baro.setTextColor(WHITE);
  display_baro.setCursor(20,10);
  display_baro.println(String(value));
  display_baro.display();
}


void setup() {
  tcaselect(0);
  display_baro.begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY_ADDRESS);  
  tcaselect(1);
  servos.begin();
  servos.setPWMFreq(60);
}

void loop() {
  // For each loop, write to display, and move servo
  tcaselect(1);
  servos.setPWM(0, 0, 200);
  delay(1000);
  servos.setPWM(0, 0, 400);
  tcaselect(0);
  display(loopcounter);
  delay(1000);
  loopcounter++;                        
}
