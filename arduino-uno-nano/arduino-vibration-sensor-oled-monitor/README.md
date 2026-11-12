# Vibration sensor monitor

**The why.** A vibration is a shape, not a number, and a threshold alone throws the shape away.

Measures how long an SW-420 switch stays closed each time it triggers, and keeps a running
count, average, minimum and maximum on a 128×32 OLED. Hold the button to reset.

## Files

- `arduino-vibration-sensor-oled-monitor.ino`

## Hardware

Arduino Nano or Uno · SW-420 vibration switch · SSD1306 OLED

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit SSD1306` | 2.5.15 |

## Build and flash

```bash
arduino-cli compile -b arduino:avr:nano  .
arduino-cli upload  -b arduino:avr:nano  -p /dev/ttyUSB0 .
```

Board: **Arduino Nano (ATmega328P) / Arduino Uno**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
