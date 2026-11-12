# Dual-axis solar panel sun tracker

**The why.** A fixed panel is a compromise with the sun; a panel that follows it is an argument won.

Two servos aim a panel, one per axis. Each step nudges an axis, measures the panel's output
through an INA219, and keeps the new angle only if the output did not drop — a hill climb on
the sun. An axis that starts losing power reverses.

## Files

- `esp8266-solar-panel-dual-axis-sun-tracker.ino`

## Hardware

ESP8266 (NodeMCU / D1 mini — servo pins D7 and D8) · 2 × servo · panel · INA219

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit INA219` | 1.2.3 |
| `Servo (built in)` | 1.2.2 |

## Build and flash

```bash
arduino-cli compile -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 .
arduino-cli upload  -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 -p /dev/ttyUSB0 .
```

Board: **ESP8266 — NodeMCU 1.0 / Wemos D1 mini (ESP-12E)**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
