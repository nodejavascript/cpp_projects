bool setupBluetooth () {
  SerialBT.begin("Power Monitor");
}

void BTListener () {
  while (SerialBT.available()) {
    delay(10);
    char character = SerialBT.read();
    if (String(character, DEC) == "13") {
      BTCommand();
      BTString = "";
      break;
    } else {
      BTString += String(character);
    }
  }
}

void BTCommand() {

  BTString.trim();

  if (BTString == "status") sendBTStatus();

  if (BTString == "toggleWifi") {
    connectToWifi = !connectToWifi;
    ensureWifiConnection();
    SerialBT.println(BTString + ": " + String(connectToWifi));
  }

  if (BTString == "toggleHotspot") {
    useHomeNetwork = !useHomeNetwork;
    disconnectFromSSID();
    ensureWifiConnection();
    SerialBT.println(BTString + ": " + String(useHomeNetwork));
  }

  if (BTString == "toggleFixedIp") {
    useFixedIpAddress = !useFixedIpAddress;
    disconnectFromSSID();
    ensureWifiConnection();
    SerialBT.println(BTString + ": " + String(useFixedIpAddress));
  }

  if (BTString == "showVoltages") {
    showVoltages = !showVoltages;
    SerialBT.println(BTString + ": " + String(showVoltages));
  }

  if (BTString == "plotVoltages") {
    plotVoltages = !plotVoltages;
    SerialBT.println(BTString + ": " + String(plotVoltages));
  }

  if (BTString == "pollFaster") {
    if (pollingDelay <= 100) {
      pollingDelay -= 25;
    } else {
      pollingDelay -= 500;
    }
    if (pollingDelay < 0) pollingDelay = 0;
    SerialBT.println(BTString + ": " + String(pollingDelay));
  }

  if (BTString == "pollSlower") {
    pollingDelay += 500;
    SerialBT.println(BTString + ": " + String(pollingDelay));
  }

  if (BTString == "eraseLog") {
    eraseLog = true;
    SerialBT.println(BTString + ": " + String(eraseLog));
  }

  if (BTString == "saveLog") {
    saveLog = !saveLog;
    SerialBT.println(BTString + ": " + String(saveLog));
  }

  if (BTString == "postLog") {
    postLog = !postLog;
    SerialBT.println(BTString + ": " + String(postLog));
  }

  if (BTString == "syncClock") {
    checkToSyncClock();
    SerialBT.println(BTString + ": " + String(syncClock));
  }

}

void sendBTStatus () {
  String ui = "";
  ui += "connectToWifi:" + String(connectToWifi);
  ui += "\n";
  ui += "useHomeNetwork:" + String(useHomeNetwork);
  ui += "\n";
  ui += "useFixedIpAddress:" + String(useFixedIpAddress);
  ui += "\n";
  ui += "showVoltages:" + String(showVoltages);
  ui += "\n";
  ui += "pollingDelay:" + String(pollingDelay);
  ui += "\n";
  ui += "eraseLog:" + String(eraseLog);
  ui += "\n";
  ui += "saveLog:" + String(saveLog);
  ui += "\n";
  ui += "postLog:" + String(postLog);
  ui += "\n";
  ui += "syncClock:" + String(syncClock);
  SerialBT.println(ui);
}
