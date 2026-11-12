# ESP8266 weather station key fob

**The why.** A sensor on a shelf reports; a sensor you carry tells you what the room is doing right now.

The fullest of the ESP8266 builds: BME280 temperature, humidity and pressure, a DS3231
clock, SD-card logging, an OLED that pages through the readings, HTTP endpoints for a phone
to poll, and Wi-Fi that reconnects by itself. A button cycles the display and triggers a log
line, and it can switch networks at run time.

## Files

- `bme.ino`
- `client.ino`
- `datetime.ino`
- `endpoints.ino`
- `esp8266-bme280-weather-station-keyfob.ino`
- `oled.ino`
- `sdcard.ino`
- `wifi.ino`

## Hardware

NodeMCU 1.0 (ESP-12E) · BME280 · DS3231 RTC · SD card · SSD1306 128×32 OLED · push button

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit BME280 Library` | 2.3.0 |
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit SSD1306` | 2.5.15 |
| `ArduinoJson` | 7.4.2 |
| `ESP Async TCP` | verified |
| `ESP Async WebServer (esp32)` | 3.x |
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
