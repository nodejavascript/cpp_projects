/*
 * Cat laser toy
 *
 * Two servos sweep a laser pointer in a pattern that never repeats: each move
 * picks a new random target for both axes and a random speed, glides there, then
 * waits a random time. Two rarer events keep a cat interested — a short blink and
 * a dark pause — both of which fire on a one-in-N chance per loop.
 *
 * Board: Arduino Nano (ATmega328P, old bootloader)
 * Wiring: laser on D3 · horizontal servo D5 · vertical servo D6
 */

#include <Servo.h>

#define LASER_PIN        3
#define SERVO_X_PIN      5
#define SERVO_Y_PIN      6

// Travel: how far either side of centre the laser is allowed to go.
#define ANGLE_FROM_CENTRE 60
#define ANGLE_MIN        (90 - ANGLE_FROM_CENTRE)
#define ANGLE_MAX        (90 + ANGLE_FROM_CENTRE)

// Speed: the delay between single-degree steps, lower is faster.
#define SPEED_MIN        10
#define SPEED_MAX        25

// Waits, in milliseconds, between moves.
#define WAIT_MIN         0
#define WAIT_MAX         3000

// Odds of the two interruptions, as "one in N" per loop.
#define BLINK_ONE_IN     300
#define DARK_ONE_IN      200
#define DARK_WAIT_MAX    3000

Servo servoX;
Servo servoY;

int angleX = 90;
int targetX = 90;
bool xArrived = true;

int angleY = 90;
int targetY = 90;
bool yArrived = true;

int stepDelay = SPEED_MIN;

void setup() {
  Serial.begin(115200);

  pinMode(LASER_PIN, OUTPUT);

  servoX.attach(SERVO_X_PIN);
  servoY.attach(SERVO_Y_PIN);
  delay(250);

  servoX.write(angleX);
  servoY.write(angleY);

  // Three quick flashes so you can see it is alive before it starts hunting.
  digitalWrite(LASER_PIN, HIGH);
  delay(500);
  digitalWrite(LASER_PIN, LOW);
  delay(500);
  digitalWrite(LASER_PIN, HIGH);
  delay(500);
  digitalWrite(LASER_PIN, LOW);
  delay(500);
  digitalWrite(LASER_PIN, HIGH);
}

void loop() {
  // Pick a new destination only once the previous one is reached, so the
  // movement is a glide rather than a jump.
  if (xArrived && yArrived) {
    targetX = random(ANGLE_MIN, ANGLE_MAX);
    targetY = random(ANGLE_MIN, ANGLE_MAX);
    stepDelay = random(SPEED_MIN, SPEED_MAX);

    xArrived = false;
    yArrived = false;
  }

  if (angleX < targetX) angleX++;
  if (angleX > targetX) angleX--;
  if (angleY < targetY) angleY++;
  if (angleY > targetY) angleY--;

  if (angleX == targetX) xArrived = true;
  if (angleY == targetY) yArrived = true;

  if (random(1, BLINK_ONE_IN) == 42) {
    blinkLaser();
  }

  if (random(1, DARK_ONE_IN) == 42) {
    digitalWrite(LASER_PIN, LOW);
    delay(random(1, DARK_WAIT_MAX));
    digitalWrite(LASER_PIN, HIGH);
  }

  // A pause at each corner is what stops the pattern feeling mechanical.
  if (xArrived && yArrived) {
    int wait = random(WAIT_MIN, WAIT_MAX);
    wait = (wait < (WAIT_MAX - WAIT_MIN) / 2) ? wait / 2 : wait * 2;
    delay(wait);
  }

  if (angleX != targetX) servoX.write(angleX);
  if (angleY != targetY) servoY.write(angleY);

  delay(stepDelay);
}

void blinkLaser() {
  digitalWrite(LASER_PIN, LOW);
  delay(500);
  digitalWrite(LASER_PIN, HIGH);
  delay(500);
  digitalWrite(LASER_PIN, LOW);
  delay(500);
  digitalWrite(LASER_PIN, HIGH);
}
