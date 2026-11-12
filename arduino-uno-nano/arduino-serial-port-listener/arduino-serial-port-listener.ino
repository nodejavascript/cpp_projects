/*
 * Serial port listener — key=value command parser
 *
 * Reads a line from the serial port, splits it at the first "=", and prints the
 * key and the value it found. Useful for testing a protocol before any device
 * is wired up: type "brightness=250" and watch it come back parsed.
 *
 * Board: Arduino Nano or Uno, or any board with a serial line to listen to
 */

String incoming = "";
String commandKey = "";
String commandValue = "";

void setup() {
  Serial.begin(115200);
  Serial.println("\nSerial listener ready — send key=value and press Enter");
}

void loop() {
  readSerial();
}

// Accumulate characters until a newline arrives, then hand the line over.
void readSerial() {
  while (Serial.available()) {
    char character = Serial.read();

    if (character == '\n') {
      handleLine();
      incoming = "";
      commandKey = "";
      commandValue = "";
      return;
    }

    if (character == '=') {
      commandKey = incoming;
    } else if (commandKey.length() > 0) {
      commandValue += character;
    }

    incoming += character;
  }
}

void handleLine() {
  incoming.trim();
  commandKey.trim();
  commandValue.trim();

  if (incoming.length() == 0) {
    return;
  }

  Serial.print("line  = ");
  Serial.println(incoming);
  Serial.print("key   = ");
  Serial.println(commandKey);
  Serial.print("value = ");
  Serial.println(commandValue);
}
