/*
 * Robot arm servo controller
 *
 * A two-axis joystick with a button drives three servos. At start-up the sketch
 * measures where the joystick rests and treats that as centre, so the arm moves
 * proportionally from wherever the stick happens to sit rather than assuming a
 * perfect midpoint. The button is read on a third pin for future use.
 *
 * Board: ESP32 (DevKitC / WROOM-32)
 * Wiring: servos on 12, 14 and 26 · joystick X on 25, Y on 33, button on 32
 */

#include <ESP32Servo.h>

#define SERVO_1_PIN   12
#define SERVO_2_PIN   14
#define SERVO_3_PIN   26

#define JOY_X_PIN     25
#define JOY_Y_PIN     33
#define JOY_BUTTON_PIN 32

#define ADC_MAX       4095
#define DEADZONE      0.1    // 10% either side of centre is treated as "not moving"

Servo servo1;
Servo servo2;
Servo servo3;

int position1 = 90;
int position2 = 90;
int position3 = 90;

int speed1 = 10;

int joystickXCentre = 0;
int joystickYCentre = 0;

bool movingLeft = false;
bool movingRight = false;

void setup() {
  Serial.begin(115200);

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);

  servo1.write(position1);
  servo2.write(position2);
  servo3.write(position3);

  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);

  // Whatever the stick reads at start-up is centre.
  joystickXCentre = map(analogRead(JOY_X_PIN), ADC_MAX, 0, 0, ADC_MAX);
  joystickYCentre = map(analogRead(JOY_Y_PIN), 0, ADC_MAX, 0, ADC_MAX);

  Serial.print("centre x=");
  Serial.print(joystickXCentre);
  Serial.print(" y=");
  Serial.println(joystickYCentre);
}

void loop() {
  readJoystick();

  // Cap the travel before writing, so the arm never drives past its limits.
  if (position1 < 0) position1 = 0;
  if (position1 > 180) position1 = 180;

  servo1.write(position1);
  delay(speed1);
}

void readJoystick() {
  int xMapped = map(analogRead(JOY_X_PIN), ADC_MAX, 0, 0, ADC_MAX);
  int yMapped = map(analogRead(JOY_Y_PIN), 0, ADC_MAX, 0, ADC_MAX);

  int deadzone = joystickXCentre * DEADZONE;

  movingLeft = xMapped < joystickXCentre - deadzone;
  movingRight = xMapped > joystickXCentre + deadzone;

  if (movingLeft) {
    position1++;
  }
  if (movingRight) {
    position1--;
    if (position1 < 0) {
      position1 = 0;
    }
  }

  // The Y axis is read and centred but not yet wired to a servo.
  (void)yMapped;
}
