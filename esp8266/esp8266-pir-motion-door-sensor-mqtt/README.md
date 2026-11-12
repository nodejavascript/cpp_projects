# PIR motion and door sensor to MQTT

**The why.** A door that opens when nobody is there is worth knowing about.

Watches an HC-SR505 on D2 and a magnetic reed switch on D1. Publishes `…/DOOR` and `…/PIR`
as JSON on every state change, a heartbeat every ten seconds, and `…/CONNECTED` when the
broker accepts the client. Four LEDs — Wi-Fi, broker, motion and fault — make a dead sensor
look different from a dead network.

## Files

- `esp8266-pir-motion-door-sensor-mqtt.ino`

## Hardware

LoLin(WEMOS) D1 mini · HC-SR505 PIR · magnetic reed switch · 4 LEDs with resistors

## Libraries

| Library | Version verified |
|---|---|
| `ArduinoJson` | 7.4.2 |
| `PubSubClient` | 2.8.0 |

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
