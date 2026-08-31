# hello-sensors

Progressive learning series for common sensors -- proximity, motion,
temperature, gas, sound, the ESP32's onboard WiFi/Bluetooth radios, and the
Raspberry Pi's camera/audio output. Steps 1-9 are ESP32 sketches (same
toolchain as [hello-esp32](../hello-esp32)); steps 10-11 switch to the
Raspberry Pi in Python, since a camera and real audio output are things the
Pi has and the ESP32 doesn't (see [robo-car](../robo-car)'s architecture,
where the Pi is the "brain" and the ESP32 is the motor/sensor controller).

## Deploying (steps 1-9, ESP32)

Requires [`arduino-cli`](https://arduino.github.io/arduino-cli/) (`brew install arduino-cli`;
`make` installs it for you if missing). `deploy.sh` auto-detects the connected serial port
and chip via `esptool` (installed into a local `venv/`), then uploads at a preset speed for
that chip family (115200 by default).

```sh
make deploy F=hello_02_ultrasonic.ino
make detect                              # just show port + chip info
```

## Running (steps 10-11, Raspberry Pi)

These run on the Pi itself, not through `deploy.sh` -- see each file's header
comment for setup (`python3 -m venv venv`, then the specific pip install).

## Phase plan

1. IR obstacle avoidance -- digital proximity, simplest sensor pattern (own
   hardware, robo-car's kit)
2. Ultrasonic distance (HC-SR04) -- a *timed* digital signal instead of a
   plain on/off one (own hardware)
3. PIR motion -- digital again, but a different sensing principle (ambient
   heat, not reflected IR) and a warm-up/retrigger quirk step 1 didn't have
4. Thermistor temperature -- first analog sensor, voltage divider + a
   resistance-to-temperature formula (own hardware, the Fun Kit's
   thermistor)
5. DHT11 temperature + humidity -- a bit-banged digital protocol, written
   by hand instead of via a library
6. MQ-2 gas/smoke -- analog again, but needs a warm-up + a calibrated
   baseline instead of a fixed threshold
7. Sound detection module -- combines an analog envelope reading with a
   digital threshold trigger
8. WiFi scan -- the ESP32's onboard radio, passively listening instead of
   reading a dedicated part
9. Bluetooth (BLE) scan -- the ESP32's other onboard radio; contrasts with
   robo-car's classic-Bluetooth speaker pairing
10. Camera -- capture a still image on the Pi (the OV5647 module robo-car's
    "See" phase will use)
11. Speaker -- play a tone through the Pi's paired Bluetooth speaker (the
    output side of robo-car's "Hear + Talk" phase)
