/*
 * ESP32 reporting client for an Express server
 *
 * A small device that keeps a Wi-Fi link to a server and POSTs status to it.
 * It also serves three endpoints of its own:
 *
 *   /            a liveness check
 *   /ping        the uptime in milliseconds
 *   /authorize   changes the server address, but only for a caller that
 *                presents AUTH_TOKEN
 *
 * Board: ESP32 (DevKitC / WROOM-32)
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>

#include "config.h"

AsyncWebServer server(80);

String expressServerUrl = EXPRESS_SERVER_URL;
String authToken = AUTH_TOKEN;

bool isWifiConnected = false;

// Watchdog: if the link cannot be established within the timeout, restart.
void (*resetBoard)(void) = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(STATUS_LED_PIN, OUTPUT);

  ensureConnection();

  postToServer("testing");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "It works!");
  });

  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(millis()));
  });

  server.on("/authorize", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("token") || !request->hasParam("expressServerURL")) {
      request->send(400, "text/plain", "token and expressServerURL are required");
      return;
    }

    if (request->getParam("token")->value() != authToken) {
      Serial.println("authorize: token rejected");
      request->send(403, "text/plain", "invalid token");
      return;
    }

    expressServerUrl = request->getParam("expressServerURL")->value();
    Serial.println("authorize: server is now " + expressServerUrl);
    request->send(200, "text/plain", "success");
  });

  server.begin();
  Serial.println("Server started");
}

void postToServer(String payload) {
  HTTPClient http;

  String url = expressServerUrl + "/arduino";
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int code = http.POST("postData=" + payload);

  Serial.print("POST ");
  Serial.print(url);
  Serial.print(" -> ");
  Serial.println(code);

  if (code > 0) {
    Serial.println(http.getString());
  }

  http.end();
}

void loop() {
  keepConnectionAlive();
}
