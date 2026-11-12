# 433 MHz RF receiver with OLED and Wi-Fi

**The why.** A receiver that cannot show you what it heard is a receiver you cannot trust.

Listens on a 433 MHz ASK receiver, decodes each packet with RadioHead, formats it, and shows
the last message on a 128×32 OLED while the Wi-Fi stack stays ready to forward it.

## Files

- `display.ino`
- `esp32-433mhz-rf-receiver-wifi-oled.ino`
- `formatting.ino`
- `wifi.ino`

## Hardware

ESP32-DevKitC / WROOM-32U · 433 MHz receiver · SSD1306 128×32 OLED

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit SSD1306` | 2.5.15 |
| `RadioHead` | 1.143.1 |

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
