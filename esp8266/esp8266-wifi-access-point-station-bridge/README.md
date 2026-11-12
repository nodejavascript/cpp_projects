# Wi-Fi access point and station bridge

**The why.** A network you cannot reach from the garden is a network with a border.

Runs the ESP8266 in access point and station mode at once: it joins the home network as a
client while serving its own network, on a fixed address so it is always reachable.

## Files

- `esp8266-wifi-access-point-station-bridge.ino`

## Hardware

Any ESP8266 — NodeMCU or D1 mini

## Libraries

| Library | Version verified |
|---|---|
| — | nothing beyond the core |

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
