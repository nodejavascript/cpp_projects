/*
 * Vibration sensor monitor
 *
 * Measures how long an SW-420 vibration switch stays closed each time it
 * triggers, and keeps a running count, average, minimum and maximum on a
 * 128x32 OLED. Hold the button down to reset the statistics.
 *
 * Board: Arduino Nano or Uno
 * Wiring: vibration switch on D5, LED on D12, reset button on D7
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  32
#define OLED_RESET     -1
#define OLED_ADDRESS   0x3C

#define LED_PIN        12
#define BUTTON_PIN     7
#define VIBRATION_PIN  5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long vibrationSum = 0;
long vibrationCount = 0;
long vibrationMax = 0;
long vibrationMin = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(VIBRATION_PIN, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.display();
  delay(250);
  display.clearDisplay();
  display.display();
  delay(250);
}

void loop() {
  // The button is a reset: pulled low while held.
  if (!digitalRead(BUTTON_PIN)) {
    vibrationSum = 0;
    vibrationCount = 0;
    vibrationMax = 0;
    vibrationMin = 0;
    printOled("Ready");
    delay(200);
    return;
  }

  long pulseUs = pulseIn(VIBRATION_PIN, HIGH);
  if (pulseUs == 0) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  digitalWrite(LED_PIN, HIGH);

  if (vibrationCount > 10 && vibrationMin == 0) {
    vibrationMin = pulseUs;
  }
  if (pulseUs < vibrationMin) {
    vibrationMin = pulseUs;
  }
  if (pulseUs > vibrationMax) {
    vibrationMax = pulseUs;
  }

  vibrationSum += pulseUs;
  vibrationCount++;

  long average = vibrationSum / vibrationCount;

  char buffer[80];
  snprintf(buffer, sizeof(buffer), "COUNT: %ld\nAVG: %ld\nMIN: %ld\nMAX: %ld",
           vibrationCount, average, vibrationMin, vibrationMax);
  printOled(buffer);

  delay(50);
}

void printOled(const String &text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}
