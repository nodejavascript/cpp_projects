
void setupOLED() {
  if ( !display.begin(SSD1306_SWITCHCAPVCC, 0x3C) ) {
    if (Serial) Serial.println("OLED failed");
    while (true);
  }
  display.clearDisplay();
  display.display();
}

void printOled (String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(text);
  display.display();
}
