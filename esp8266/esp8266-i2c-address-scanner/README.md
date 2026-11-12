# I²C address scanner

**The why.** You cannot wire a sensor you cannot see.

Walks every address on the I²C bus and prints the ones that answer, so an unknown breakout
tells you its address before you write a driver for it.

## Files

- `esp8266-i2c-address-scanner.ino`

## Hardware

Any ESP8266 or AVR board · one I²C device attached

## Libraries

| Library | Version verified |
|---|---|
| — | nothing beyond the core |

## Build and flash

```bash
arduino-cli compile -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 .
arduino-cli upload  -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 -p /dev/ttyUSB0 .
```

Board: **ESP8266 — NodeMCU 1.0 / Wemos D1 mini (ESP-12E)**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
