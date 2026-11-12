/*
 * ESP8266 weather station key fob
 *
 * The fullest of the ESP8266 builds: BME280 temperature, humidity and
 * pressure, a DS3231 real-time clock, SD-card logging, an OLED that pages
 * through the readings, HTTP endpoints for a phone to poll, and Wi-Fi that
 * reconnects by itself. A button cycles the display and triggers a fresh log
 * line, and it can switch between two networks at run time.
 *
 * Board: NodeMCU 1.0 (ESP-12E) · BME280 · DS3231 RTC · SD card · SSD1306 OLED · push button
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
String cr = "\n";

// wifi
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

bool useAlternateNetwork = true;

AsyncWebServer server(80);

// TEMPERATURE
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1003.40)
Adafruit_BME280 bme;

// DS3231
#include <TimeLib.h>
#include "RTClib.h"

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h
RTC_DS3231 rtc;

// GPIO
const int PIN_TOGGLE_WIFI = 2;

const int PIN_ALARM__RED = 3;
const int PIN_LOCK___YELLOW = 0;
const int PIN_UNLOCK_GREEN = 1;

String fobAction = "";
String lastFobAction = "";

#include <ArduinoJson.h>

// sd card
#include <SPI.h>
#include <SD.h>
const int chipSelect = D4;
File logFile;

int logCounter = 0;

String graphqlEndpoint = DATA_SERVER_URL;
String accessKey = DATA_ACCESS_KEY;

void setup(){

  Serial.begin(115200);

  // oled
  setupOled();

  // wifi
  connectToSSID();

  createEndpoints();
  server.onNotFound(notFound);
  server.begin();

  // temperature
  unsigned bmp280;
  bmp280 = bme.begin(0x76);

  // DS3231
  while ( !syncRealTimeClock() );

  pinMode(PIN_TOGGLE_WIFI, INPUT_PULLUP);

  pinMode(PIN_ALARM__RED, OUTPUT);
  pinMode(PIN_LOCK___YELLOW, OUTPUT);
  pinMode(PIN_UNLOCK_GREEN, OUTPUT);

  digitalWrite(PIN_ALARM__RED, LOW);
  digitalWrite(PIN_LOCK___YELLOW, LOW);
  digitalWrite(PIN_UNLOCK_GREEN, LOW);

  // client
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop(){

//
//
  String log = "";

  log += getISOString() + ",";
  log += String( temperature() ) + ",";
  log += String( humidity() ) + ",";
  log += String( pressure() ) + ",";
  log += String( altitude() );

  digitalWrite(LED_BUILTIN, LOW);
  String recordResponse = postRecordToServer( log );
  digitalWrite(LED_BUILTIN, HIGH);

  printOled( "RESPONSE:" + recordResponse + "\n" + log );

  delay(1000 * 60);

//
//  logCounter += 1;
//
//
//  ui += String(logCounter);
//
//

}

void singleClick(int PIN) {

  if (fobAction.length() > 0) {
    lastFobAction = fobAction;
  }

  fobAction = "";
  digitalWrite(PIN, HIGH);
  delay(500);
  digitalWrite(PIN, LOW);
}

void doubleClick(int PIN) {
  singleClick(PIN);
  delay(500);
  singleClick(PIN);
}
