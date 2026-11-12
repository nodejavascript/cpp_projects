/*
 * ESP8266 asynchronous web server with a JSON API
 *
 * Serves an async web server that accepts JSON POSTs, parses them with
 * ArduinoJson, and can forward a payload onward as an HTTP client. Every
 * request and response is recorded so the last call, its code, its timing
 * and its error can be read from a browser.
 *
 * Board: NodeMCU 0.9 / ESP-12 · ESP8266 core 3.x
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>

#include "config.h"   // Wi-Fi credentials — copy config.h.example to config.h

struct httpStruct {
  String httpMethod;
  String httpUrl;
  String httpResponse;
  String httpError;
  int httpResponseTime;
  int httpCode;
} httpResponse;

AsyncWebServer server(80);

bool connectToWifi = true;
bool useAlternateNetwork = true;
bool useFixedIpAddress = true;
int waitForWifiTimeout = 15;

int red = 12;
int green = 13;
int blue = 14;

String postAccessKey = "";
String postPayload = "";

void setup(){

  Serial.begin(115200);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  connectToSSID();

  createEndpoints();

}

void loop(){

  ensureWifiConnection();

  if (postAccessKey.length() > 0 && postPayload.length() > 0) {

    digitalWrite(blue, HIGH);

    String tempAccessKey = postAccessKey;
    String tempPayload = postPayload;

    clientPost(DATA_SERVER_URL, tempAccessKey, tempPayload);

    postAccessKey = "";
    postPayload = "";

    digitalWrite(blue, LOW);

  }

}
