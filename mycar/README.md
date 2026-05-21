# mycar

An autonomous home-patrol robot: drives around the house on its own checking on
things like a patrol officer, sees, hears, talks, avoids obstacles, and can be
watched or manually driven from a browser dashboard on the home WiFi.

Implementation is unverified against real hardware -- the car isn't assembled yet
(see [SHOPPING_LIST.md](./SHOPPING_LIST.md)), so each step is compiled/syntax-checked
but not run end-to-end until you have wheels turning to test against.

## Hardware inventory (on hand)

Confirmed from the kit box lid and the chassis kit's product listing (Aug 2026).

- 3x ESP32 Dev Kit boards
- 5x L298N motor driver modules
- **Miuzei MA49 "Electronic Fun Kit"** -- power supply module, breadboard,
  jumper/Dupont wires, pin headers, 2x potentiometer, LEDs + RGB LEDs, 4N35
  optocoupler, 74HC595 shift register, NPN transistors, **photoresistors,
  thermistors**, active/passive buzzers, buttons/switches, capacitors, diodes,
  full resistor assortment. **Confirmed NOT in this kit**: no ultrasonic
  sensor, no servo, no IR sensor.
- HC-SR04 ultrasonic sensor -- separate from the kit above, confirmed on hand
- **DWWTKL Mecanum Wheel Car Kit** -- aluminum chassis (186x161x91mm, 1500g
  load capacity), 4x **mecanum wheels** (68mm dia., angled rollers --
  omnidirectional, and *handed*: they must be mounted in the correct
  front/rear-left/right orientation or the car won't strafe), 4x
  **independent** TT gear motors (1:120 ratio, **3-6V rated** -- this is low;
  see the power note below), motors include **speed encoders** (not used in
  Phase 1, useful later for odometry/closed-loop speed), battery box
  **included but no battery**, mounting screws
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

**Motor driver pairing**: mecanum wheels need all 4 motors controlled
**independently** (not left/right pairs -- that's skid-steering, which only
works with plain wheels). 2 of the 5 L298N boards give exactly 4 independent
channels (2 boards x 2 channels each), one per wheel; the firmware mixes them
for forward/strafe/rotate. 3 L298N boards stay spare.

**Motor voltage**: the TT motors are rated 3-6V, but the L298N's own H-bridge
drops ~2V between input and output -- so a 7.4V (2S) battery could land in a
safe ~5.5V at the motor, which is a common trick, but this must be verified
with a multimeter before running at full duty cycle, not assumed.

**LLM**: OpenAI API over the Pi's WiFi connection (your API key). This is a
metered cloud service -- expect a small ongoing cost per patrol/conversation,
not a one-time hardware purchase.

## Phased plan

1. **Drive** -- broken into its own sub-steps in [ASSEMBLY.md](./ASSEMBLY.md)
   and [esp32/](./esp32) / [pi/](./pi), physical assembly first, dashboard
   last:
   1. Mount motors + mecanum wheels on the chassis (ASSEMBLY.md step 1 --
      wheel orientation matters, read this before attaching anything)
   2. Wire battery -> switch -> both L298N boards; verify motor voltage with
      a multimeter (ASSEMBLY.md step 2)
   3. Wire each of the 4 motors to its own L298N channel (ASSEMBLY.md step 3)
   4. Mount the ESP32, wire the 2 L298N boards' 12 control pins to it
      (ASSEMBLY.md step 4)
   5. First firmware: spin one wheel only, no serial/network, just prove the
      ESP32->L298N->motor chain works
   6. All 4 wheels forward together, hardcoded -- catches any wheel wired
      backward before trusting the mixing math
   7. Mecanum mixing test (strafe/rotate) -- if this doesn't move as
      expected, a wheel's likely mounted in the wrong handed position
   8. Serial command control, typed into the Arduino IDE's Serial Monitor
      over USB -- no Pi involved yet
   9. Swap to the Pi: wire GPIO14/15, run the web dashboard, drive from a
      browser

   **Status**: only the assembly steps (1-4) are written so far, in
   ASSEMBLY.md. [`esp32/step1_drive.ino`](./esp32/step1_drive.ino) and
   [`pi/step1_dashboard/`](./pi/step1_dashboard) were an earlier draft built
   before the wheels were confirmed mecanum -- their skid-steering logic
   (left/right motor pairs) is wrong for this chassis and needs a mecanum
   mixing rewrite (steps 5-9 above) before use. Left in place for reference,
   not for deploying as-is.
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
- **Pi Zero W's Bluetooth competes with GPIO14/15 for the good UART** -- BT (needed
  for the speaker) claims the PL011 UART by default, pushing GPIO14/15 onto the
  less stable mini-UART. Add `core_freq=250` to `/boot/config.txt` once both are
  in use, or the serial link to the ESP32 can get flaky.
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
- **Battery not yet chosen** -- the chassis kit's battery box came without one.
  Needs to land the motors in their 3-6V range (through the L298N's ~2V drop --
  see the power note above); confirm cell count/chemistry before wiring.
- **Mecanum wheel handedness unconfirmed** -- check for L/R or A/B markings on
  the wheel hubs before mounting (ASSEMBLY.md step 1); mounting them in the
  wrong front/rear-left/right positions means the car drives fine forward/back
  but can't strafe correctly.

## Missing / to buy

See [SHOPPING_LIST.md](./SHOPPING_LIST.md).
