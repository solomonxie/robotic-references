# mycar assembly (Phase 1, steps 1-4: physical + wiring, no code)

Steps 5-9 (firmware, then the Pi dashboard) are in [esp32/](./esp32) and
[pi/](./pi) once these are done -- see [README.md](./README.md#phased-plan).

## Step 1: Mount motors + wheels on the chassis

1. Screw all 4 TT motors onto the aluminum chassis mounts.
2. **Check each wheel hub for an L/R or A/B marking before attaching.**
   Mecanum wheels are handed -- their rollers run at a 45-degree angle, and
   opposite-position wheels (front-left & rear-right, or front-right &
   rear-left) need the same-handed wheel so the roller angles form an "X"
   viewed from above. If the hubs aren't marked, lay all 4 out and look at
   the roller direction directly; group the two that angle the same way as
   diagonal pairs.
3. Attach each wheel to its motor shaft, tightened per the kit's screws --
   don't power anything yet.
4. Confirm nothing binds by spinning each wheel by hand.

## Step 2: Wire power

1. Wire the battery box -> an on/off switch -> both L298N boards' power
   input terminals (+ and -). Don't connect a battery yet.
2. Pick a battery that keeps the motors in their 3-6V rated range once
   through the L298N's ~2V drop (e.g. a 7.4V/2S pack lands ~5.5V at the
   motor -- a common trick, but verify, don't assume).
3. With the battery connected and switch on, **measure the L298N's output
   terminals with a multimeter before wiring any motor** -- confirm it's in
   the safe range. If it's too high, don't proceed to Step 3 yet (an
   in-line buck converter or a different battery is safer than risking the
   motors).
4. Keep the Pi/ESP32 on a separate 5V supply, not sharing the motor battery
   directly -- motor stall current can brown out logic boards on the same
   rail (see README's power note).

## Step 3: Wire motors to the L298N boards

Each L298N has 2 independent channels (OUT1/OUT2 and OUT3/OUT4) -- mecanum
needs all 4 motors independent, so:

- L298N #1: front-left motor -> OUT1/OUT2, front-right motor -> OUT3/OUT4
- L298N #2: rear-left motor -> OUT1/OUT2, rear-right motor -> OUT3/OUT4

Don't parallel any two motors onto the same channel -- that's skid-steering
wiring, and it won't let the firmware mix them independently for strafing.

## Step 4: Wire the L298N control pins to the ESP32

Each L298N needs 3 logic pins per channel (IN1, IN2, ENA for channel A;
IN3, IN4, ENB for channel B) -- 6 pins per board, 12 total across both
boards. Any ESP32 GPIO works for these except the input-only pins
(GPIO34-39) and the strapping pins (GPIO0, 2, 12, 15) -- avoid those, use
any other free GPIOs. Wire each L298N's GND to the ESP32's GND (shared
reference, even though motor power is separate from the ESP32's own supply).

Write down your exact pin choices here once wired -- the firmware in
[esp32/](./esp32) will need them:

| Motor | IN1 | IN2 | EN (PWM) |
|---|---|---|---|
| Front-left | | | |
| Front-right | | | |
| Rear-left | | | |
| Rear-right | | | |

Once this is done and double-checked, move to esp32/ for step 5 (spin one
wheel, no code complexity yet).
