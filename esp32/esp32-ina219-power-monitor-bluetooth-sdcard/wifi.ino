bool isConnected() { return (WiFi.status() == WL_CONNECTED ); }

void disconnectFromSSID () {
  WiFi.disconnect();
  delay(500);
}

void ensureWifiConnection () {
  if ( connectToWifi ) connectToSSID();
  if ( !connectToWifi ) disconnectFromSSID();
}

void configWifi (IPAddress local_IP, IPAddress gateway, IPAddress subnet, IPAddress primaryDNS, IPAddress secondaryDNS) {

  if ( useFixedIpAddress ) {

    if (Serial) Serial.println("local_IP" + String(local_IP));

    WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  }
}

void connectToSSID () {

  if ( isConnected() ) return;

  IPAddress subnet(WIFI_SUBNET);
  IPAddress primaryDNS(WIFI_DNS_PRIMARY);
  IPAddress secondaryDNS(WIFI_DNS_SECONDARY);

  if ( useHomeNetwork ) {
    ssid = WIFI_SSID_HOME;
    password = WIFI_PASS_HOME;
    postUrl = DATA_SERVER_URL;
    IPAddress local_IP(WIFI_IP_HOME);
    IPAddress gateway(192, 168, 43, 10);
    configWifi(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  } else {
    ssid = WIFI_SSID_ALT;
    password = WIFI_PASS_ALT;
    postUrl = DATA_SERVER_URL;
    IPAddress local_IP(WIFI_IP_ALT);
    IPAddress gateway(10, 0, 0, 1);
    configWifi(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  waitForConnection();
}

void waitForConnection() {
  int secondsLeft = waitForWifiTimeout;
  while ( !isConnected() ) {
    if (secondsLeft == 0) {
      break;
    }
    printOled( formattedWifiConnectionStatus() + "\nTimeout in " + String(secondsLeft) +" s");
    delay(1000);
    secondsLeft -= 1;
  }
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

const String formattedIP() {
  IPAddress ipAddress = WiFi.localIP();
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]) ;
}

const String formattedRSSI() {
  return String(WiFi.RSSI()) +" dBm";
}

const String formattedMacAddress() {
  byte mac[6];
  WiFi.macAddress(mac);
  String macAddress = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);
  macAddress.toUpperCase();
  return macAddress;
}
