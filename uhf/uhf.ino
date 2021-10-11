#include "TM1637_6D.h"
#include "TM1637Display.h"

#define CLK 2 //pins definitions for TM1637 and can be changed to other ports
#define DIO 3

#define PCLK 4
#define PDIO 5

TM1637_6D tm1637_6D(CLK, DIO);
TM1637Display tm1637_4D(PCLK, PDIO);

void setup()
{
  tm1637_6D.init();
  tm1637_6D.set(BRIGHTEST); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637_4D.setBrightness(0x0f); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}
void loop()
{
  int8_t ListDisp[6] = {0, 5, 0, 0, 5, 2};
  int8_t ListDispPoint[6] = {POINT_OFF, POINT_OFF, POINT_OFF, POINT_ON, POINT_OFF, POINT_OFF};

  delay(150);

  while (1)
  {
    tm1637_6D.display(ListDisp, ListDispPoint);
    tm1637_4D.showNumberDec(12, false);
    delay(5000);
  }
}
