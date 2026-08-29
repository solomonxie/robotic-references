# mycar assembly (Phase 1, steps 1-4: physical + wiring, no code)

Steps 5-9 (firmware, then the Pi dashboard) live flat in this same folder
once these are done -- see [README.md](./README.md#phased-plan).

**No screw-by-screw mechanical diagram found.** The kit's manual page and its
linked docs (a motor wiring-test PDF for a *different* driver board, not
yours) don't include exploded assembly views. Check the physical box for a
separate printed fold-out diagram -- these kits usually ship one distinct
from the manual card. Steps below are what's confirmed (wheel arrangement,
electrical wiring); general mechanical assembly (which screw in which hole)
isn't -- dry-fit before final tightening.

## Step 1: Mount motors + wheels on the chassis

1. Screw all 4 TT motors onto the aluminum chassis mounts (there are 2 chassis
   plates -- a bending base plate and a top plate -- assemble those per the
   kit's own quick-start diagram first if they aren't joined yet).
2. **Wheel arrangement is confirmed from the kit's manual** -- the kit ships
   2x "wheel A" and 2x "wheel B" (visibly different roller angle):
   ```
        front
      A       B
   left         right
      B       A
        back
   ```
   Front-left and rear-right get the **A** wheels; front-right and rear-left
   get the **B** wheels. Getting this backward doesn't break forward/backward
   driving, only strafing -- but get it right now, it's not worth debugging
   later.
3. Each wheel connects to its motor through a **TT joint** (a small coupler
   between the motor's D-shaft and the wheel hub) -- don't try to press the
   wheel directly onto the motor shaft.
4. The kit also includes 4 **coding discs** (for the motor's speed encoder).
   Not needed for Phase 1 (no encoder feedback yet) -- set them aside safely
   rather than mounting them now, unless the gearbox assembly requires the
   disc in place mechanically (check the kit's own diagram if unsure).
5. Tighten everything per the kit's screws -- don't power anything yet.
6. Confirm nothing binds by spinning each wheel by hand.

## Step 2: Wire power

1. Wire the battery box -> an on/off switch -> both L298N boards' power
   input terminals (+ and -). Don't connect a battery yet.
2. The kit's battery box takes **18650 Li-ion cells** (not included). Count
   its cell slots to know what you're working with: 1 cell = ~3.7V nominal
   (~4.2V full charge), 2 cells in series = ~7.4V nominal (~8.4V full).
   The motors' own rated range is **3-6V** (printed on the motor casings).
   The kit manual's "3-7.4V" figure is the *driver system's* supply input
   range, not the motor's -- a 2S (7.4V) box feeds the L298N, whose own ~2V
   drop brings what reaches the motor back down near 5.4V, inside the
   motor's real 3-6V range.
3. With the battery connected and switch on, **measure the L298N's output
   terminals with a multimeter before wiring any motor** -- confirm it's in
   the safe range regardless (good practice even when the numbers should
   work out). If it's too high, don't proceed to Step 3 yet (an in-line buck
   converter or fewer cells is safer than risking the motors).
4. Keep the Pi/ESP32 on a separate 5V supply, not sharing the motor battery
   directly -- motor stall current can brown out logic boards on the same
   rail (see README's power note).

## Step 3: Wire motors to the L298N boards

Each L298N has 2 independent channels (OUT1/OUT2 and OUT3/OUT4) -- mecanum
needs all 4 motors independent, so:

- L298N #1: front-right motor (**M1**) -> OUT1/OUT2, front-left motor (**M2**) -> OUT3/OUT4
- L298N #2: left-rear motor (**M3**) -> OUT1/OUT2, right-rear motor (**M4**) -> OUT3/OUT4

(M1-M4 naming matches the kit manufacturer's own convention, from their
motor wiring-test reference doc -- kept for consistency with any other kit
docs you find.)

Don't parallel any two motors onto the same channel -- that's skid-steering
wiring, and it won't let the firmware mix them independently for strafing.

## Step 4: Wire the L298N control pins to the ESP32

Each L298N needs 3 logic pins per channel (IN1, IN2, ENA for channel A;
IN3, IN4, ENB for channel B) -- 6 pins per board, 12 total across both
boards. Any ESP32 GPIO works for these except the input-only pins
(GPIO34-39) and the strapping pins (GPIO0, 2, 12, 15) -- avoid those, use
any other free GPIOs. Wire each L298N's GND to the ESP32's GND (shared
reference, even though motor power is separate from the ESP32's own supply).

Write down your exact pin choices here once wired -- the firmware will need them:

| Motor | IN1 | IN2 | EN (PWM) |
|---|---|---|---|
| M1 - Front-right | | | |
| M2 - Front-left | | | |
| M3 - Rear-left | | | |
| M4 - Rear-right | | | |

Once this is done and double-checked, move to step 5 (spin one wheel, no
code complexity yet). Test one motor at a time before all four together --
this matches how the kit manufacturer's own QC test brings motors up
(each one alone, a few seconds forward then stop, in sequence).
