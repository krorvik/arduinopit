void setup() {
  // put your setup code here, to run once:
  for (int i = 8; i<11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for (int i = 8; i<11; i++) {
    digitalWrite(i, HIGH);
    delay(2000);
    digitalWrite(i, LOW);    
  }

}
