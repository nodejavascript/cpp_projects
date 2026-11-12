# ESP32-CAM video streaming web server

**The why.** A camera you have to walk to is a window, not a camera.

Espressif's camera server with this board's pin map and settings: a web page with the live
stream, resolution and quality controls, face detection and the flash LED. The multi-file
form — sketch, app_httpd.cpp, pin map and embedded index page.

## Files

- `esp32-cam-video-streaming-web-server.ino`

## Hardware

ESP32-CAM (AI-Thinker), PSRAM required for UXGA

## Libraries

| Library | Version verified |
|---|---|
| — | nothing beyond the core |

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

**Third-party.** Espressif's CameraWebServer example, Apache 2.0. See NOTICE.md.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
