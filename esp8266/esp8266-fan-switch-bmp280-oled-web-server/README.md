# Fan switch with BMP280, OLED and web control

**The why.** A fan that turns itself on when the room is actually hot is worth more than a schedule.

Reads a BMP280, drives a fan relay and serves a web page with on, off and automatic control.
The OLED shows temperature and state, and every request is kept in a status record so the
last command and its result survive a page reload.

## Files

- `client.ino`
- `esp8266-fan-switch-bmp280-oled-web-server.ino`
- `oled.ino`
- `server.ino`
- `wifi.ino`

## Hardware

NodeMCU 1.0 (ESP-12E) · BMP280 · SSD1306 OLED · relay or MOSFET for the fan

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit BMP280 Library` | 2.6.8 |
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit SSD1306` | 2.5.15 |
| `ArduinoJson` | 7.4.2 |
| `ESP Async TCP` | verified |
| `ESP Async WebServer (esp32)` | 3.x |

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
