# MQTT publish and subscribe demo

**The why.** Before a sensor network, prove the broker and the client can hear each other.

Connects to a broker, subscribes to one topic, publishes a message on connect, then
publishes its uptime every 50 ms and prints anything that comes back.

## Files

- `esp8266-mqtt-publish-subscribe-demo.ino`

## Hardware

NodeMCU / Wemos D1 mini · an MQTT broker on the local network

## Libraries

| Library | Version verified |
|---|---|
| `EspMQTTClient` | 1.13.3 |

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
