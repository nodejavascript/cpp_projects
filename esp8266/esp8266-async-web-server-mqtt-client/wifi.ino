
void(* resetFunc) (void) = 0;

bool ensureWifiConnection () {
  if ( !isConnected() ) connectToSSID();
  return true;
}

bool connectToSSID() {

  if ( isConnected() ) return true;

  if ( !connectToWifi ) {
    WiFi.disconnect();
    while ( !isConnected() )
    return false;
  }

  IPAddress subnet(WIFI_SUBNET);
  IPAddress dns1(WIFI_DNS_PRIMARY);
  IPAddress dns2(WIFI_DNS_SECONDARY);

  String ssid = "";
  String password = "";

  if ( useAlternateNetwork ) {

    ssid = WIFI_SSID_ALT;
    password = WIFI_PASS_ALT;

    if (useFixedIpAddress) {
      IPAddress local_IP(WIFI_IP_ALT);
      IPAddress gateway(10, 0, 0, 1);
      WiFi.config(local_IP, gateway, subnet, dns1, dns2);
    }

  } else {

    ssid = WIFI_SSID_HOME;
    password = WIFI_PASS_HOME;

    if (useFixedIpAddress) {
      IPAddress local_IP(WIFI_IP_HOME);
      IPAddress gateway(192, 168, 43, 10);
      WiFi.config(local_IP, gateway, subnet, dns1, dns2);
    }

  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  waitForConnection( "SSID\n" + ssid + " > " + formattedRSSI() );

  return true;
}

bool isConnected() {
  bool connectedToWifi = (WiFi.status() == WL_CONNECTED);

  if (connectedToWifi) {
    digitalWrite(green, HIGH);
  } else {
    digitalWrite(green, LOW);
  }

  return (WiFi.status() == WL_CONNECTED);
}

bool waitForConnection( String ui ) {

  int secondsLeft = waitForWifiTimeout;

  while ( !isConnected() ) {

    if (secondsLeft == 0) resetFunc();

    secondsLeft -= 1;

    digitalWrite(red, HIGH);

    delay(500);

    digitalWrite(red, LOW);

    delay(500);

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
