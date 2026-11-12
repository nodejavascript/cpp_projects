# ESP32 power monitor with Bluetooth, SD card and OLED

**The why.** A power meter that needs a laptop to read it is a power meter you stop reading.

INA219 voltage and current logged to SD with an RTC timestamp, shown on an SSD1306, and
readable over Bluetooth Serial so a phone can poll it on the bench.

## Files

- `bluetooth.ino`
- `client.ino`
- `clock.ino`
- `esp32-ina219-power-monitor-bluetooth-sdcard.ino`
- `ina.ino`
- `oled.ino`
- `reading.ino`
- `sdcard.ino`
- `wifi.ino`

## Hardware

ESP32-WROOM-32U (DOIT DevKit V1) · INA219 · DS3231 RTC · SD card · SSD1306 OLED

## Libraries

| Library | Version verified |
|---|---|
| `Adafruit GFX Library` | 1.12.1 |
| `Adafruit INA219` | 1.2.3 |
| `Adafruit SSD1306` | 2.5.15 |
| `ArduinoJson` | 7.4.2 |
| `RTClib` | 2.1.4 |
| `Time` | 1.6.1 |

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

Exceeds the default 1.3 MB app partition — build with `PartitionScheme=huge_app`.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
