# Third-party code and attribution

Most code in this repository is original. Two projects build on someone else's work, and the attribution is also carried inside the sketch headers.

| Project | Origin | Licence |
|---|---|---|
| `arduino-uno-nano/arduino-ssd1306-analog-signal-oscilloscope` | Jon E. Froehlich, makeabilitylab.io | See the header of the `.ino`; the project `LICENSE` file applies to the surrounding work |
| `esp32/esp32-cam-video-streaming-web-server` | Espressif Systems, `CameraWebServer` example shipped with the ESP32 core | Apache 2.0, as shipped with the ESP32 Arduino core |

Library licences are held by their respective authors and are not vendored here.

`app_httpd.cpp`, `camera_pins.h`, `camera_index.h` and `board_config.h`
inside `esp32/esp32-cam-video-streaming-web-server` are Espressif's, taken from the
ESP32 Arduino core's own `CameraWebServer` example (Apache 2.0). `config.h` in that
project is George's.
