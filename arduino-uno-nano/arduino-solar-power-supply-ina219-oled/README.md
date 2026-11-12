# Solar power supply monitor with INA219 and OLED

**The why.** A solar panel's output is only honest when it is measured.

Reads a panel through an INA219 and shows panel voltage in large type with the current being
drawn beneath, so a passing cloud is visible immediately.

## Files

- `arduino-solar-power-supply-ina219-oled.ino`

## Hardware

Arduino Nano or Uno · solar panel · INA219 · SSD1306 128×32 OLED

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit INA219` | 1.2.3 |
| `Adafruit SSD1306` | 2.5.15 |

## Build and flash

```bash
arduino-cli compile -b arduino:avr:nano  .
arduino-cli upload  -b arduino:avr:nano  -p /dev/ttyUSB0 .
```

Board: **Arduino Nano (ATmega328P) / Arduino Uno**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
