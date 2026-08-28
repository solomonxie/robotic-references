# hello-esp32

Progressive learning series for ESP32 (Arduino framework, C/C++, `.ino` sketches —
same toolchain as hello-arduino). Each step is a standalone file that builds on the
previous one, adding exactly one new concept.

## Steps

| File | Description |
|------|-------------|
| `hello_02_button.ino` | Read a pushbutton state over Serial |
| `hello_03_3lights.ino` | Cycle green/red/blue LEDs on a breadboard |

## Deploying

Requires [`arduino-cli`](https://arduino.github.io/arduino-cli/) (`brew install arduino-cli`;
`make` installs it for you if missing). `deploy.sh` auto-detects the connected serial port
and chip via `esptool` (installed into a local `venv/`), then uploads at a preset speed for
that chip family (115200 by default). If that speed fails, it prompts you to pick another
from a list instead of silently retrying every speed.

```sh
make deploy F=hello_03_3lights.ino
make detect                              # just show port + chip info
make deploy F=hello_03_3lights.ino SPEED=230400   # override the preset speed
```
