# C++ projects

Arduino Nano, ESP8266 and ESP32 firmware — 27 sketches, each in its own folder.

**The why.** A sensor reading that lives only on a serial monitor dies with the cable. Every project here exists because a number had to be written down, seen on a screen, or acted on by something other than the person holding the laptop — and each one was built on a bench, pointed at a real problem in a real room.

This is the collected C++ work — sketches for ESP8266, ESP32 and the Arduino Nano — brought into one place so it can be read, rebuilt and kept working instead of sitting on a drive in a folder nobody opens.

**27 projects. All of them compile against the current toolchain**, checked twice: once with the author's own settings and once from a clean clone where only `config.h.example` exists.

## Using this

Copy a project's `config.h.example` to `config.h` and fill in your own values — Wi-Fi, broker, tokens. `config.h` is git-ignored and never leaves your machine; only the example with placeholders is tracked. Then open the folder in the Arduino IDE, or build it from the command line:

```bash
arduino-cli compile -b esp8266:esp8266:d1_mini esp8266/esp8266-bmp280-temperature-mqtt-publisher
```

Every folder is self-contained and each has its own README saying what it does, which board it targets and which libraries it needs.

## Layout

Three folders, one per platform. The folder name is the project name and matches the `.ino` inside, so each folder opens directly in the Arduino IDE.

| Platform | Board | Projects |
|---|---|---|
| [`arduino-uno-nano/`](./arduino-uno-nano) | Arduino Nano / Uno (ATmega328P) | 8 |
| [`esp8266/`](./esp8266) | NodeMCU / Wemos D1 mini | 12 |
| [`esp32/`](./esp32) | ESP32-WROOM-32 / ESP32-CAM | 7 |

## ESP8266 — 12 projects

| Project | What it does |
|---|---|
| [esp8266-ina219-battery-logger-sd-card-oled](./esp8266/esp8266-ina219-battery-logger-sd-card-oled) | Two INA219 sensors logged to SD with an RTC timestamp, NTP clock sync, OLED and a serial report per subsystem |
| [esp8266-ina219-battery-charge-monitor-oled-mqtt](./esp8266/esp8266-ina219-battery-charge-monitor-oled-mqtt) | INA219 charge monitor publishing eight values as JSON every 10 s, two-page OLED, pot sets refresh rate |
| [esp8266-bme280-weather-station-keyfob](./esp8266/esp8266-bme280-weather-station-keyfob) | The fullest build: BME280, RTC, SD logging, paged OLED, HTTP endpoints, Wi-Fi reconnect, run-time network switch |
| [esp8266-fan-switch-bmp280-oled-web-server](./esp8266/esp8266-fan-switch-bmp280-oled-web-server) | Fan relay with BMP280, OLED and a web page for on / off / auto |
| [esp8266-async-web-server-mqtt-client](./esp8266/esp8266-async-web-server-mqtt-client) | Async web server that accepts JSON, parses it, and can forward it as an HTTP client — with a request log |
| [esp8266-pir-motion-door-sensor-mqtt](./esp8266/esp8266-pir-motion-door-sensor-mqtt) | PIR and reed switch to MQTT, four LEDs separating "no motion" from "no network" |
| [esp8266-laundry-done-notifier-webhook](./esp8266/esp8266-laundry-done-notifier-webhook) | Watches the machine's motion and calls a webhook when it stops |
| [esp8266-bmp280-temperature-mqtt-publisher](./esp8266/esp8266-bmp280-temperature-mqtt-publisher) | Three MQTT topics a second from one BMP280 |
| [esp8266-solar-panel-dual-axis-sun-tracker](./esp8266/esp8266-solar-panel-dual-axis-sun-tracker) | Two servos hill-climb for the angle that produces the most power, reading an INA219 as they go |
| [esp8266-wifi-access-point-station-bridge](./esp8266/esp8266-wifi-access-point-station-bridge) | Access point and station at once, on a pinned static address |
| [esp8266-mqtt-publish-subscribe-demo](./esp8266/esp8266-mqtt-publish-subscribe-demo) | Round-trip MQTT test — publish, subscribe, print |
| [esp8266-i2c-address-scanner](./esp8266/esp8266-i2c-address-scanner) | Walks the I²C bus and prints every address that answers |

## ESP32 — 7 projects

| Project | What it does |
|---|---|
| [esp32-cam-video-streaming-web-server](./esp32/esp32-cam-video-streaming-web-server) | Espressif's camera server with this board's pin map — live stream, resolution control, face detection |
| [esp32-ina219-power-monitor-bluetooth-sdcard](./esp32/esp32-ina219-power-monitor-bluetooth-sdcard) | Power monitor logged to SD, shown on OLED, readable over Bluetooth Serial |
| [esp32-truck-key-fob-remote-web-server](./esp32/esp32-truck-key-fob-remote-web-server) | Web page that fires the truck's key-fob wiring |
| [esp32-express-server-reporting-client](./esp32/esp32-express-server-reporting-client) | Posts status to an Express server, with a token-protected endpoint to move the server address |
| [esp32-433mhz-rf-receiver-wifi-oled](./esp32/esp32-433mhz-rf-receiver-wifi-oled) | 433 MHz receiver that shows what it heard on a 128×32 OLED |
| [esp32-mqtt-firmware-web-updater](./esp32/esp32-mqtt-firmware-web-updater) | MQTT client with the HTTP firmware updater on, plus a last-will message |
| [esp32-robot-arm-servo-controller](./esp32/esp32-robot-arm-servo-controller) | Three servos on a two-axis joystick, calibrated to its own centre at start-up |

## Arduino Nano / Uno — 8 projects

