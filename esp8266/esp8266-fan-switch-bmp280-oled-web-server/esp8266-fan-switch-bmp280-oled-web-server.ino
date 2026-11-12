/*
 * Fan switch with BMP280, OLED and web control
 *
 * Reads a BMP280, drives a fan relay and serves a web page with on, off and
 * automatic control. The OLED shows the temperature and the current state,
 * and every request is kept in a status record so the last command and its
 * result are visible after a reload.
 *
 * Board: NodeMCU 1.0 (ESP-12E) · BMP280 · SSD1306 OLED · relay or MOSFET on the fan
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include <ArduinoJson.h>

AsyncWebServer server(80);
String htmlAction = "";

// oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

#include <Adafruit_BMP280.h>

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h
#define SEALEVELPRESSURE_HPA (1003.40)
Adafruit_BMP280 heaterSensor;
Adafruit_BMP280 roomSensor;

const int isOnLedPin = 2;
const int onOffSwitchPin = 15;
const int fanPin = 14;
const int vibrationSensorPin = 12;
const int heaterPin = 13;

// settings
bool connectToWifi = true;
bool useAlternateNetwork = true;
bool useFixedIpAddress = false;

float fanTempTrigger = 24;
bool itsHot = false;
bool isFanOn = false;

float minRoomTemp = 19.5;
float maxRoomTemp = 21.5;

double heaterTemp;
double roomTemp;

int vibration;
int vibrationCounter = 0;
int notVibrationCounter = 0;

bool heaterIsOn = false;
bool manualHeatOn = false;
bool manualHeatOff = false;

int switchOn = 0;

bool fault = false;
bool sentFaultToSlack = false;

void setup(){

  Serial.begin(115200);

  Wire.begin();

  setupOled();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("  " + formattedRSSI() );
  display.setTextSize(1);
  display.print("\n");
  display.print("\n");
  display.print("\n");
  display.print("\n");
  display.print("     CONNECTING");
  display.display();

  ensureWifiConnection();
  createEndpoints();
  server.onNotFound(notFound);
  server.begin();

  clientPostSlack("fan_switch > Started\nhttp://" + formattedIP() );

  unsigned bmp280;
  bmp280 = heaterSensor.begin(0x77);
  unsigned bmp280b;
  bmp280b = roomSensor.begin(0x76);

  pinMode(onOffSwitchPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(isOnLedPin, OUTPUT);

  digitalWrite(heaterPin, LOW);
}

void loop(){

  ensureWifiConnection();

  webServer();

  heaterTemp = heaterSensor.readTemperature();
  roomTemp = roomSensor.readTemperature();

  switchOn = digitalRead(onOffSwitchPin);

  if (switchOn) {
    digitalWrite(isOnLedPin, HIGH);
  } else {
    digitalWrite(isOnLedPin, LOW);
  }

  if ( heaterTemp < 10 || heaterTemp > 30 || roomTemp < 10 || roomTemp > 30) {
    fault = true;
  } else {
    fault = false;
  }

  if (fault && !sentFaultToSlack){
    sentFaultToSlack = true;
    sendDetails();
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  if (fault) {

    display.setTextSize(2);
    display.print(" - FAULT -");

    display.setTextSize(1);
    display.print("\n");
    display.print("\n");
    display.print("\n");
    display.print("\n");

    display.print("roomTemp   : " + String (roomTemp));
    display.print("\n");
    display.print("\n");
    display.print("heaterTemp : " + String (heaterTemp));

  } else {
    display.setTextSize(2);
    display.print("   " + String (roomTemp ));
    display.setTextSize(1);

    if (switchOn) {
      display.print(" ON");
    } else {
      display.print(" OFF");
    }
    display.print("\n");
    display.print("\n");
    display.print("\n");

    if (heaterIsOn) {
      display.print("       ON  ");
    } else {
      display.print("      OFF  ");
    }

    display.print(String(heaterTemp));

    display.print("\n");
    display.print("\n");
    display.print("    " + String(minRoomTemp) + " <> " + String(maxRoomTemp) );
    display.print("\n");
    display.print("\n");
    display.print("      " + formattedIP() );
  }

  display.display();

  if (switchOn && roomTemp < minRoomTemp) turnHeatOn();
  if (switchOn && manualHeatOn) turnHeatOn();

  if (fault) turnHeatOff();
  if (!switchOn) turnHeatOff();
  if (manualHeatOff) turnHeatOff();
  if (heaterIsOn && roomTemp > maxRoomTemp) turnHeatOff();

  if (!switchOn && manualHeatOn) {
    clientPostSlack("fan_switch > Cant turn on. Switch if Off.");
    manualHeatOn = false;
  }

  handleFan();

  delay(500);

}

void sendDetails () {
  String ui = "fan_switch >";
  ui += "\n";
  ui += "switchOn: " + String(switchOn);
  ui += "\n";
  ui += "fault: " + String(fault);
  ui += "\n";
  ui += "roomTemp: " + String(roomTemp);
  ui += "\n";
  ui += "heaterTemp: " + String(heaterTemp);
  ui += "\n";
  ui += "fanTempTrigger: " + String(fanTempTrigger);
  ui += "\n";
  ui += "minRoomTemp: " + String(minRoomTemp);
  ui += "\n";
  ui += "maxRoomTemp: " + String(maxRoomTemp);
  ui += "\n";
  ui += "itsHot: " + String(itsHot);
  ui += "\n";
  ui += "isFanOn: " + String(isFanOn);
  ui += "\n";
  ui += "heaterIsOn: " + String(heaterIsOn);
  ui += "\n";
  ui += "http://" + String( formattedIP() );

  clientPostSlack(ui);

}

void turnHeatOn() {
  if (!heaterIsOn) {
    Serial.println("turnHeatOn");
    digitalWrite(heaterPin, HIGH);
    heaterIsOn = true;
    manualHeatOn = false;
    manualHeatOff = false;
  }
}
void turnHeatOff() {
  if (heaterIsOn) {
    Serial.println("turnHeatOff");
    digitalWrite(heaterPin, LOW);
    heaterIsOn = false;
    manualHeatOn = false;
    manualHeatOff = false;
  }
}

void handleFan() {
  if (!itsHot && heaterTemp > fanTempTrigger) itsHot = true;
  if (itsHot && heaterTemp < fanTempTrigger) itsHot = false;

  vibration = pulseIn(vibrationSensorPin, HIGH);

  if (abs(vibration) > 0) {
    vibrationCounter ++;
    notVibrationCounter = 0;
  } else {
    vibrationCounter = 0;
    notVibrationCounter ++;;
  }

  if (Serial) Serial.println( String(vibrationCounter) + "," + String(notVibrationCounter) );

  if (vibrationCounter > 5) isFanOn = true;
  if (notVibrationCounter > 5) isFanOn = false;

  if (!itsHot && isFanOn) turnFanOff();
  if (itsHot && !isFanOn) turnFanOn();
}

void webServer () {
  if (htmlAction.length() > 0) {

    if (htmlAction == "turnHeaterOn") manualHeatOn = true;
    if (htmlAction == "turnHeaterOff") manualHeatOff = true;
    if (htmlAction == "details") sendDetails();

    htmlAction = "";
  }
}

void turnFanOff () {
  clickFanSwitch();
  isFanOn = false;
  vibrationCounter = 0;
  notVibrationCounter = 0;
}

void turnFanOn () {
  clickFanSwitch();
  clickFanSwitch();
  isFanOn = true;
  vibrationCounter = 0;
  notVibrationCounter = 0;
}

void clickFanSwitch () {
  digitalWrite(fanPin, HIGH);
  delay(50);
  digitalWrite(fanPin, LOW);
  delay(1000);
}
