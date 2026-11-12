/*
 * Laundry done notifier
 *
 * Watches a vibration input, learns what a running machine looks like, and
 * calls an HTTP endpoint once the motion stops for long enough. Retries on
 * failure and reports the last response code, so a missed notification is
 * visible rather than silent.
 *
 * Board: NodeMCU / Wemos D1 mini · vibration or accelerometer input
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h

// settings
bool connectToWifi = true;
bool useAlternateNetwork = true;
bool useFixedIpAddress = true;

const int pinDryer = 12;
const int pinWasher = 14;

int vibrationNowDryer = 0;
int vibrationNowWasher = 0;

int vibratingCounterDryer = 0;
int vibratingCounterWasher = 0;

int notVibratingCounterDryer = 0;
int notVibratingCounterWasher = 0;

bool currentlyVibratingDryer = false;
bool currentlyVibratingWasher = false;

const int vibrationTimeoutMilli = 1500 * 1000;
const int vibrationCountLimit = 5;

void setup(){

  Serial.begin(115200);

  if (Serial) Serial.println("formattedRSSI: " + String(formattedRSSI()) );

  ensureWifiConnection();

  pinMode(pinDryer, INPUT);
  pinMode(pinWasher, INPUT);

  clientPostSlack("laundry_notifier started");

}

void loop(){

  ensureWifiConnection();

  vibrationNowDryer = pulseInLong(pinDryer, HIGH, vibrationTimeoutMilli);
  vibrationNowWasher = pulseInLong(pinWasher, HIGH, vibrationTimeoutMilli);

  if (abs(vibrationNowDryer) > 0) {
    vibratingCounterDryer ++;
    notVibratingCounterDryer = 0;
  } else {
    vibratingCounterDryer = 0;
    notVibratingCounterDryer ++;;
  }

  if (abs(vibrationNowWasher) > 0) {
    vibratingCounterWasher ++;
    notVibratingCounterWasher = 0;
  } else {
    vibratingCounterWasher = 0;
    notVibratingCounterWasher ++;;
  }

  if (currentlyVibratingDryer & notVibratingCounterDryer > vibrationCountLimit) {
    currentlyVibratingDryer = false;
    clientPostSlack("Dryer STOPPED");
  }

  if (!currentlyVibratingDryer & vibratingCounterDryer > vibrationCountLimit) {
    currentlyVibratingDryer = true;
    clientPostSlack("Dryer STARTED");
  }

  if (currentlyVibratingWasher & notVibratingCounterWasher > vibrationCountLimit) {
    currentlyVibratingWasher = false;
    clientPostSlack("Washer STOPPED");
  }

  if (!currentlyVibratingWasher & vibratingCounterWasher > vibrationCountLimit) {
    currentlyVibratingWasher = true;
    clientPostSlack("Washer STARTED");
  }

  if (Serial) {
    Serial.print("D:");
    Serial.print(currentlyVibratingDryer);
    Serial.print(",");
    Serial.print(vibrationNowDryer);
    Serial.print(",");
    Serial.print(vibratingCounterDryer);
    Serial.print(",");
    Serial.print(notVibratingCounterDryer);
    Serial.print(",");
    Serial.print("W:");
    Serial.print(currentlyVibratingWasher);
    Serial.print(",");
    Serial.print(vibrationNowWasher);
    Serial.print(",");
    Serial.print(vibratingCounterWasher);
    Serial.print(",");
    Serial.print(notVibratingCounterWasher);

    Serial.println("");
  }

}
