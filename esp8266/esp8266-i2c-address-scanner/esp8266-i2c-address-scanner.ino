/*
 * I2C address scanner
 *
 * Walks every address on the I2C bus and prints the ones that answer, so an
 * unknown breakout tells you its address before you write a driver for it.
 * Run it once on any new sensor; the address it prints goes into that project.
 *
 * Board: any ESP8266 or AVR board with an I2C device attached
 */

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(100);
  Serial.println("\nI2C scanner");
}

void loop() {
  int found = 0;

  Serial.println("Scanning...");

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("  found 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      found++;
    } else if (error == 4) {
      Serial.print("  unknown error at 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (found == 0) {
    Serial.println("  no devices — check SDA, SCL and power");
  } else {
    Serial.print("  ");
    Serial.print(found);
    Serial.println(" device(s)");
  }

  delay(3000);
}
