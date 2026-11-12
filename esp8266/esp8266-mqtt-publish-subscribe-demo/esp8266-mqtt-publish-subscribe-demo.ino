/*
 * MQTT publish and subscribe demo
 *
 * The smallest round trip between an ESP8266 and a broker: connect, subscribe
 * to one topic, publish to it, and print whatever comes back. Use it to prove a
 * broker and a client can hear each other before building anything on top.
 *
 * Board: NodeMCU 1.0 / Wemos D1 mini
 * Copy config.h.example to config.h and fill in your own network and broker.
 */

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
}

// Everything is connected — subscribe first, then announce ourself.
void onConnectionEstablished() {
  client.subscribe(MQTT_TOPIC, [](const String &payload) {
    Serial.print("received: ");
    Serial.println(payload);
  });

  client.publish(MQTT_TOPIC, "This is a message");
}

// Publish our uptime every 50 ms so the topic has a steady pulse.
void publishUptime() {
  client.publish(MQTT_TOPIC, String(millis()));
  delay(50);
}

void loop() {
  client.loop();
  publishUptime();
}
