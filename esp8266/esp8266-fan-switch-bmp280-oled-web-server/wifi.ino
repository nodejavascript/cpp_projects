bool isConnected() { return (WiFi.status() == WL_CONNECTED ); }

bool ensureWifiConnection () {
  if ( connectToWifi && !isConnected() ) setIpAndConnect();
  if ( !connectToWifi && isConnected() ) disconnectWifi();
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
