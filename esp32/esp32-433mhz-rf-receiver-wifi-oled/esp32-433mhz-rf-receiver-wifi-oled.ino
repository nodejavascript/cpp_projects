/*
 * 433 MHz RF receiver with OLED and Wi-Fi
 *
 * Listens on a 433 MHz ASK receiver, decodes each packet with RadioHead,
 * formats it, and shows the last message on a 128x32 OLED while the Wi-Fi
 * stack stays ready to forward it.
 *
 * Board: ESP32-DevKitC / ESP32-WROOM-32U · 433 MHz receiver · SSD1306 128x32 OLED
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

// WIFI
// https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
// https://github.com/me-no-dev/AsyncTCP/archive/master.zip
#include <WiFi.h>

#include "config.h"

bool isWifiConnected = false;

// 433
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
RH_ASK driver(2000, 18, 2, 5); //(speed, receivePin, transmitPin, pushToTalkPin ? );
struct dataStruct{
  float brightness;
  unsigned long counter;
}myData;

int receivements = 0;

void setup(){

  Serial.begin(115200);

  // display
  setupDisplay();

  // wifi
  connectToWifi();

  // 433
  if (!driver.init()) {
    printOled("433 driver failed");
  }

}

void loop(){

  // wifi
  printOled( formattedMacAddress() + "\n" + formattedWifiConnectionStatus() + ": "+ formattedRSSI() + "\n"+ formattedIP() );
  delay(500);

//  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//
//
//
//
//
//
//    receivements += 1;
//

}
