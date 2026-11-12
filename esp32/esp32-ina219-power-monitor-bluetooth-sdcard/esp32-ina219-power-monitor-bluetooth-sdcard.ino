/*
 * ESP32 power monitor with Bluetooth, SD card and OLED
 *
 * INA219 voltage and current logged to an SD card with an RTC timestamp,
 * shown on an SSD1306 and exposed over Bluetooth Serial, so a phone can poll
 * it on the bench without a laptop. Same measurement chain as the ESP8266
 * logger, with Bluetooth added.
 *
 * Board: ESP32-WROOM-32U (DOIT DevKit V1) · INA219 · DS3231 RTC · SD card · SSD1306 OLED
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, 4);

// CLOCK
#include <TimeLib.h>
#include "RTClib.h"
RTC_DS3231 rtc;

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// Adafruit_INA219
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219_usb5vSource(0x41);
Adafruit_INA219 ina219_batteryTerminals(0x40);

// sd card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h

struct fileStruct {
  int totalMb;
  int usedMb;
  int fileSizeB;
  double fileSizeKB;
  int lines;
  int lineLength;
  int processingMillis;
  double processingBytesPerMilli;
  double processingLinesPerMilli;
} logStats;
byte buf1[sizeof(logStats)] = {0};

struct readingStruct {
  String batteryLabelId = "4";
  String batteryLabelVotage = "9";
  String batteryLabelType = "LiPo";
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
} reading;
byte buf2[sizeof(reading)] = {0};

char* ssid;
char* password;

char* accessKey = DATA_ACCESS_KEY;
char* postUrl;

bool connectToWifi = true;
bool useHomeNetwork = false;
bool useFixedIpAddress = false;
int waitForWifiTimeout = 15;
String BTString;

bool eraseLog = false;
bool saveLog = false;
bool postLog = true;
bool showVoltages = false;
bool plotVoltages = false;
int pollingDelay = 5000;

bool syncClock = false;

const char* logFile = "/LOGS.TXT";

void setup() {

  Serial.begin(115200);

  setupOLED();

  ensureWifiConnection();

  setupIna();

  setupBluetooth();

  setupSD();

  refreshlogStats(SD);

}

int counter = 0;

void loop() {

  delay(pollingDelay);

  ensureWifiConnection();

  if (eraseLog) {
    eraseLogFile(SD);
    eraseLog = false;
  }

  String ui = "";
  if ( isConnected() ) {
    ui += "CONNECTED: " + formattedRSSI();
  } else {
    ui += "DISCONNECTED";
  }
  ui += "\n";
  ui += getDateNow() + " " + getTimeNow();
  ui += "\n";
//  ui += "Hotspot: " + String(useHomeNetwork);
//  ui += "\n";
//  ui += "FixedIp: " + String(useFixedIpAddress);
//  ui += "\n";
//  ui += "Polling: " + String(pollingDelay)  + "ms";
//  ui += "\n";
  ui += "cardTotal: " + String(logStats.totalMb) + " mb";
  ui += "\n";
  ui += "cardUsed: " + String(logStats.usedMb) + " mb";
  ui += "\n";
  ui += "logFile: " + String(logStats.fileSizeKB) + " kb";
  ui += "\n";
  ui += "lines: " + String(logStats.lines);

  if (!showVoltages && !postLog) printOled( ui );

  takeReading();

  counter ++;

  if (postLog && counter < 5) postReading();

  if (saveLog) saveReading();
  if (showVoltages) displayReading();
  if (plotVoltages) plotReading();

}
