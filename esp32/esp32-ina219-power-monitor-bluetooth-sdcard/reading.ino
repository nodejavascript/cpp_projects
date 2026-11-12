
void postReading () {
  String payload = getReadingCSV();
  clientPost(postUrl, payload);
}

void takeReading () {
  reading.ina219_usb5vSource_busVoltage = ina219_usb5vSource.getBusVoltage_V();
  reading.ina219_usb5vSource_shuntVoltage = ina219_usb5vSource.getShuntVoltage_mV();
  reading.ina219_usb5vSource_loadVoltage = reading.ina219_usb5vSource_busVoltage + (reading.ina219_usb5vSource_shuntVoltage / 1000);
  reading.ina219_usb5vSource_loadMilliAmps = ina219_usb5vSource.getCurrent_mA();
  reading.ina219_usb5vSource_loadMilliWatts = ina219_usb5vSource.getPower_mW();

  reading.ina219_batteryTerminal_busVoltage = ina219_batteryTerminals.getBusVoltage_V();
  reading.ina219_batteryTerminal_shuntVoltage = ina219_batteryTerminals.getShuntVoltage_mV();
  reading.ina219_batteryTerminal_loadVoltage = reading.ina219_batteryTerminal_busVoltage + (reading.ina219_batteryTerminal_shuntVoltage / 1000);
  reading.ina219_batteryTerminal_loadMilliAmps = ina219_batteryTerminals.getCurrent_mA();
  reading.ina219_batteryTerminal_loadMilliWatts = ina219_batteryTerminals.getPower_mW();
}

void displayReading () {

  bool charging = false;

  if (reading.ina219_usb5vSource_loadMilliAmps <= 1) {
    reading.ina219_usb5vSource_loadMilliAmps = 0;
    reading.ina219_usb5vSource_loadMilliWatts = 0;
  } else {
    charging = true;
  }
//

  String ui = "";
  if (charging) ui += "CHARGING";
  if (!charging) ui += "IDLE";
  ui += "\n";
//  ui += "Level: " + String(chargedPercent) + "%";
//  ui += "\n";
  ui += "S Voltage: " + String(reading.ina219_usb5vSource_loadVoltage) +" V";
  ui += "\n";
  ui += "S Current: " + String(reading.ina219_usb5vSource_loadMilliAmps) +" mA";
  ui += "\n";
  ui += "S Power: " + String(reading.ina219_usb5vSource_loadMilliWatts) +" mW";
  ui += "\n";
  ui += "B Voltage: " + String(reading.ina219_batteryTerminal_loadVoltage) +" V";
  ui += "\n";
  ui += "B Current: " + String(reading.ina219_batteryTerminal_loadMilliAmps) +" mA";
  ui += "\n";
  ui += "B Power: " + String(reading.ina219_batteryTerminal_loadMilliWatts) +" mW";

  printOled( ui );

}

String logHeader () {
  String header = "";
  header += "batteryLabelId";
  header += ",";
  header += "batteryLabelVotage";
  header += ",";
  header += "batteryLabelType";
  header += ",";
  header += "ISOString";
  header += ",";
  header += "ina219_usb5vSource_busVoltage";
  header += ",";
  header += "ina219_usb5vSource_shuntVoltage";
  header += ",";
  header += "ina219_usb5vSource_loadVoltage";
  header += ",";
  header += "ina219_usb5vSource_loadMilliAmps";
  header += ",";
  header += "ina219_usb5vSource_loadMilliWatts";
  header += ",";
  header += "ina219_batteryTerminal_busVoltage";
  header += ",";
  header += "ina219_batteryTerminal_shuntVoltage";
  header += ",";
  header += "ina219_batteryTerminal_loadVoltage";
  header += ",";
  header += "ina219_batteryTerminal_loadMilliAmps";
  header += ",";
  header += "ina219_batteryTerminal_loadMilliWatts";
  return header;
}

String getReadingCSV () {
  String dataToCSV = "";
  dataToCSV += String(reading.batteryLabelId);
  dataToCSV += ",";
  dataToCSV += String(reading.batteryLabelVotage);
  dataToCSV += ",";
  dataToCSV += String(reading.batteryLabelType);
  dataToCSV += ",";
  dataToCSV += getISOString();
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_usb5vSource_busVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_usb5vSource_shuntVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_usb5vSource_loadVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_usb5vSource_loadMilliAmps);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_usb5vSource_loadMilliWatts);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_batteryTerminal_busVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_batteryTerminal_shuntVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_batteryTerminal_loadVoltage);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_batteryTerminal_loadMilliAmps);
  dataToCSV += ",";
  dataToCSV += String(reading.ina219_batteryTerminal_loadMilliWatts);
  return dataToCSV;
}

void saveReading () {
  if ( checkToSyncClock() ) {
    String dataToCSV = getReadingCSV();
    writeToLog(SD, dataToCSV);
  }
}

void plotReading () {
  if (Serial) {
    Serial.print("Battery-Voltage:");
    Serial.println(reading.ina219_batteryTerminal_loadVoltage);
  }
}

//calibrateReading () {
