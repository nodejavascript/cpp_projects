# OLED analog signal visualizer — a pocket oscilloscope

**The why.** A signal you can see is a signal you can reason about; a number on a serial monitor is a guess with decimals.

Samples A0 every 10 ms into a 128-sample circular buffer and draws it as a scrolling bar
graph on a 128×64 SSD1306, with the live value and frame rate in a status bar.

## Files

- `arduino-ssd1306-analog-signal-oscilloscope.ino`

## Hardware

Arduino Uno or Nano · SSD1306 128×64 OLED · any analog signal on A0

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

## Notes

Derived from Jon E. Froehlich's scrolling-graph sketch (makeabilitylab.io). See NOTICE.md.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
