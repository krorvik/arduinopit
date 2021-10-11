int potpin = 0;  // analog pin used to connect the potentiometer
int ledpin = 9;
int val;    // variable to read the value from the analog pin

void setup() {
  // put your setup code here, to run once:
  pinMode(ledpin, OUTPUT);
}

void loop() {
  // PWM expects values from 0 to 255, so divide read analog (0-1024) by four. 
  val = analogRead(potpin) / 4;
  analogWrite(ledpin, val);
}
