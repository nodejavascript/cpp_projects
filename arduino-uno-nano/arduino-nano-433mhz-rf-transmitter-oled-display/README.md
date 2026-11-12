# 433 MHz RF transmitter with OLED readout

**The why.** A remote you can see what it is sending is a remote you can debug.

Reads a photocell on A7, converts it to a brightness percentage, and sends that plus a
packet counter over a 433 MHz ASK module. The OLED shows the value as it leaves.

## Files

- `arduino-nano-433mhz-rf-transmitter-oled-display.ino`

## Hardware

Arduino Nano (ATmega328P, old bootloader) · FS1000A transmitter · SSD1306 OLED · photocell

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit SSD1306` | 2.5.15 |
| `RadioHead` | 1.143.1 |

## Build and flash

```bash
arduino-cli compile -b arduino:avr:nano  .
arduino-cli upload  -b arduino:avr:nano  -p /dev/ttyUSB0 .
```

Board: **Arduino Nano (ATmega328P) / Arduino Uno**.

## Notes

Uses analog pin A7, which exists on the Nano but not the Uno.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
