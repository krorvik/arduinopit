#include <Adafruit_SSD1306.h>
#include <Fonts/falconded8pt7b.h>


//Define SPI Connections:
#define OLED_MOSI   5
#define OLED_CLK   6
#define OLED_DC    3
#define OLED_CS    2
#define OLED_RESET 4
//Remove comments (//) if using default SPI connection:
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//Add comments (//) if using default SPI connection:
//Adafruit_SSD1306 display(OLED_RESET);

void setup()  

{               
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Initialize with the I2C address 0x3C.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setFont(&falconded8pt7b);
  display.setCursor(0,12);
  display.println("0123456789012345");
  display.setCursor(0,24);
  display.println("ABCDEFGHIJKLMNOP");
  display.setCursor(0,36);
  display.println("0123456789012345");
  display.setCursor(0,48);
  display.println("0123456789012345");
  display.setCursor(0,60);
  display.println("0123456789012345");
  display.display();
}

void loop()
{ 
}
