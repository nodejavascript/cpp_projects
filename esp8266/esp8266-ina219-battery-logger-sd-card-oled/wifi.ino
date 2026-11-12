bool isConnected() { return (WiFi.status() == WL_CONNECTED ); }

bool ensureWifiConnection () {
  long startMillis = millis();
  if ( connectToWifi && !isConnected() ) setIpAndConnect();
  if ( !connectToWifi && isConnected() ) disconnectWifi();
  connectionWaitTime = (long)millis() - startMillis;
  return true;
}

bool setIpAndConnect () {

  if ( isConnected() ) return true;

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

  waitForConnection(ssid, password);

  return true;
}

bool disconnectWifi () {
  if (Serial) Serial.println("\ndisconnectWifi");
  if ( isConnected() ) WiFi.disconnect();
  while ( isConnected() ) delay(10);
  return true;
}

bool beginWifi (String ssid, String password) {
  if (Serial) Serial.println("\nbeginWifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  return true;
}

bool waitForConnection (String ssid, String password) {

  beginWifi(ssid, password);

  int waitSeconds = 10;

  if (Serial) Serial.print("\nwaitForConnection (" + String(waitSeconds) +"s)");

  int secondsLeft = waitSeconds;

  while ( !isConnected() ) {

    if (Serial) Serial.print(".");

    if (secondsLeft == 0) {

      disconnectWifi();
      beginWifi(ssid, password);
      secondsLeft = waitSeconds;

    }
    secondsLeft -= 1;
    delay(1000);
  }

  if (Serial) Serial.println("");
  return true;
}

const String connectedStatus () {
  String ui = "";
  ui += "\n";
  ui += ">>> connectedStatus() <<<";
  ui += "\n";
  ui += "connectToWifi: " + String (connectToWifi);
  ui += "\n";
  ui += "useAlternateNetwork: " + String (useAlternateNetwork);
  ui += "\n";
  ui += "useFixedIpAddress: " + String (useFixedIpAddress);
  ui += "\n";
  ui += "isConnected: " + String (isConnected());
  ui += "\n";
  ui += "formattedWifiStatus: " + formattedWifiStatus();
  ui += "\n";
  ui += "formattedRSSI: " + formattedRSSI();
  ui += "\n";
  ui += "formattedRSSIStength: " + formattedRSSIStength();
  ui += "\n";
  ui += "formattedMacAddress: " + formattedMacAddress();
  ui += "\n";
  ui += "formattedIP: " + formattedIP();
  ui += "\n";
  ui += "formattedConnectionWaitTime: " + formattedConnectionWaitTime();
  if (Serial) Serial.println(ui);
  return ui;
}

const String formattedConnectionWaitTime () {
  return String(connectionWaitTime) + " ms";
}

const String formattedWifiStatus () {
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

const String formattedMacAddress () {
  byte mac[6];
  WiFi.macAddress(mac);
  String macAddress = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);
  macAddress.toUpperCase();
  return macAddress;
}

const String formattedIP () {
  return String(WiFi.localIP()[0]) + String(".") +\
  String(WiFi.localIP()[1]) + String(".") +\
  String(WiFi.localIP()[2]) + String(".") +\
  String(WiFi.localIP()[3]) ;
}

const String formattedRSSI () {
  return String( WiFi.RSSI() ) +" dBm";
}

const String formattedRSSIStength () {
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
