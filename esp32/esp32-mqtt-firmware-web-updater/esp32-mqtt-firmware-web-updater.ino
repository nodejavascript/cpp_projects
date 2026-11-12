/*
 * MQTT client with an HTTP firmware updater
 *
 * Connects to Wi-Fi and a broker, then switches on EspMQTTClient's built-in web
 * updater so a new binary can be pushed to the board over the network instead of
 * unplugging it. A last-will message tells the broker if the device falls off.
 *
 * Board: ESP32 (DevKitC / WROOM-32)
 * Copy config.h.example to config.h and fill in your own network and broker.
 */

#include <WiFi.h>                  // EspMQTTClient assumes this on ESP32 core 3.x
#include <ESP32HTTPUpdateServer.h>
#include <EspMQTTClient.h>

#include "config.h"

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_HOST,
  MQTT_USER,
  MQTT_PASSWORD,
  MQTT_CLIENT_ID,
  MQTT_PORT
);

void setup() {
  Serial.begin(115200);

  client.enableDebuggingMessages();

  // The updater signs in with the MQTT username and password.
  client.enableHTTPWebUpdater();

  client.enableLastWillMessage(MQTT_TOPIC "/lastwill", "I am going offline");
}

void onConnectionEstablished() {
  client.subscribe(MQTT_TOPIC, [](const String &payload) {
    Serial.println(payload);
  });

  client.subscribe("mytopic/wildcardtest/#", [](const String &topic, const String &payload) {
    Serial.println("(wildcard) topic: " + topic + ", payload: " + payload);
  });

  client.publish(MQTT_TOPIC, "This is a message");

  // Prove the delayed path works: one extra publish five seconds after connect.
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "Sent five seconds later");
  });
}

void loop() {
  client.loop();
}
