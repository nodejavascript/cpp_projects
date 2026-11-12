# BME280 temperature and humidity display

**The why.** A room's climate is more than one number, and a display that shows one is a decoration.

Reads a BME280 over I²C and shows temperature in large type with humidity beneath on a
128×32 OLED. The screen blanks after four seconds without a button press, so a bedside unit
is not a lamp.

## Files

- `arduino-nano-bme280-oled-temperature-display.ino`

## Hardware

Arduino Nano (ATmega328P, old bootloader) · BME280 · SSD1306 OLED · button on D4

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit BME280 Library` | 2.3.0 |
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
