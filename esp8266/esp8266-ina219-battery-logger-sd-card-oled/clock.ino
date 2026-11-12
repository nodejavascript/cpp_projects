bool setupClock () {
  if ( !rtc.begin() ) {
    if (Serial) Serial.println("\nClock failed");
    while(true);
  }
  lostPower = rtc.lostPower();
  return true;
}

bool syncClock () {

  if (clockBeforeSync.length() == 0) clockBeforeSync = getISOString();

  long startMillis = millis();

  if ( !synced ) {

    DateTime now = rtc.now();

    int yearNow = now.year();

    if (!lostPower && !forceClockSync) {
      synced = true;
      statusMessage = "Already synced";
    }

    if (lostPower || forceClockSync) {

      attemptNTP = true;

      if ( isConnected() ) {

        bool wasSynced = syncNTP();

        if (wasSynced) {
          synced = true;
          statusMessage = "Synced from NTP";
        } else {
          synced = false;
          statusMessage = httpError;
        }

      } else {
        synced = false;
        statusMessage = formattedWifiStatus();
      }

    }

  }
  forceClockSync = false;
  clockWaitTime = (long)millis() - startMillis;
  return true;
}

const String clockSyncStatus () {
  String ui = "";
  ui += "\n";
  ui += ">>> clockSyncStatus() <<<";
  ui += "\n";
  ui += "lostPower: " + String(lostPower);
  ui += "\n";
  ui += "clockBeforeSync: " + clockBeforeSync;
  ui += "\n";
  ui += "clockNow: " + getISOString();
  ui += "\n";
  ui += "synced: " + String(synced);
  ui += "\n";
  ui += "attemptNTP: " + String(attemptNTP);
  ui += "\n";
  ui += "statusMessage: " + statusMessage;
  ui += "\n";
  ui += "clockWaitTime: " + String(clockWaitTime) + " ms";
  if (Serial) Serial.println(ui);
  return ui;
}

String getISOString () {

  DateTime now = rtc.now();

  String yearString = String( now.year(), DEC );
  String monthString = String( now.month(), DEC );
  String dayString = String( now.day(), DEC );
  String hourString = String( now.hour(), DEC );
  String minuteString = String( now.minute(), DEC );
  String secondString = String( now.second(), DEC );

  if (monthString.length() == 1) monthString = "0" + monthString;
  if (dayString.length() == 1) dayString = "0" + dayString;
  if (hourString.length() == 1) hourString = "0" + hourString;
  if (minuteString.length() == 1) minuteString = "0" + minuteString;
  if (secondString.length() == 1) secondString = "0" + secondString;

  String ISOString = "";

  ISOString += yearString;
  ISOString += "-";
  ISOString += monthString;
  ISOString += "-";
  ISOString += dayString;
  ISOString += "T";
  ISOString += hourString;
  ISOString += ":";
  ISOString += minuteString;
  ISOString += ":";
  ISOString += secondString;
  ISOString += ".000Z";

  return ISOString;
}

String getDateNow () {

  DateTime now = rtc.now();

  String yearString = String( now.year() );
  String monthString = String( now.month() );
  String dayString = String( now.day() );

  if (monthString.length() == 1) monthString = "0" + monthString;
  if (dayString.length() == 1) dayString = "0" + dayString;

  return yearString + "-" +  monthString + "-" + dayString;
}

String getTimeNow () {

  DateTime now = rtc.now();

  String ampm = "AM";
  int hourNow = now.hour();
  if (hourNow > 12) {
    hourNow -= 12;
    ampm = "PM";
  }

  String hourString = String( hourNow );
  String minuteString = String( now.minute() );
  String secondString = String( now.second() );

  if (minuteString.length() == 1) minuteString = "0" + minuteString;
  if (secondString.length() == 1) secondString = "0" + secondString;

  return hourString + ":" +  minuteString + ":" + secondString + " " + ampm;
}

boolean syncNTP () {
  clientGet("https://worldtimeapi.org/api/ip");

  if ( httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument doc( 600 );
    deserializeJson(doc, httpResponse);

    long unix_time = doc["unixtime"];
    long raw_offset = doc["raw_offset"];
    long local_time = unix_time + raw_offset;

    rtc.adjust(DateTime(year(local_time), month(local_time), day(local_time), hour(local_time), minute(local_time), second(local_time)));

    return true;
  }
  return false;
}
