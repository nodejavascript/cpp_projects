bool ensureSDCardInserted () {
  bool initializedError = false;
  while ( !SD.begin(4) ) {
    delay(100);
    if ( !initializedError ) {
      initializedError = true;
      if (Serial) Serial.println("\nINSERT SDCARD");
      while (true);
    }
  }
  return true;
}

bool ensureFileCreated () {
  if ( !SD.exists( filename ) ) {
    File file = SD.open(filename, FILE_WRITE);
    file.println( logHeader() );
    file.close();
  }
  return true;
}

bool eraseFile () {
  if ( SD.exists( filename ) ) {
    SD.remove(filename);
    ensureFileCreated();
  }
  return true;
}

bool logData ( String payload ) {
  File file = SD.open(filename, FILE_WRITE);
  file.println(payload);
  file.close();
  refreshFileStatus();
  return true;
}

bool refreshFileStatus () {

  ensureSDCardInserted();

  long startMillis = millis();

  File file = SD.open(filename);

  int fileSize = file.size();

  fileSize = fileSize;
  records = 0;

  if (file && fileSize > 0) {

    while( file.available() ) {
      if (file.read() == '\n') records += 1;
    }
  }

  fileWaitTime = (long)millis() - startMillis;

  file.close();

  return true;
}

const String fileStatus () {
  ensureSDCardInserted();
  refreshFileStatus();
  String ui = "";
  ui += "\n";
  ui += ">>> fileStatus() <<<";
  ui += "\n";
  ui += "filename: " + filename;
  ui += "\n";
  ui += "fileSize: " + String(fileSize) + " b";
  ui += "\n";
  ui += "records: " + String(records);
  ui += "\n";
  ui += "fileWaitTime: " + String(fileWaitTime) + " ms";
  if (Serial) Serial.println(ui);
  return ui;
}
