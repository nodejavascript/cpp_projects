# MQTT client with an HTTP firmware updater

**The why.** A device on a shelf you have to unplug to update is a device that stops getting updates.

Connects to Wi-Fi and a broker, then switches on EspMQTTClient's built-in web updater so a
new binary can be pushed over the network. A last-will message tells the broker when the
device falls off, and it demonstrates both a plain and a wildcard subscription plus a
delayed publish.

## Files

- `esp32-mqtt-firmware-web-updater.ino`

## Hardware

ESP32 (DevKitC / WROOM-32)

## Libraries

| Library | Version verified |
|---|---|
| `EspMQTTClient` | 1.13.3 |

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

## Notes

EspMQTTClient 1.13.3 omits `<WiFi.h>` in its ESP32 branch, which core 3.x requires — one line added to the library. See PATCHES.md.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
