# INA219 battery logger with SD card, RTC and OLED

**The why.** A logger that loses its history on a power cut is a logger that lies about the past.

Two INA219 sensors — one on the battery terminals, one on the 5 V source — log voltage,
current and power to a CSV on an SD card with a DS3231 timestamp. The clock re-syncs from
NTP over HTTPS and records whether that succeeded; every subsystem has a text report
function for the serial monitor.

## Files

- `client.ino`
- `clock.ino`
- `esp8266-ina219-battery-logger-sd-card-oled.ino`
- `ina.ino`
- `oled.ino`
- `reading.ino`
- `sdcard.ino`
- `serial.ino`
- `wifi.ino`

## Hardware

NodeMCU 0.9 (ESP-12) · 2 × INA219 · DS3231 RTC · SD card · SSD1306 OLED

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit INA219` | 1.2.3 |
| `Adafruit SSD1306` | 2.5.15 |
| `ArduinoJson` | 7.4.2 |
| `RTClib` | 2.1.4 |
| `Time` | 1.6.1 |

## Build and flash

```bash
arduino-cli compile -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 .
arduino-cli upload  -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 -p /dev/ttyUSB0 .
```

Board: **ESP8266 — NodeMCU 1.0 / Wemos D1 mini (ESP-12E)**.

## Configuration

All credentials live in `config.h`, which git ignores. Copy the example and fill it in:

```bash
cp config.h.example config.h
```

`config.h.example` carries placeholders only — it is what proves the project still builds for anyone who clones it. Nothing in the tracked tree holds a real password.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
