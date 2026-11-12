void serialListener () {
  while (Serial.available()) {
    delay(10);
    char character = Serial.read();
    if (String(character, DEC) == "10") { //submit
      serialCommand();
      serialString = "";
      serialKey = "";
      serialValue = "";
      hasEquals = false;
      break;
    } else {
      if (String(character, DEC) == "61") { // equals
        hasEquals = true;
        serialKey = serialString;
      } else {
        if (hasEquals) serialValue += String(character);
      }
      serialString += String(character);
    }
  }
}

void serialCommand() {
  serialString.trim();
  serialKey.trim();
  serialValue.trim();
  if ( serialString.length() > 0 ) {

    if (serialString == "LIVE_MODE") {
      if (!LIVE_MODE) {
        connectToWifi = true;
        ensureWifiConnection();
        setupClock();
        setupIna();
        syncClock();
        postHeader();
      }
      LIVE_MODE = !LIVE_MODE;
    }
    if (serialString == "connectToWifi") connectToWifi = !connectToWifi;
    if (serialString == "useAlternateNetwork") useAlternateNetwork = !useAlternateNetwork;
    if (serialString == "useFixedIpAddress") useFixedIpAddress = !useFixedIpAddress;

    if (serialKey == "postAccessKey") postAccessKey = serialValue;
    if (serialKey == "batteryLabelId") batteryLabelId = serialValue;
    if (serialKey == "batteryLabelVoltage") batteryLabelVoltage = serialValue;
    if (serialKey == "batteryLabelType") batteryLabelType = serialValue;
    if (serialKey == "readingDelay") readingDelay = serialValue.toInt();

    if (serialString == "?") displayHelp();
    if (serialString == "displaySettings") displaySettings();
    if (serialString == "connectedStatus") connectedStatus();
    if (serialString == "clockSyncStatus") clockSyncStatus();
    if (serialString == "formattedHttpResponse") formattedHttpResponse();
    if (serialString == "readStatus") readStatus();
    if (serialString == "fileStatus") fileStatus();

    if (serialString == "takeReading") takeReading();
    if (serialString == "eraseFile") eraseFile();
    if (serialString == "syncClock") syncClock();
    if (serialString == "forceClockSync") {
      forceClockSync = true;
      syncClock();
    }

  }
}

void displayHelp () {
  if (Serial) {
    Serial.println("\n>> switches <<");
    Serial.println("LIVE_MODE");
    Serial.println("connectToWifi");
    Serial.println("useAlternateNetwork");
    Serial.println("useFixedIpAddress");
    Serial.println("\n>> key/values <<");
    Serial.println("postAccessKey");
    Serial.println("batteryLabelId");
    Serial.println("batteryLabelVoltage");
    Serial.println("batteryLabelType");
    Serial.println("readingDelay");
    Serial.println("\n>> status <<");
    Serial.println("displaySettings");
    Serial.println("connectedStatus");
    Serial.println("clockSyncStatus");
    Serial.println("formattedHttpResponse");
    Serial.println("readStatus");
    Serial.println("fileStatus");
    Serial.println("\n>> actions <<");
    Serial.println("takeReading");
    Serial.println("eraseFile");
    Serial.println("syncClock");
    Serial.println("forceClockSync");

  }
}

void displaySettings () {
  if (Serial) {
    Serial.println("\n>>> settings <<<");
    Serial.println("LIVE_MODE: " + String(LIVE_MODE) );
    Serial.println("connectToWifi: " + String(connectToWifi) );
    Serial.println("useAlternateNetwork: " + String(useAlternateNetwork) );
    Serial.println("useFixedIpAddress: " + String(useFixedIpAddress) );
    Serial.println("forceClockSync: " + String(forceClockSync) );
    Serial.println("postAccessKey: " + postAccessKey );
    Serial.println("batteryLabelId: " + batteryLabelId );
    Serial.println("batteryLabelVoltage: " + batteryLabelVoltage );
    Serial.println("batteryLabelType: " + batteryLabelType );
    Serial.println("readingDelay: " + readingDelay );
  }
}
