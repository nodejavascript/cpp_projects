# Serial port listener — key=value command parser

**The why.** Before you write a protocol, listen to what is already on the wire.

Reads a line from serial, splits it at the first `=`, and prints the key and value it found.
Type `brightness=250` and watch it come back parsed.

## Files

- `arduino-serial-port-listener.ino`

## Hardware

Arduino Nano or Uno, or any board with a serial line to spy on

## Libraries

| Library | Version verified |
|---|---|
| — | nothing beyond the core |

## Build and flash

```bash
arduino-cli compile -b arduino:avr:nano  .
arduino-cli upload  -b arduino:avr:nano  -p /dev/ttyUSB0 .
```

Board: **Arduino Nano (ATmega328P) / Arduino Uno**.

## Licence

MIT, unless a `LICENSE` file in this folder says otherwise.
