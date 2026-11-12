# Truck key fob remote over Wi-Fi

**The why.** The one time you need the truck to unlock is the one time the key fob is in the house.

A web page served from the ESP32 that posts a command to the vehicle's key-fob wiring, so
the truck answers to a browser on the same network. Reports the last command and its result
and keeps a fixed address on either of two networks.

## Files

- `esp32-truck-key-fob-remote-web-server.ino`
- `server.ino`
- `wifi.ino`

## Hardware

ESP32-WROOM-32U (DOIT DevKit V1) · relay or opto interface to the vehicle

## Libraries

| Library | Version verified |
|---|---|
| `ArduinoJson` | 7.4.2 |
| `Async TCP` | 3.x |
| `ESP Async WebServer (esp32)` | 3.x |

## Build and flash

```bash
arduino-cli compile -b esp32:esp32:esp32:PartitionScheme=huge_app --libraries ~/.arduino-libs/esp32 .
arduino-cli upload  -b esp32:esp32:esp32:PartitionScheme=huge_app --libraries ~/.arduino-libs/esp32 -p /dev/ttyUSB0 .
```

Board: **ESP32 — DOIT ESP32 DevKit V1 / ESP32-WROOM-32**.

## Configuration

All credentials live in `config.h`, which git ignores. Copy the example and fill it in:

```bash
cp config.h.example config.h
```

`config.h.example` carries placeholders only — it is what proves the project still builds for anyone who clones it. Nothing in the tracked tree holds a real password.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
