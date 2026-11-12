bool checkToSyncClock () {
  DateTime now = rtc.now();
  int yearNow = now.year();
  int success = false;
  String message;

  if (yearNow < 2020) {
    bool wasSynced = clockSynced();
    if (wasSynced) {
      message = "Clock sync success";
      success = true;
    } else {
      message = "Clock sync failed";
    }
  } else {
    message = "Clock didnt require sync";
    success = true;
  }
  if (Serial) Serial.println( message + ": " + getISOString() );
  return success;
}

String getISOString() {

  // 2011-10-05T14:48:00.000Z
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

bool clockSynced () {

  return true;

//
//
//    DynamicJsonDocument doc( 600 );
//
//
//
//
//

}
