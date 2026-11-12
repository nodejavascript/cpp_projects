/*
 * ATX bench power supply — voltage, current and power on an OLED
 *
 * Switches on a salvaged ATX supply and reads its output through an INA219
 * shunt, so the screen shows what the supply is actually delivering instead of
 * what the label claims.
 *
 * Wiring: INA219 on I2C (SDA/SCL), OLED on the same bus, ATX PS-ON pulled low.
 * Board: Arduino Nano or Uno
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  32
#define OLED_RESET     -1
#define OLED_ADDRESS   0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 not found");
    while (true) {
      delay(1000);
    }
  }
  display.clearDisplay();
  display.display();

  if (!ina219.begin()) {
    Serial.println("INA219 not found");
    while (true) {
      delay(1000);
    }
  }
  ina219.setCalibration_32V_2A();

  Serial.println("Ready");
}

void loop() {
  float shuntMv = ina219.getShuntVoltage_mV();
  float busV    = ina219.getBusVoltage_V();
  float currentMa = ina219.getCurrent_mA();
  float powerMw   = ina219.getPower_mW();
  float loadV     = busV + (shuntMv / 1000.0);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.setTextSize(1);
  display.print(loadV, 2);
  display.print(" V");
  display.setCursor(SCREEN_WIDTH - 60, 0);
  display.print(powerMw, 0);
  display.print(" mW");

  display.setTextSize(2);
  display.setCursor(0, 14);
  display.print(currentMa, 0);
  display.print(" mA");

  display.display();
  delay(500);
}
