
void postHeader () {
  String payload = logHeader();
  clientPost(postURL, postAccessKey, payload);
}

void postReading () {
  String payload = getReadingCSV();
  clientPost(postURL, postAccessKey, payload);
}

bool takeReading () {
  ISOString = getISOString();

  ina219_usb5vSource_busVoltage = ina219_usb5vSource.getBusVoltage_V();
  ina219_usb5vSource_shuntVoltage = ina219_usb5vSource.getShuntVoltage_mV();
  ina219_usb5vSource_loadVoltage = ina219_usb5vSource_busVoltage + (ina219_usb5vSource_shuntVoltage / 1000);
  ina219_usb5vSource_loadMilliAmps = ina219_usb5vSource.getCurrent_mA();
  ina219_usb5vSource_loadMilliWatts = ina219_usb5vSource.getPower_mW();

  ina219_batteryTerminal_busVoltage = ina219_batteryTerminals.getBusVoltage_V();
  ina219_batteryTerminal_shuntVoltage = ina219_batteryTerminals.getShuntVoltage_mV();
  ina219_batteryTerminal_loadVoltage = ina219_batteryTerminal_busVoltage + (ina219_batteryTerminal_shuntVoltage / 1000);
  ina219_batteryTerminal_loadMilliAmps = ina219_batteryTerminals.getCurrent_mA();
  ina219_batteryTerminal_loadMilliWatts = ina219_batteryTerminals.getPower_mW();
  return true;
}

const String readStatus () {
  String ui = "";
  ui += "\n";
  ui += ">>> readStatus() <<<";
  ui += "\n";
  ui += "batteryLabelId: " + batteryLabelId;
  ui += "\n";
  ui += "batteryLabelVoltage: " + batteryLabelVoltage;
  ui += "\n";
  ui += "batteryLabelType: " + batteryLabelType;
  ui += "\n";
  ui += "ISOString: " + ISOString;
  ui += "\n";
  ui += "ina219_usb5vSource_busVoltage: " + String(ina219_usb5vSource_busVoltage) + " V";
  ui += "\n";
  ui += "ina219_usb5vSource_shuntVoltage: " + String(ina219_usb5vSource_shuntVoltage) + " V";
  ui += "\n";
  ui += "ina219_usb5vSource_loadVoltage: " + String(ina219_usb5vSource_loadVoltage) + " V";
  ui += "\n";
  ui += "ina219_usb5vSource_loadMilliAmps: " + String(ina219_usb5vSource_loadMilliAmps) + " mA";
  ui += "\n";
  ui += "ina219_usb5vSource_loadMilliWatts: " + String(ina219_usb5vSource_loadMilliWatts) + " mW";
  ui += "\n";
  ui += "ina219_batteryTerminal_busVoltage: " + String(ina219_batteryTerminal_busVoltage) + " V";
  ui += "\n";
  ui += "ina219_batteryTerminal_shuntVoltage: " + String(ina219_batteryTerminal_shuntVoltage) + " V";
  ui += "\n";
  ui += "ina219_batteryTerminal_loadVoltage: " + String(ina219_batteryTerminal_loadVoltage) + " V";
  ui += "\n";
  ui += "ina219_batteryTerminal_loadMilliAmps: " + String(ina219_batteryTerminal_loadMilliAmps) + " mA";
  ui += "\n";
  ui += "ina219_batteryTerminal_loadMilliWatts: " + String(ina219_batteryTerminal_loadMilliWatts) + " mW";
  if (Serial) Serial.println(ui);
  return ui;
}

String displayReading () {
  String ui = "";
  ui += "\n";
  ui += "S Voltage: " + String(ina219_usb5vSource_loadVoltage) +" V";
  ui += "\n";
  ui += "S Current: " + String(ina219_usb5vSource_loadMilliAmps) +" mA";
  ui += "\n";
  ui += "S Power: " + String(ina219_usb5vSource_loadMilliWatts) +" mW";
  ui += "\n";
  ui += "B Voltage: " + String(ina219_batteryTerminal_loadVoltage) +" V";
  ui += "\n";
  ui += "B Current: " + String(ina219_batteryTerminal_loadMilliAmps) +" mA";
  ui += "\n";
  ui += "B Power: " + String(ina219_batteryTerminal_loadMilliWatts) +" mW";
  return ui;
}

String logHeader () {
  String header = "";
  header += "batteryLabelId";
  header += ",";
  header += "batteryLabelVoltage";
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
  dataToCSV += String(batteryLabelId);
  dataToCSV += ",";
  dataToCSV += String(batteryLabelVoltage);
  dataToCSV += ",";
  dataToCSV += String(batteryLabelType);
  dataToCSV += ",";
  dataToCSV += getISOString();
  dataToCSV += ",";
  dataToCSV += String(ina219_usb5vSource_busVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_usb5vSource_shuntVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_usb5vSource_loadVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_usb5vSource_loadMilliAmps);
  dataToCSV += ",";
  dataToCSV += String(ina219_usb5vSource_loadMilliWatts);
  dataToCSV += ",";
  dataToCSV += String(ina219_batteryTerminal_busVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_batteryTerminal_shuntVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_batteryTerminal_loadVoltage);
  dataToCSV += ",";
  dataToCSV += String(ina219_batteryTerminal_loadMilliAmps);
  dataToCSV += ",";
  dataToCSV += String(ina219_batteryTerminal_loadMilliWatts);
  return dataToCSV;
}

//void saveReading () {

void plotReading () {
  if (Serial) {
    Serial.print("Source-Voltage:");
    Serial.print(ina219_usb5vSource_loadVoltage);
    Serial.print(",");
    Serial.print("Source-Current:");
    Serial.print(ina219_usb5vSource_loadMilliAmps / 300 * 8);
    Serial.print(",");
    Serial.print("Battery-Voltage:");
    Serial.print(ina219_batteryTerminal_loadVoltage);

    Serial.println("");
  }
}

//calibrateReading () {
