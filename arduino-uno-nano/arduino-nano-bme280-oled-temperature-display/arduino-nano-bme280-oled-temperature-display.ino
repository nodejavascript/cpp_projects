/*
 * BME280 temperature and humidity display
 *
 * Reads a BME280 over I2C and shows temperature in large type with humidity
 * beneath it on a 128x32 OLED. The display blanks itself after a few seconds
 * without a button press and comes back when the button is pressed, so a
 * bedside unit is not a lamp.
 *
 * Board: Arduino Nano (ATmega328P, old bootloader)
 * Wiring: BME280 and OLED on I2C · button on D4 to ground
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  32
#define OLED_RESET     -1
#define OLED_ADDRESS   0x3C
#define BME280_ADDRESS 0x76

#define BUTTON_PIN     4
#define BLANK_AFTER_MS 4000
#define REFRESH_MS     250

// Local sea-level pressure, used only to convert pressure into an altitude.
#define SEALEVELPRESSURE_HPA 1003.40

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;

unsigned long lastButtonPress = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 not found");
    while (true) {
      delay(1000);
    }
  }
  display.clearDisplay();
  display.display();

  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("BME280 not found");
    while (true) {
      delay(1000);
    }
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonPress = millis();
}

void loop() {
  // The button is wired to ground, so a press reads LOW.
  if (digitalRead(BUTTON_PIN) == LOW) {
    lastButtonPress = millis();
  }

  bool awake = (millis() - lastButtonPress) < BLANK_AFTER_MS;

  if (awake) {
    drawReadings();
    display.ssd1306_command(SSD1306_DISPLAYON);
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  delay(REFRESH_MS);
}

void drawReadings() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(bme.readTemperature(), 1);
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(0, 22);
  display.print(bme.readHumidity(), 1);
  display.print(" % RH");

  display.display();
}

// Kept for the projects that log pressure and altitude as well as the screen.
float pressureHpa() {
  return bme.readPressure() / 100.0F;
}

float altitudeM() {
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}
