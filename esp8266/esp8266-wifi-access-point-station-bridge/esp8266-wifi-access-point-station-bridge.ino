/*
 * Wi-Fi access point and station at once
 *
 * Runs the ESP8266 in WIFI_AP_STA: it joins the home network as a client and
 * serves its own access point at the same time, on a fixed address so it is
 * always reachable. The usual use is a small bridge — a device with no Wi-Fi of
 * its own talks to this access point, and this board forwards it.
 *
 * Board: any ESP8266 — NodeMCU or Wemos D1 mini
 * Copy config.h.example to config.h and fill in your own values.
 */

#include <ESP8266WiFi.h>

#include "config.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting access point and station");

  // Serve our own network first, so it is up while the home link is still forming.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  IPAddress subnet(WIFI_SUBNET);
  IPAddress dns1(WIFI_DNS1);
  IPAddress dns2(WIFI_DNS2);
  IPAddress localIp(WIFI_IP);
  IPAddress gateway(WIFI_GATEWAY);
  WiFi.config(localIp, gateway, subnet, dns1, dns2);

  Serial.print("Joining ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Station address on ");
  Serial.print(WIFI_SSID);
  Serial.print(": ");
  Serial.println(WiFi.localIP());

  Serial.print("Access point address on ");
  Serial.print(AP_SSID);
  Serial.print(": ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Nothing to do — the Wi-Fi stack runs on its own.
}
