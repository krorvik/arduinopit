#include <Servo.h>

Servo oil;  // create servo object to control a servo
Servo noz;
Servo rpm;
Servo ftit;
int position = 0 ;
int direction = 1;

void setup() {
  oil.attach(3);
  noz.attach(5);
  rpm.attach(6);
  ftit.attach(9);
}

void loop() {
  oil.write(position % 180);
  noz.write(position % 180);
  rpm.write(position % 180);
  ftit.write(position % 180);
  //position += 1;
  delay(30);                           // waits for the servo to get there
}
