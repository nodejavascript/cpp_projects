# ATX bench power supply with INA219 and OLED

**The why.** A bench supply you already own should not need a second mortgage to measure what it is giving you.

Switches on a salvaged ATX supply and reads its output through an INA219 shunt, showing
voltage, current and power on a 128×32 OLED.

## Files

- `arduino-atx-bench-power-supply-ina219-oled.ino`

## Hardware

Arduino Nano or Uno · ATX power supply · INA219 · SSD1306 128×32 OLED

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
