
const String formatIsWifiConnected() {
  return String(isWifiConnected);
}

const String formattedWifiConnectionStatus() {
  switch ( WiFi.status() ) {
    case WL_NO_SHIELD: return "NO_SHIELD";
    case WL_IDLE_STATUS: return "IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
    case WL_CONNECTED: return "CONNECTED";
    case WL_CONNECT_FAILED: return "CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION_LOST";
    case WL_DISCONNECTED: return "DISCONNECTED";
    default: return "UNKNOWN";
  }
}

const String formattedMacAddress() {
  byte mac[6];
  WiFi.macAddress(mac);
  return String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);
}

const String formattedIP() {
  IPAddress ipAddress = WiFi.localIP();

  return "IP:" + String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]) ;
}

const String formattedRSSI() {
  int rrsi = WiFi.RSSI();

  String text = "RSSI:" + String(rrsi) +" dBm";
  String strengthLanguage = "Horrible";

  if (rrsi > -100) strengthLanguage = "Very Low";
  if (rrsi > -90) strengthLanguage = "Low";
  if (rrsi > -80) strengthLanguage = "Good";
  if (rrsi > -70) strengthLanguage = "Very good";
  if (rrsi > -60) strengthLanguage = "Excellent";
  if (rrsi == 0) strengthLanguage = "No Signal";

  text += "\n" + strengthLanguage;

  return String(text);
}

void printOled (String text) {
  // text display tests
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(text);
  display.setCursor(0,0);
  display.display();
}

void printSerial (String text) {
  if (Serial) Serial.println( text );
}