| Project | What it does |
|---|---|
| [arduino-atx-bench-power-supply-ina219-oled](./arduino-uno-nano/arduino-atx-bench-power-supply-ina219-oled) | Switches on a salvaged ATX supply and measures what it delivers |
| [arduino-solar-power-supply-ina219-oled](./arduino-uno-nano/arduino-solar-power-supply-ina219-oled) | Solar panel output on an OLED |
| [arduino-nano-bme280-oled-temperature-display](./arduino-uno-nano/arduino-nano-bme280-oled-temperature-display) | Temperature and humidity on a 128×32 OLED, blanking when idle |
| [arduino-nano-433mhz-rf-transmitter-oled-display](./arduino-uno-nano/arduino-nano-433mhz-rf-transmitter-oled-display) | Photocell to 433 MHz, with the value shown as it leaves |
| [arduino-vibration-sensor-oled-monitor](./arduino-uno-nano/arduino-vibration-sensor-oled-monitor) | Count, average, min and max of each vibration pulse |
| [arduino-ssd1306-analog-signal-oscilloscope](./arduino-uno-nano/arduino-ssd1306-analog-signal-oscilloscope) | A pocket oscilloscope for A0 on a 128×64 OLED |
| [arduino-cat-laser-servo-toy](./arduino-uno-nano/arduino-cat-laser-servo-toy) | Two servos, one laser, one cat |
| [arduino-serial-port-listener](./arduino-uno-nano/arduino-serial-port-listener) | Parses `key=value` off a serial line |

## Credentials — every project that needs them uses `config.h`

Sixteen projects need a Wi-Fi network, a broker or a token. In all sixteen the values live in **`config.h`, which git ignores**:

```bash
cd esp8266/esp8266-bme280-weather-station-keyfob
cp config.h.example config.h
$EDITOR config.h
```

`config.h.example` is tracked and holds placeholders only. It is the file that proves the project builds for anyone who clones it — and it is the exact path this repository was verified through.

Sixteen projects need one, and **all sixteen have a `config.h.example`**.

A full credential audit of every tracked file — Wi-Fi passwords, broker logins,
Slack webhook URLs, data-server API keys, private keys, connection strings and
every high-entropy string in the tree — finds **nothing**. The values live only
in each project's local `config.h`.

### Protection, so a credential cannot get in again

A commit that carries a credential is **refused by a pre-commit hook**, and a push
is **refused by CI**. Both look for the shape of a secret rather than for a list of
known values, because a list only catches what has already burned you.

```bash
python3 tools/scan-secrets.py --install-hook     # once, after cloning
python3 tools/scan-secrets.py                    # scan the tracked tree
```

Hooks do not travel with `git clone`, so that install step is the one thing to
remember on a new machine. Everything else — the `.gitignore` rules, the CI
workflow, the scanner itself — is in the repository. See [SECURITY.md](./SECURITY.md)
for what the scanner checks, what it cannot check, and the four credentials that
reached this repository before it existed.


## Building anything here

```bash
arduino-cli core install arduino:avr esp8266:esp8266 esp32:esp32
arduino-cli lib install "Adafruit GFX Library" "Adafruit SSD1306" "Adafruit BusIO" \
  "Adafruit INA219" "Adafruit BMP280 Library" "Adafruit BME280 Library" \
  "Adafruit Unified Sensor" ArduinoJson RTClib Time RadioHead EspMQTTClient \
  PubSubClient Servo ESP32Servo
```

The ESP8266 and ESP32 async web servers are different forks and cannot share a library folder. Keep them apart and pass the right one at build time:

```bash
git clone https://github.com/me-no-dev/ESPAsyncWebServer.git ~/.arduino-libs/esp8266/ESPAsyncWebServer
git clone https://github.com/me-no-dev/ESPAsyncTCP.git    ~/.arduino-libs/esp8266/ESPAsyncTCP
git clone https://github.com/ESP32Async/ESPAsyncWebServer.git ~/.arduino-libs/esp32/ESPAsyncWebServer
git clone https://github.com/ESP32Async/AsyncTCP.git          ~/.arduino-libs/esp32/AsyncTCP
```

Then, from inside a project folder:

```bash
arduino-cli compile -b esp8266:esp8266:d1_mini --libraries ~/.arduino-libs/esp8266 .
arduino-cli compile -b esp32:esp32:esp32:PartitionScheme=huge_app --libraries ~/.arduino-libs/esp32 .
```

Each project's own README carries the exact command for its board.

## Duplicates that were merged

Two projects were the same work at different ages. Both were removed, and their content survives in the copy that was kept:

| Removed | Kept instead | Why |
|---|---|---|
| `esp8266-mqtt-subscribe-demo` | [`esp8266-mqtt-publish-subscribe-demo`](./esp8266/esp8266-mqtt-publish-subscribe-demo) | The subscribe demo was a strict subset — identical client setup and identical `onConnectionEstablished()`, without the uptime publisher |
| `esp8266-ina219-power-monitor-oled-sdcard` | [`esp8266-ina219-battery-logger-sd-card-oled`](./esp8266/esp8266-ina219-battery-logger-sd-card-oled) | The 2021 monitor stopped in February 2021; the logger is the same lineage carried to December 2023, with a second INA219, NTP sync and a fuller report |

One project was also renamed: `esp32-cam-mjpeg-stream-server` was not a camera streamer at all — it posts status to an Express server — so it is now [`esp32-express-server-reporting-client`](./esp32/esp32-express-server-reporting-client).

## What was repaired

Build failures, dead code, four sketches filed under the wrong board, and two displays that could never show a reading because the sensor reads were commented out. The full list, with the project each belongs to, is in [PATCHES.md](./PATCHES.md).

## Licence

MIT.
