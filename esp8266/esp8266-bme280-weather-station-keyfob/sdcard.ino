
void initializeSdCard() {

  while ( !SD.begin(chipSelect) ) {
    printOled( "Insert SDCARD" );
    delay(1000);
  }

  if (!SD.exists("LOGS.TXT")) {
    logFile = SD.open("LOGS.TXT", FILE_WRITE);
    logFile.close();
  }

}
