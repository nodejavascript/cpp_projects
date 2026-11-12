
bool setupIna () {
  if ( !ina219_batteryTerminals.begin() ) {
    if (Serial) Serial.println("\nina219_batteryTerminals failed");
    while(true);
  }
  if ( !ina219_usb5vSource.begin() ) {
    if (Serial) Serial.println("\nina219_usb5vSource failed");
    while(true);
  }
  delay(100);
  return true;
}
