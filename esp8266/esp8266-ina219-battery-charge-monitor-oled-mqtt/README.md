# INA219 battery charge monitor with OLED and MQTT

**The why.** Charging a battery tells you nothing until you can watch the current fall.

INA219 plus an SSD1306. Publishes shunt voltage, bus voltage, current, power, load voltage
and the derived amps and watts as one JSON document every ten seconds. A button pages
between a large three-number view and a five-value detail view; the pot on A0 sets how often
the display refreshes.

## Files

- `esp8266-ina219-battery-charge-monitor-oled-mqtt.ino`
- `ssd1306.ino`

## Hardware

Wemos D1 mini · INA219 · SSD1306 128×64 OLED · button D6 · pot A0

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit INA219` | 1.2.3 |
| `Adafruit SSD1306` | 2.5.15 |
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
