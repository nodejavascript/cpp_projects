/*
 * BMP280 temperature, pressure and altitude to MQTT
 *
 * Reads a BMP280 on I2C address 0x76 once a second and publishes three separate
 * topics, so a dashboard can chart temperature without parsing a combined
 * payload. EspMQTTClient handles Wi-Fi and broker reconnection for us.
 *
 * Board: NodeMCU / Wemos D1 mini
 * Copy config.h.example to config.h and fill in your own network and broker.
 */

#include <Adafruit_BMP280.h>
#include <EspMQTTClient.h>

#include "config.h"

// Local sea-level pressure, used only to convert pressure into an altitude.
#define SEALEVELPRESSURE_HPA 1013.0

Adafruit_BMP280 roomSensor;

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

  if (!roomSensor.begin(0x76)) {
    Serial.println("BMP280 not found at 0x76 — check the wiring");
    while (true) {
      delay(1000);
    }
  }
}

void publishReadings() {
  client.publish("sensors/" SENSOR_NAME "/temperature", String(roomSensor.readTemperature()));
  client.publish("sensors/" SENSOR_NAME "/pressure", String(roomSensor.readPressure() / 100.0F));
  client.publish("sensors/" SENSOR_NAME "/altitude", String(roomSensor.readAltitude(SEALEVELPRESSURE_HPA)));
  delay(1000);
}

void onConnectionEstablished() {
  // Nothing to do once: the readings are published from loop().
}

void loop() {
  client.loop();
  publishReadings();
}
