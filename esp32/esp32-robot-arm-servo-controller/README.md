# Robot arm servo controller

**The why.** A joystick is a better robot-arm interface than a list of angles.

A two-axis joystick with a button drives three servos. At start-up it measures where the
stick rests and treats that as centre, so the arm moves proportionally from wherever the
stick happens to sit rather than assuming a perfect midpoint.

## Files

- `esp32-robot-arm-servo-controller.ino`

## Hardware

ESP32 (DevKitC / WROOM-32) · 3 × servo (12, 14, 26) · joystick on 25 / 33 / 32

## Libraries

| Library | Version verified |
|---|---|
| `ESP32Servo` | 3.0.8 |

## Build and flash

```bash
arduino-cli compile -b esp32:esp32:esp32:PartitionScheme=huge_app --libraries ~/.arduino-libs/esp32 .
arduino-cli upload  -b esp32:esp32:esp32:PartitionScheme=huge_app --libraries ~/.arduino-libs/esp32 -p /dev/ttyUSB0 .
```

Board: **ESP32 — DOIT ESP32 DevKit V1 / ESP32-WROOM-32**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
