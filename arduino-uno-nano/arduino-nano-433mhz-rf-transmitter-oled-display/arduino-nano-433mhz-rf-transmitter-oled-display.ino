/*
 * 433 MHz RF transmitter with an OLED readout
 *
 * Reads a photocell on A7, converts it to a brightness percentage, and sends
 * that plus a packet counter over a 433 MHz ASK module. The OLED shows the value
 * as it leaves, so a transmitter that has quietly stopped sending is obvious.
 *
 * Board: Arduino Nano (ATmega328P, old bootloader) — A7 exists on the Nano only
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>   // RadioHead needs it compiled even when unused
#endif

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  32
#define OLED_RESET     -1
#define OLED_ADDRESS   0x3C

#define PHOTOCELL_PIN  A7
#define PHOTOCELL_DARK  120     // raw reading in darkness
#define PHOTOCELL_BRIGHT 700    // raw reading in full light

#define TX_SPEED       2000
#define TX_PIN         2
#define TX_RX_PIN      4
#define TX_PTT_PIN     5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RH_ASK driver(TX_SPEED, TX_RX_PIN, TX_PIN, TX_PTT_PIN);

// The exact bytes that go over the air. Keep transmitter and receiver identical.
struct Reading {
  float brightness;
  unsigned long counter;
};

Reading reading;

void setup() {
  Serial.begin(9600);

  setupDisplay();

  if (!driver.init()) {
    printOled("433 driver failed");
  }
}

void loop() {
  int raw = analogRead(PHOTOCELL_PIN);
  int mapped = map(raw, PHOTOCELL_DARK, PHOTOCELL_BRIGHT, 0, 1000);

  reading.brightness = mapped / 10.0;
  reading.counter++;

  uint8_t buffer[sizeof(reading)];
  memcpy(buffer, &reading, sizeof(reading));

  driver.send(buffer, sizeof(reading));
  driver.waitPacketSent();

  printOled("Brightness: " + String(reading.brightness, 1) +
            "\nCounter: " + String(reading.counter));

  delay(100);
}

bool setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 not found");
    return false;
  }
  display.clearDisplay();
  display.display();
  return true;
}

void printOled(const String &text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}
