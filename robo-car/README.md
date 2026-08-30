# robo-car

An autonomous home-patrol robot: drives around the house on its own checking on
things like a patrol officer, sees, hears, talks, avoids obstacles, and can be
watched or manually driven from a browser dashboard on the home WiFi.

Implementation is unverified against real hardware -- the car isn't assembled yet
(see [SHOPPING_LIST.md](./SHOPPING_LIST.md)), so each step is compiled/syntax-checked
but not run end-to-end until you have wheels turning to test against.

## Hardware inventory (on hand)

Confirmed from the kit box lid, the chassis kit's official product manual, and
its product listing (Aug 2026).

**Boards**
- 3x ESP32 Dev Kit boards
- Raspberry Pi Zero W (2017) -- see [hello-raspberrypi](../hello-raspberrypi) for OS setup
- NanoPi R2S -- optional, see [hello-nanopi](../hello-nanopi)

**Motor driving**
- 5x L298N motor driver modules
- 2-layer aluminum chassis, 186x161x91mm, 1500g load capacity (DWWTKL Mecanum Wheel Car Kit)
- 4x mecanum wheels, 68mm dia., 2x type "A" + 2x type "B" (*handed*: A goes
  front-left + rear-right, B goes front-right + rear-left -- confirmed from
  the kit's manual diagram)
- 4x independent TT gear motors, 1:48 ratio, **3-6V rated** (printed on the
  casings -- the driver system accepts up to 7.4V input per the manual,
  dropped down by the L298N before it reaches the motor)
- 4x TT joints (motor-to-wheel couplers)
- 4x coding discs, for the motors' speed encoders (**no encoder sensor board
  included** in this kit -- open item if you want encoder feedback later)
- 18650 x2 battery holder (included with the chassis kit; cells not included
  -- see [POWER.md](./POWER.md))
- Mounting screws (assorted, from the chassis kit)

**Sensors**
- Elegoo HC-SR04 ultrasonic distance sensor (4-pin: VCC, Trig, Echo, GND)
- IR Infrared Obstacle Avoidance Sensor x2 (3-pin: VCC, GND, OUT digital,
  onboard sensitivity adjustment)

**Electronic Fun Kit** (Miuzei MA49) -- power supply module, breadboard,
jumper/Dupont wires, pin headers, 2x potentiometer, LEDs, RGB LEDs, 4N35
optocoupler, 74HC595 shift register, NPN transistors, photoresistors,
thermistors, active buzzer, passive buzzer, buttons, switches, capacitors,
diodes, full resistor assortment. **Confirmed NOT in this kit**: no servo.

**Audio**
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

**Power supply**: battery choice, motor voltage math, and how the Pi + ESP32
get a separate clean 5V rail is its own document -- see [POWER.md](./POWER.md).

**Mecanum mixing reference** (from the kit's manual, for steps 6-7's firmware
later -- transcribed from a photo, worth double-checking against the physical
page before trusting it blindly, especially the strafe rows). The manual's
table has 14 rows total; the 4 "single-axle-only turn" variants are omitted
here as less essential -- check the physical page if you end up wanting them:

| Motion | M2 Front-left (A) | M1 Front-right (B) | M3 Rear-left (B) | M4 Rear-right (A) |
|---|---|---|---|---|
| Forward | CW | CW | CW | CW |
| Backward | CCW | CCW | CCW | CCW |
| Strafe left | CCW | CW | CW | CCW |
| Strafe right | CW | CCW | CCW | CW |
| Rotate left (in place) | CCW | CW | CCW | CW |
| Rotate right (in place) | CW | CCW | CW | CCW |
| Diagonal front-left | STOP | CW | CW | STOP |
| Diagonal front-right | CW | STOP | STOP | CW |
| Diagonal rear-left | CCW | STOP | STOP | CCW |
| Diagonal rear-right | STOP | CCW | CCW | STOP |

Even if a row here is mis-transcribed, step 7 (mecanum mixing test) is
exactly the point where that would surface -- if strafing goes the wrong
way, flip that row's signs rather than trusting the table over reality.

**LLM**: OpenAI API over the Pi's WiFi connection (your API key). This is a
metered cloud service -- expect a small ongoing cost per patrol/conversation,
not a one-time hardware purchase.

## Phased plan

1. **Drive** -- broken into its own sub-steps in [ASSEMBLY.md](./ASSEMBLY.md)
   and this folder's firmware/dashboard files, physical assembly first,
   dashboard last:
   1. Mount motors + mecanum wheels on the chassis (ASSEMBLY.md step 1 --
      wheel orientation matters, read this before attaching anything)
   2. Wire power for motors + Pi + ESP32; verify all outputs with a
      multimeter (ASSEMBLY.md step 2, full design in POWER.md)
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
   ASSEMBLY.md. [`step1_drive.ino`](./step1_drive.ino) and
   [`app.py`](./app.py) were an earlier draft built before the wheels were
   confirmed mecanum -- their skid-steering logic (left/right motor pairs)
   is wrong for this chassis and needs a mecanum mixing rewrite (steps 5-9
   above) before use. Left in place for reference,
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
- **Power supply open items** -- see POWER.md's own "Open items" section
  (battery box cell count, shared-vs-separate battery for motors/logic).
- **NanoPi R2S's job is undecided** -- it has no WiFi/camera/GPIO header, so
  it can't replace the Pi; it's optional infrastructure for later (e.g.
  dedicated WiFi AP or VPN/reverse-proxy for remote dashboard access).
- **MicroSD card for the Pi Zero** -- not in your listed inventory; flagged in
  the shopping list in case it's still needed.
- **Which physical wheel is A vs B isn't confirmed yet** -- the front-left/
  rear-right = A, front-right/rear-left = B *arrangement* is confirmed from
  the manual, but matching that to your actual 4 wheels (hub markings or
  visible roller angle) still needs doing at mount time (ASSEMBLY.md step 1);
  getting it backward means the car drives fine forward/back but can't
  strafe correctly.

## Missing / to buy

See [SHOPPING_LIST.md](./SHOPPING_LIST.md).
