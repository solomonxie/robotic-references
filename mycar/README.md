# mycar

An autonomous home-patrol robot: drives around the house on its own checking on
things like a patrol officer, sees, hears, talks, avoids obstacles, and can be
watched or manually driven from a browser dashboard on the home WiFi. This is a
plan/design document -- no car-control code exists yet.

## Hardware inventory (on hand)

- 3x ESP32 Dev Kit boards
- 5x L298N motor driver modules
- Electronic Fun Kit -- power supply module, jumper/dupont wires, pin headers,
  breadboard, resistors, LEDs, HC-SR04 ultrasonic sensor (confirmed on hand;
  other sensors in the kit to be identified/added as needed, see [hello-esp32](../hello-esp32))
- Car chassis kit, 4x DC motors, wheels, battery + battery connections
- Raspberry Pi Zero W (2017) -- see [hello-raspberrypi](../hello-raspberrypi) for OS setup
- NanoPi R2S -- optional, see [hello-nanopi](../hello-nanopi)
- Bluetooth speaker

## Architecture

```
                         home WiFi
                             |
     browser dashboard <-----+-----> Raspberry Pi Zero W ("the brain")
     (manual control,                 - hosts the dashboard (Flask/FastAPI)
      live status)                     - talks to OpenAI API (LLM, vision, STT, TTS)
                                        - plays audio to the Bluetooth speaker
                                        - patrol state machine (auto/manual mode)
                                              |
                                       UART serial (GPIO14/15, 3.3V both sides
                                       -- no level shifter needed)
                                              |
                                        ESP32 ("motor + sensor controller")
                                        - reads HC-SR04 ultrasonic distance
                                        - drives 2x L298N -> 4 DC motors
                                        - reports sensor data / ack's commands
```

**Board roles**: the Pi Zero W is the only board with enough RAM/OS to run a web
server, call cloud APIs, and juggle camera/audio -- so it's the brain. One ESP32
is a dedicated real-time motor/sensor controller (fast obstacle response
shouldn't have to wait on a Python web server or a network call). The other 2
ESP32s and the NanoPi R2S are spare for later add-ons, not part of the MVP.

**Pi <-> ESP32 link**: plain UART serial over a few GPIO wires, not WiFi. It's
wired anyway (same chassis), avoids WiFi latency/dropouts for motor commands,
and both boards run 3.3V logic so no level shifter is needed.

**Motor driver pairing**: 4 motors, 2 of the 5 L298N boards -- one channel per
motor gives independent control of all 4 wheels (skid-steering: left-side
motors always commanded together, right-side together). 3 L298N boards stay
spare.

**LLM**: OpenAI API over the Pi's WiFi connection (your API key). This is a
metered cloud service -- expect a small ongoing cost per patrol/conversation,
not a one-time hardware purchase.

## Phased plan

1. **Drive** -- ESP32 + 2x L298N move all 4 wheels; Pi dashboard has manual
   directional controls; verify wiring/direction before anything autonomous.
2. **Sense** -- add the HC-SR04 ultrasonic sensor to the ESP32; a simple
   auto-patrol state machine (drive forward, stop/turn near obstacles).
3. **Think** -- Pi calls the OpenAI API to turn patrol telemetry into a log/
   summary ("commentary" on what it's been doing), surfaced on the dashboard.
   Text-only at this stage -- no camera/mic yet.
4. **See** -- add a camera (see shopping list); live video on the dashboard;
   optionally send snapshots to an OpenAI vision model during patrol.
5. **Hear + Talk** -- add a USB microphone (see shopping list); speech-to-text
   (OpenAI Whisper API) -> LLM -> text-to-speech (OpenAI TTS API) -> play
   through the Bluetooth speaker you already have. Two-way voice interaction.
6. **Polish** -- scheduled patrol runs, dashboard alerts/notifications,
   persistent logs, possibly the NanoPi R2S as a dedicated router/AP for the
   car's own network segment.

## Open assumptions -- confirm or correct

- **Pi<->ESP32 link is Serial, not WiFi.** Switch this if you'd rather keep the
  boards physically separable.
- **Power**: unconfirmed whether the Electronic Fun Kit's power supply module
  can supply both motor voltage (7.4-12V typical) *and* a clean, separate 5V
  rail for the Pi. Motor stall current can brown out a Pi sharing the same
  supply -- if there's no separate regulated 5V source already, add the buck
  converter in the shopping list.
- **NanoPi R2S's job is undecided** -- it has no WiFi/camera/GPIO header, so
  it can't replace the Pi; it's optional infrastructure for later (e.g.
  dedicated WiFi AP or VPN/reverse-proxy for remote dashboard access).
- **MicroSD card for the Pi Zero** -- not in your listed inventory; flagged in
  the shopping list in case it's still needed.

## Missing / to buy

See [SHOPPING_LIST.md](./SHOPPING_LIST.md).
