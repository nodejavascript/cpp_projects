
void refreshlogStats (fs::FS &fs) {

  logStats.totalMb = SD.totalBytes() / (1024 * 1024);
  logStats.usedMb = SD.usedBytes() / (1024 * 1024);

  File file = fs.open(logFile);

  int startMillis = millis();

  int fileSize = file.size();

  logStats.fileSizeB = fileSize;
  logStats.fileSizeKB = (double)fileSize / 1024;
  logStats.lines = 0;
  logStats.lineLength = 0;
  logStats.processingBytesPerMilli = 0;
  logStats.processingLinesPerMilli = 0;

  if (file && fileSize > 0) {

    while( file.available() ) {

      if (file.read() == '\n') {

        logStats.lines += 1;

        if ( logStats.lineLength == 0 ) logStats.lineLength = file.position();

      }
    }
  }

  logStats.processingMillis = (int)millis() - startMillis;

  logStats.processingBytesPerMilli = (double)logStats.fileSizeB / (double)logStats.processingMillis;
  logStats.processingLinesPerMilli = (double)logStats.lines / (double)logStats.processingMillis;

  file.close();

}

bool setupSD () {
  if(!SD.begin()){
    printOled( "SD mount failed");
    while (true);
  }
  return true;
}

bool initLogFile() {
  if ( !SD.exists( logFile ) ) {
    createLogFile(SD);
  }
  return true;
}

void createLogFile (fs::FS &fs) {
  File file = fs.open(logFile, FILE_WRITE);
  file.println( logHeader() );
  file.close();
  refreshlogStats(SD);
}

void writeToLog (fs::FS &fs, String record) {
  initLogFile();
  File file = fs.open(logFile, FILE_APPEND);
  file.println(record);
  file.close();
  refreshlogStats(SD);
}

bool eraseLogFile (fs::FS &fs) {
  if ( SD.exists( logFile ) ) {
    fs.remove(logFile);
  }
  initLogFile();
  return true;
}
