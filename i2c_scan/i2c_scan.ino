 #include "I2CScanner.h"

I2CScanner scanner;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) {};
  scanner.Init();
}

void loop() 
{
  for (int i = 0; i < 3; i++) {
    Wire.beginTransmission(0x70);
    Wire.write(1 << i);
    Wire.endTransmission(); 
    Serial.print("Scanning channel: ");
    Serial.println(i);
    scanner.Scan();
    delay(5000);
  }
}
