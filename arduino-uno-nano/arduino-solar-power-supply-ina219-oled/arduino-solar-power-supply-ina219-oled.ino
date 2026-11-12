/*
 * Solar power supply monitor — panel voltage and current on an OLED
 *
 * Reads a solar panel through an INA219 shunt and shows the panel voltage in
 * large type with the current being drawn beneath it, so a cloud passing over
 * is visible immediately.
 *
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

  setupOled();

  if (!ina219.begin()) {
    printOled("INA219 failed");
    while (true) {
      delay(1000);
    }
  }
  ina219.setCalibration_32V_2A();
}

void loop() {
  float shuntMv   = ina219.getShuntVoltage_mV();
  float busV      = ina219.getBusVoltage_V();
  float currentMa = ina219.getCurrent_mA();
  float loadV     = busV + (shuntMv / 1000.0);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.setTextSize(2);
  display.print(busV, 2);
  display.print(" V");

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Using ");
  display.print(currentMa, 0);
  display.print(" mA");

  display.setCursor(0, 10);
  display.print(loadV, 2);
  display.print(" V load");

  display.display();
  delay(500);
}

bool setupOled() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 not found");
    while (true) {
      delay(1000);
    }
  }
  display.clearDisplay();
  display.setRotation(2);   // mounted upside down on the panel box
  display.display();
  return true;
}

void printOled(const String &text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}
