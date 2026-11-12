# BMP280 temperature, pressure and altitude to MQTT

**The why.** A sensor reading that lives only on a serial monitor dies with the cable.

Reads a BMP280 at 0x76 once a second and publishes three separate MQTT topics — temperature,
pressure in hectopascals, and altitude from a 1013 hPa sea-level constant. EspMQTTClient
handles Wi-Fi and broker reconnection.

## Files

- `esp8266-bmp280-temperature-mqtt-publisher.ino`

## Hardware

NodeMCU / Wemos D1 mini · BMP280 breakout

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit BMP280 Library` | 2.6.8 |
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
