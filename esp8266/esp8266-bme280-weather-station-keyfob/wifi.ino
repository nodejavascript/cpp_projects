
void flipConnection () {
  useAlternateNetwork = !useAlternateNetwork;
  WiFi.disconnect();
  delay(500);
  connectToSSID();
}

void connectToSSID() {

  IPAddress dns1(WIFI_DNS_PRIMARY);
  IPAddress dns2(WIFI_DNS_SECONDARY);
  IPAddress subnet(WIFI_SUBNET);

  if ( useAlternateNetwork ) {

    IPAddress local_IP(WIFI_IP_ALT);
    IPAddress gateway(10, 0, 0, 1);
    WiFi.config(local_IP, gateway, subnet, dns1, dns2);
    WiFi.begin(WIFI_SSID_ALT, WIFI_PASS_ALT);

  } else {

    IPAddress local_IP(WIFI_IP_HOME);
    IPAddress gateway(192, 168, 43, 10);
    IPAddress subnet(WIFI_SUBNET);
    WiFi.config(local_IP, gateway, subnet, dns1, dns2);

    WiFi.begin(WIFI_SSID_HOME, WIFI_PASS_HOME);
  }

  connectToWifi();
}

bool isConnected() { return (WiFi.status() == WL_CONNECTED ); }

bool connectToWifi() {
  int tryCount = 0;
  while ( !isConnected() ) {
    tryCount += 1;
    printOled(formattedWifiConnectionStatus() + cr + String(tryCount) );
    delay(1000);
  }
  return true;
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
  String macAddress = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);
  return macAddress;
}

const String formattedIP(IPAddress ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]) ;
}

const String formattedRSSI() {
  return String(WiFi.RSSI()) +" dBm";
}

const String formattedRSSIStength() {
  int rrsi = WiFi.RSSI();
  String stength = "Horrible";
  if (rrsi > -100) stength = "Very Low";
  if (rrsi > -90) stength = "Low";
  if (rrsi > -80) stength = "Good";
  if (rrsi > -70) stength = "Very good";
  if (rrsi > -60) stength = "Excellent";
  if (rrsi == 0) stength = "No Signal";
  return stength;
}
