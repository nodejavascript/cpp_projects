# Cat laser toy — servo-driven laser pointer

**The why.** A cat will chase a laser for hours; a human holding one will not.

Two servos sweep a laser in a pattern that never repeats: each move picks a random target
for both axes and a random speed, glides there, then waits a random time. Shorter blinks and
dark pauses fire on a one-in-N chance to keep the cat guessing.

## Files

- `arduino-cat-laser-servo-toy.ino`

## Hardware

Arduino Nano · 2 × hobby servo · laser module

## Libraries

| Library | Version verified |
|---|---|
| `Servo (built in)` | 1.2.2 |

## Build and flash

```bash
arduino-cli compile -b arduino:avr:nano  .
arduino-cli upload  -b arduino:avr:nano  -p /dev/ttyUSB0 .
```

Board: **Arduino Nano (ATmega328P) / Arduino Uno**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
