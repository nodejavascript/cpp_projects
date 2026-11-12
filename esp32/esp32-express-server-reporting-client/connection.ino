// Wi-Fi link management. The ESP32 blinks STATUS_LED_PIN while it searches,
// and restarts itself if the router never answers.

int getSignalStrength() {
  return WiFi.RSSI();
}

void ensureConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    // Nothing has answered after five seconds — a restart is cleaner than
    // sitting here forever, and the board comes straight back up.
    if (millis() > 5000) {
      resetBoard();
    }

    Serial.print(".");

    digitalWrite(STATUS_LED_PIN, LOW);
    delay(500);
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(500);
  }

  if (!isWifiConnected) {
    isWifiConnected = true;
    Serial.print(" connected: ");
    Serial.println(WiFi.localIP());
  }
}

void keepConnectionAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    isWifiConnected = false;
    ensureConnection();
  }
}
