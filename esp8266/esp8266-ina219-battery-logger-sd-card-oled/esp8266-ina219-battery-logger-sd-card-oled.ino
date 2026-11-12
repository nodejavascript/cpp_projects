/*
 * INA219 battery logger with SD card, RTC and OLED
 *
 * Two INA219 sensors — one on the battery terminals, one on the 5 V source —
 * log voltage, current and power to a CSV on an SD card, timestamped from a
 * DS3231. The clock re-syncs from NTP over HTTPS and records whether that
 * succeeded, and everything is on the OLED and over serial.
 *
 * Board: NodeMCU 0.9 (ESP-12) · 2 x INA219 · DS3231 RTC · SD card module · SSD1306 OLED
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

// SD CARD
#include <SPI.h>
#include <SD.h>

// INA219
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219_batteryTerminals(0x40);
Adafruit_INA219 ina219_usb5vSource(0x41);

// CLOCK
#include <TimeLib.h>
#include "RTClib.h"
RTC_DS3231 rtc;

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// http
bool wifiConnected = false;
long connectionWaitTime = 0;
String httpMethod;
String httpUrl;
String httpResponse;
String httpError;
long httpResponseTime;
int httpCode;

// clock
bool lostPower = false;
String clockBeforeSync = "";
bool synced = false;
bool attemptNTP = false;
String statusMessage = "";
long clockWaitTime = 0;

// log file
String filename = "LOGS.TXT";
int fileSize;
int records;
long fileWaitTime;

// ina
String ISOString = "";
float ina219_usb5vSource_busVoltage  = 0;
float ina219_usb5vSource_shuntVoltage = 0;
float ina219_usb5vSource_loadVoltage = 0;
float ina219_usb5vSource_loadMilliAmps = 0;
float ina219_usb5vSource_loadMilliWatts = 0;
float ina219_batteryTerminal_busVoltage  = 0;
float ina219_batteryTerminal_shuntVoltage = 0;
float ina219_batteryTerminal_loadVoltage = 0;
float ina219_batteryTerminal_loadMilliAmps = 0;
float ina219_batteryTerminal_loadMilliWatts = 0;

// serial
String serialString = "";
String serialKey = "";
String serialValue = "";
bool hasEquals = false;

// settings
bool LIVE_MODE = false;
bool connectToWifi = true;
bool useAlternateNetwork = true;
bool useFixedIpAddress = false;
bool forceClockSync = false;

const String postURL = DATA_SERVER_URL;
String postAccessKey = DATA_ACCESS_KEY;
String batteryLabelId = "18650B3";
String batteryLabelVoltage = "4.7";
String batteryLabelType = "Blue - 18650 #3";
int readingDelay = 5000;

void setup(){
  Serial.begin(115200);
  if (Serial) Serial.println("\n\n\n\n\n\nSTART");

  Wire.begin();

  connectToWifi = true;
  ensureWifiConnection();
  setupClock();
  setupIna();
  syncClock();
  postHeader();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){

  serialListener();
  ensureWifiConnection();

  if (LIVE_MODE) {

    digitalWrite(LED_BUILTIN, LOW);

    takeReading();
    postReading();

  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(readingDelay);

}
