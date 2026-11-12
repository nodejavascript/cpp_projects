# Laundry done notifier

**The why.** The washing machine finished twenty minutes ago and nobody knew.

Watches a vibration input, learns what a running machine looks like, and calls an HTTP
endpoint once the motion stops for long enough. Retries with a backoff and reports the last
response code.

## Files

- `client.ino`
- `esp8266-laundry-done-notifier-webhook.ino`
- `wifi.ino`

## Hardware

NodeMCU / Wemos D1 mini · vibration or accelerometer input

## Libraries

| Library | Version verified |
|---|---|
| `ArduinoJson` | 7.4.2 |

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
