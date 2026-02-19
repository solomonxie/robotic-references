# hello-esp32

Progressive learning series for ESP32 (Arduino framework, C/C++, `.ino` sketches —
same toolchain as hello-arduino). Each step is a standalone file that builds on the
previous one, adding exactly one new concept.

## Steps

| File | Description |
|------|-------------|
| `hello_03_3lights.ino` | Cycle green/red/blue LEDs on a breadboard |

## Deploying

Requires [`arduino-cli`](https://arduino.github.io/arduino-cli/) (`brew install arduino-cli`).
`deploy.sh` auto-detects the connected serial port, prints the exact ESP32 chip via
`esptool` (installed into a local `venv/`), and tries upload speeds from 921600 down to
57600 until one works — no need to hardcode a port or baud rate.

```sh
make deploy hello_03_3lights.ino
make detect                              # just show port + chip info
make deploy hello_03_3lights.ino SPEED=115200   # skip the speed search
```
