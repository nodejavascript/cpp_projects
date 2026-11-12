
String getISOString() {
  // 2011-10-05T14:48:00.000Z
  DateTime now = rtc.now();

  String ISOString = "";

  ISOString += String(now.year(), DEC);
  ISOString += "-";
  ISOString += String(now.month(), DEC);
  ISOString += "-";
  ISOString += String(now.day(), DEC);
  ISOString += "T";
  ISOString += String(now.hour(), DEC);
  ISOString += ":";
  ISOString += String(now.minute(), DEC);
  ISOString += ":";
  ISOString += String(now.second(), DEC);
  ISOString += ".000Z";

  return ISOString;
}

boolean syncRealTimeClock () {

  String worldTimeReponse = clientGet("https://worldtimeapi.org/api/ip");

  DynamicJsonDocument doc(600);
  deserializeJson(doc, worldTimeReponse);

  if (worldTimeReponse.length() > 0) {

    long unix_time = doc["unixtime"];
    long raw_offset = doc["raw_offset"];
    long local_time = unix_time + raw_offset;

    rtc.adjust(DateTime(year(local_time), month(local_time), day(local_time), hour(local_time), minute(local_time), second(local_time)));

    return true;

  }

  return false;

}
