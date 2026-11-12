/*
 * OLED analog signal visualizer — a pocket oscilloscope
 *
 * Samples one analog pin every 10 ms into a circular buffer and draws it as a
 * scrolling bar graph on a 128x64 SSD1306, with the live value and the frame rate
 * in a status bar. A signal you can see is a signal you can reason about.
 *
 * Board: Arduino Uno or Nano
 * Wiring: any analog signal on A0, OLED on I2C
 *
 * Derived from the scrolling-graph sketch by Jon E. Froehlich
 * (@jonfroehlich, makeabilitylab.io) — see NOTICE.md at the repository root.
 */

#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display_(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ANALOG_INPUT_PIN = A0;
const int MIN_ANALOG_INPUT = 0;
const int MAX_ANALOG_INPUT = 1023;
const int DELAY_LOOP_MS = 10; // change to slow down how often to read and graph value

int circularBuffer[SCREEN_WIDTH]; //fast way to store values
int writeIndex = 0; // tracks where we are in the circular buffer

// for tracking fps
float framesPerSecond = 0;
unsigned long frameCount = 0;
unsigned long framesPerSecondStartTimeStamp = 0;

// status bar
boolean showStatusBar = true; // change to show/hide status bar
int graphHeight = SCREEN_HEIGHT;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display_.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display_.clearDisplay();

  display_.setTextSize(1);
  display_.setTextColor(WHITE, BLACK);
  display_.setCursor(0, 0);
  display_.println("Screen initialized!");
  display_.display();
  delay(500);
  display_.clearDisplay();

  if(showStatusBar){
    graphHeight = SCREEN_HEIGHT - 10;
  }

  framesPerSecondStartTimeStamp = millis();
}

void loop() {
  // Clear the display on each frame. We draw from the circularBuffer
  display_.clearDisplay();

  // Read and store the analog data into a circular buffer
  int analogVal = analogRead(ANALOG_INPUT_PIN);
  Serial.println(analogVal);
  circularBuffer[writeIndex++] = analogVal;

  // Set the circular buffer index back to zero when it reaches the
  // right of the screen
  if(writeIndex >= display_.width()){
    writeIndex = 0;
  }

  if(showStatusBar){
    drawStatusBar(analogVal);
  }

  // Draw the line graph based on data in circularBuffer
  int xPos = 0;
  for (int i = writeIndex; i < display_.width(); i++){
    int analogVal = circularBuffer[i];
    drawLine(xPos, analogVal);
    xPos++;
  }

  for(int i = 0; i < writeIndex; i++){
    int analogVal = circularBuffer[i];
    drawLine(xPos, analogVal);
    xPos++;;
  }

  display_.display();

  calcFrameRate();

  delay(DELAY_LOOP_MS);
}

/**
 * Draw the line at the given x position and analog value
 */
void drawLine(int xPos, int analogVal){
  int lineHeight = map(analogVal, MIN_ANALOG_INPUT, MAX_ANALOG_INPUT, 0, graphHeight);
  int yPos = display_.height() - lineHeight;
  display_.drawFastVLine(xPos, yPos, lineHeight, SSD1306_WHITE);
}

/**
 * Call this every frame to calculate frame rate
 */
void calcFrameRate() {

  unsigned long elapsedTime = millis() - framesPerSecondStartTimeStamp;
  frameCount++;
  if (elapsedTime > 1000) {
    framesPerSecond = frameCount / (elapsedTime / 1000.0);
    framesPerSecondStartTimeStamp = millis();
    frameCount = 0;
  }
}

/**
 * Draws the status bar at top of screen with fps and analog value
 */
void drawStatusBar(int analogVal) {

   // erase status bar by drawing all black
  display_.fillRect(0, 0, display_.width(), 8, SSD1306_BLACK);

  // Draw current val
  display_.setCursor(0, 0);
  display_.print(analogVal);

  // Draw frame count
  int16_t x1, y1;
  uint16_t w, h;
  display_.getTextBounds("XX.XX fps", 0, 0, &x1, &y1, &w, &h);
  display_.setCursor(display_.width() - w, 0);
  display_.print(framesPerSecond);
  display_.print(" fps");
}
