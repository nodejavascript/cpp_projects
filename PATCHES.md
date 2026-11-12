# Changes made to get this code working again

Two rounds of work. **The first** made the code compile against the current
toolchain. **The second** moved every credential into a git-ignored `config.h`,
removed dead code, and rewrote the sketches whose real logic was switched off.
Neither round changes what a project is for. Every line below was verified by
compiling the project afterwards.

---

## Round two — credentials, dead code, and switched-off logic

### Credentials moved to `config.h` (16 projects)

Every sketch that held a Wi-Fi network, a broker, a token or a server address in
plain text now reads it from `config.h`. The tracked file is `config.h.example`,
which carries placeholders only; `config.h` is git-ignored.

| Style found in the original | Projects | What replaced it |
|---|---|---|
| `const char* ssid = "…"` inline | camera streamer, 433 MHz receiver, Express client | `WIFI_SSID` / `WIFI_PASSWORD` from `config.h` |
| Positional `EspMQTTClient client("ssid", "pass", "host", …)` | MQTT pub-sub demo, BMP280 publisher, MQTT firmware updater | named macros, so the argument order is now readable |
| Two networks chosen by a run-time flag (`useHouseWIfi`, `usePhoneHotspot`) | 7 projects | `WIFI_SSID_HOME` / `WIFI_PASS_HOME` / `WIFI_IP_HOME` and the `_ALT` set, one macro per value |
| `#define WIFI_SSID "…"` at the top of the sketch | access point bridge | `config.h`, with the AP credentials separated from the home ones |

Three sketches also carried an authentication token or a server URL — the
Express reporting client had both. Those moved too, because a token in a
repository is a password in a repository.

**No credential value was changed.** The real values are preserved in each
project's local `config.h`; only the tracked example carries placeholders.

Two flags were renamed because their names argued with their values:
`useHouseWIfi` became `useAlternateNetwork`, and `usePhoneHotspot` — which was
true for the *home* network — became `useHomeNetwork`.

### Logic that had been switched off, restored

| Project | What was found | What was done |
|---|---|---|
| `arduino-atx-bench-power-supply-ina219-oled` | `ina219.begin()` and all four sensor reads were commented out, so the display could only ever show `0.00 V` and `0 mW` | restored, so the project does what its name says |
| `esp8266-solar-panel-dual-axis-sun-tracker` | `moveHorizontal()`, `moveVertical()` and `voltage()` — the whole tracking algorithm — were written but never called; the loop just swept both servos 0→180 together | the hill climb is wired back into `loop()` |
| `esp8266-mqtt-publish-subscribe-demo` | `delay(50)` sat between two publishes, so the "publish our uptime" call ran at a rate nobody chose | the delay is inside the publish helper, which is what it was there for |

### Sketches rewritten for readability

These five were rewritten rather than patched, because the original was mostly
dead code interleaved with live code:

| Project | Lines before → after | What changed |
|---|---|---|
| `arduino-cat-laser-servo-toy` | 199 → 118 | debug plotting removed, the two interruption behaviours given names, the speed and wait ranges lifted into named constants |
| `arduino-vibration-sensor-oled-monitor` | 101 → 92 | nested `if`/`else` flattened into early returns, the display string built with `snprintf` instead of eleven `String` concatenations |
| `arduino-serial-port-listener` | 47 → 62 | `String(character, DEC) == "10"` string comparison replaced with `character == '\n'`; the parser is now one function per job |
| `esp32-express-server-reporting-client` | 136 + 53 → 108 + 54 | name corrected (it is not a camera), a redeclared `const AsyncWebParameter* p` fixed, `/authorize` now answers 400 or 403 instead of silently doing nothing |
| `esp8266-solar-panel-dual-axis-sun-tracker` | 137 → 101 | see above |

### Dead code removed

115 lines of commented-out code were removed from six projects — the ESP32 power
monitor, the key fob, the battery logger, the truck fob, the 433 MHz receiver and
the laundry notifier. What was removed was code that had been disabled: dead
assignments, disabled `Serial` prints, an `#include` of a library no longer used.
Comments that explain *why* something is done were all kept.

Two misspelt identifiers were corrected across fifteen files:
`printOLED` → `printOled`, and `refeshFileStatus` → `refreshFileStatus`.

### Duplicates merged

| Removed | Kept | Reason |
|---|---|---|
| `esp8266-mqtt-subscribe-demo` | `esp8266-mqtt-publish-subscribe-demo` | strict subset: same client setup, same `onConnectionEstablished()`, minus the uptime publisher |
| `esp8266-ina219-power-monitor-oled-sdcard` | `esp8266-ina219-battery-logger-sd-card-oled` | the 2021 monitor's history stops in February 2021; the logger is the same code carried to December 2023 with a second sensor and NTP sync |

Both were moved to a quarantine directory outside the repository, not deleted.

### Files absorbed into their project

Where a project had separate `oled.ino`, `bme.ino` or `formatting.ino` files that
were only ever called from the main sketch, they were folded in so the project
reads top to bottom.


### A credential found on the second pass (12 November 2026)

Two sketches — the laundry notifier and the fan switch — carried a **live Slack
incoming-webhook URL** in `client.ino`:

```
https://hooks.slack.com/services/<workspace>/<channel>/<token>
```

A Slack webhook URL *is* the credential: anyone holding it can post to that
channel. Both now read `SLACK_WEBHOOK_URL` from `config.h`, and only a
placeholder is tracked.

**That URL was pushed to GitHub before this was caught.** Treat it as exposed
and regenerate the webhook in Slack — a private repository is not enough once a
value has travelled.


### The full audit also found three API keys (12 November 2026)

A credential scan that looked for *shapes* rather than for the values already
known turned up three more, all 22 characters long, all for one third-party
data server:

| Project | What leaked |
|---|---|
| `esp32-ina219-power-monitor-bluetooth-sdcard` | a server access key |
| `esp8266-bme280-weather-station-keyfob` | a server access key |
| `esp8266-ina219-battery-logger-sd-card-oled` | a server access key |
| `esp8266-async-web-server-mqtt-client` | the server URL only — its key arrives in the POST |

Each project now reads `DATA_SERVER_URL` and `DATA_ACCESS_KEY` from `config.h`.
**All three keys were pushed to GitHub and must be treated as exposed.** They were
deleted from every copy on the machine on 13 November 2026, and the server, its
URL and every fragment of the keys were removed from this repository at the same
time — a partly-quoted credential is still a credential.

The lesson is worth recording: the first two passes searched for the credential
*values* already known and for `const char*` and `#define` declarations, so an
`accessKey` assigned to a plain `char*` or `String` never matched. The shape
scan — every quoted string of 16–80 characters with high entropy and mixed
case — is what found them.

---

## Round one — making it compile

| Project | Defect | Fix |
|---|---|---|
| `esp8266-ina219-battery-charge-monitor-oled-mqtt` | `config.h.example` never defined `MQTT_PASSWORD`, so the sketch could not build from the example | added the define |
| `esp8266-pir-motion-door-sensor-mqtt` | `config.h.example` had an unterminated `MQTT_SERVER` string and no `MQTT_TOPIC` at all | closed the quote, added `MQTT_TOPIC` and `MQTT_PORT` |
| `esp8266-ina219-battery-logger-sd-card-oled` | the `Adafruit_SSD1306 display` object was commented out | uncommented |
| `esp8266-ina219-battery-logger-sd-card-oled` | `resetHttpResponse()` assigned its own parameters to themselves, so the globals were never set | parameters renamed so the assignment reaches the globals |
| `esp8266-ina219-battery-logger-sd-card-oled` | four functions built a report string and never returned it | `return ui;` added |
| battery logger, power monitor, key fob | `formattedWifiStatus()` had no path that returned a value | `default: return "UNKNOWN";` |
| laundry, fan switch, battery logger | `while (!isConnected()) return true;` — the `return` *was* the loop body, so the function could never return | `while (isConnected()) delay(10); return true;` |
| key fob, fan switch, truck fob, Express client | `AsyncWebParameter* p = request->getParam(...)` — the current library returns a const pointer | `const AsyncWebParameter*` |
| `esp8266-async-web-server-mqtt-client` | `DynamicJsonDocument`, removed in ArduinoJson 7 | `JsonDocument` |
| `esp8266-async-web-server-mqtt-client` | an unused `#include <AsyncJson.h>` suppressed ArduinoJson entirely, so nothing after it compiled | include removed |
| `esp32-robot-arm-servo-controller` | `Servo_ESP32.h` exists only in one tutorial repository | switched to the maintained `ESP32Servo`, same API |
| `esp32-cam-video-streaming-web-server` | `app_httpd.cpp` used `fd_forward.h`, removed in ESP32 core 3.x | replaced with the driver file from the core's own example; sketch ported to `board_config.h` |
| `esp32-ina219-power-monitor-bluetooth-sdcard` | exceeded the default 1.3 MB app partition | built with `PartitionScheme=huge_app` |

### Four sketches were filed under the wrong board

| Sketch | Was | Is | Evidence |
|---|---|---|---|
| 433 MHz RF receiver with OLED | ESP8266 | ESP32 | its own header says ESP32-DevKitC / WROOM-32U, and it includes `WiFi.h` |
| 433 MHz RF transmitter with OLED | ESP8266 | Nano | header says Mini Nano ATmega328P; it reads analog pin A7 |
| MQTT firmware web updater | ESP8266 | ESP32 | includes `ESP32HTTPUpdateServer.h` |
| Dual-axis sun tracker | AVR | ESP8266 | uses `D7` / `D8` pin names |

---

## One library patch, needed on any machine

`EspMQTTClient` 1.13.3 does not include `<WiFi.h>` in its ESP32 branch, and ESP32
core 3.x no longer pulls it in transitively, so the library fails to compile in
any sketch that uses it.

```c
// ~/Arduino/libraries/EspMQTTClient/src/EspMQTTClient.h, ESP32 branch
+ #include <WiFi.h>
  #include <WiFiClient.h>
```

This is an upstream bug, not a defect in these projects. Only
`esp32-mqtt-firmware-web-updater` is affected, and its sketch also includes
`<WiFi.h>` first as a second line of defence.
