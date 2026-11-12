/*
 * Truck key fob remote over Wi-Fi
 *
 * A web page served from the ESP32 that posts a command to the vehicle's
 * key-fob wiring, so the truck can be unlocked from a browser on the same
 * network instead of from inside the house. Reports the last command and its
 * result, and keeps a fixed address on either of two networks.
 *
 * Board: ESP32-WROOM-32U (DOIT DevKit V1) · relay or opto interface to the vehicle
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h

char* ssid;
char* password;

bool connectToWifi = true;
bool useHomeNetwork = false;
bool useFixedIpAddress = true;
int waitForWifiTimeout = 15;

#define pinAlarm 12
#define pinUnlock 14
#define pinLock 27

AsyncWebServer server(80);
String htmlAction = "";

void setup() {

  Serial.begin(115200);

  ensureWifiConnection();

  createEndpoints();
  server.onNotFound(notFound);
  server.begin();

  pinMode(pinUnlock, OUTPUT);
  pinMode(pinLock, OUTPUT);
  pinMode(pinAlarm, OUTPUT);

}

void webServer () {
  if (htmlAction.length() > 0) {

    if (Serial) Serial.println("htmlAction " + htmlAction);

    if (htmlAction == "unlock") singleClick(pinUnlock);
    if (htmlAction == "unlockAll") doubleClick(pinUnlock);
    if (htmlAction == "lock") singleClick(pinLock);
    if (htmlAction == "lockAndArm") doubleClick(pinLock);
    if (htmlAction == "alarm") singleClick(pinAlarm);

    htmlAction = "";
  }
}

void singleClick (int pin) {
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(200);
}

void doubleClick (int pin) {
  singleClick(pin);
  singleClick(pin);
}

void loop() {

  ensureWifiConnection();

  webServer();

//

}
