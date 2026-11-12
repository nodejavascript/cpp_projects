

bool connectToWifi() {

  if (!isWifiConnected) {
    printOled("CONNECTING...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000);
  }

  if (isWifiConnected && WiFi.RSSI() == 0) {

    isWifiConnected = false;
    connectToWifi();

  } else {

    while (WiFi.status() != WL_CONNECTED) {
      if (millis() > 10000) {
        printOled("Restarting...");
        ESP.restart();
      }
    }

  }

  if (!isWifiConnected) {
    printOled("CONNECTED");
    delay(1000);
    isWifiConnected = true;
  }

  return true;
}
