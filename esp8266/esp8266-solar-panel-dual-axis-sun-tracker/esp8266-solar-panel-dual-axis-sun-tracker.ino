/*
 * Dual-axis solar panel sun tracker
 *
 * Two servos aim a small panel, one on each axis. At every step the sketch
 * nudges an axis, measures the panel's output through an INA219, and keeps the
 * new angle only if the output did not drop — a hill climb on the sun. When an
 * axis starts losing power it reverses direction.
 *
 * Board: ESP8266 (NodeMCU / D1 mini) — the servo pins are D7 and D8
 * Wiring: horizontal servo D8, vertical servo D7, INA219 on I2C
 */

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_INA219.h>

#define SERVO_PIN_HORIZONTAL  D8
#define SERVO_PIN_VERTICAL    D7
#define ANGLE_STEP            5
#define ANGLE_MIN             0
#define ANGLE_MAX             180
#define ANGLE_CENTRE          90

Adafruit_INA219 ina219;
Servo servoHorizontal;
Servo servoVertical;

int angleHorizontal = ANGLE_CENTRE;
int angleVertical = ANGLE_CENTRE;

bool horizontalIncreasing = true;
bool verticalIncreasing = true;

void setup() {
  Serial.begin(9600);

  if (!ina219.begin()) {
    Serial.println("INA219 failed");
    while (true) {
      delay(1000);
    }
  }

  servoHorizontal.attach(SERVO_PIN_HORIZONTAL);
  servoVertical.attach(SERVO_PIN_VERTICAL);
  delay(100);

  servoHorizontal.write(angleHorizontal);
  servoVertical.write(angleVertical);
}

void loop() {
  moveHorizontal();
  moveVertical();

  Serial.print("horizontal ");
  Serial.print(angleHorizontal);
  Serial.print("  vertical ");
  Serial.print(angleVertical);
  Serial.print("  panel ");
  Serial.print(panelVoltage());
  Serial.println(" V x10");
}

// Nudge one servo and keep the move only if the panel did not lose power.
void moveHorizontal() {
  int previousAngle = angleHorizontal;
  int previousPower = panelVoltage();

  angleHorizontal += horizontalIncreasing ? ANGLE_STEP : -ANGLE_STEP;
  clampAngles();
  servoHorizontal.write(angleHorizontal);

  if (panelVoltage() < previousPower) {
    angleHorizontal = previousAngle;
    servoHorizontal.write(angleHorizontal);
    horizontalIncreasing = !horizontalIncreasing;
  }
}

void moveVertical() {
  int previousAngle = angleVertical;
  int previousPower = panelVoltage();

  angleVertical += verticalIncreasing ? ANGLE_STEP : -ANGLE_STEP;
  clampAngles();
  servoVertical.write(angleVertical);

  if (panelVoltage() < previousPower) {
    angleVertical = previousAngle;
    servoVertical.write(angleVertical);
    verticalIncreasing = !verticalIncreasing;
  }
}

// The panel takes a moment to settle after a servo moves, so read after a pause.
int panelVoltage() {
  delay(100);
  return round(ina219.getBusVoltage_V() * 10);
}

void clampAngles() {
  if (angleHorizontal > ANGLE_MAX || angleHorizontal < ANGLE_MIN) {
    angleHorizontal = ANGLE_CENTRE;
  }
  if (angleVertical > ANGLE_MAX || angleVertical < ANGLE_MIN) {
    angleVertical = ANGLE_CENTRE;
  }
}
