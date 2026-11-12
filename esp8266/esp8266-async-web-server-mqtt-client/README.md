# ESP8266 asynchronous web server with a JSON API

**The why.** A device you can only talk to by re-flashing it is a device you cannot operate.

Serves an async web server that accepts JSON POSTs, parses them with ArduinoJson, and can
forward a payload onward as an HTTP client. Each request and response is recorded, so the
last call, its code, its timing and its error are readable from a browser.

## Files

- `client.ino`
- `esp8266-async-web-server-mqtt-client.ino`
- `server.ino`
- `wifi.ino`

## Hardware

NodeMCU 0.9 / ESP-12 · ESP8266 core 3.x

## Libraries

| Library | Version verified |
|---|---|
| `ArduinoJson` | 7.4.2 |
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
