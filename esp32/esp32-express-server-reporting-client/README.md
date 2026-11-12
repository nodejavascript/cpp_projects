# ESP32 reporting client for an Express server

**The why.** A device that cannot say it is alive is indistinguishable from one that is not.

Keeps a Wi-Fi link to a server and POSTs status to it, and serves three endpoints of its
own: `/` for liveness, `/ping` for uptime, and `/authorize` which changes the server address
but only for a caller presenting the shared token.

## Files

- `connection.ino`
- `esp32-express-server-reporting-client.ino`

## Hardware

ESP32 (DevKitC / WROOM-32)

## Libraries

| Library | Version verified |
|---|---|
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
