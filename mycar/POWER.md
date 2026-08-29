# mycar power supply design

One battery, two independent regulation paths -- motors get raw battery
voltage through the L298N's own drop, logic (Pi + ESP32) gets a clean
regulated 5V from a separate buck converter tapped off the same battery.
Never feed logic boards from the L298N's onboard 5V regulator (noisy,
current-limited, dips under motor load) or from a battery voltage sagging
under motor stall current.

```
                 2x 18650 (2S, ~7.4V nominal)
                          |
                     [on/off switch]
                          |
                       [fuse]
                          |
              +-----------+-----------+
              |                       |
        L298N #1 + #2            UBEC buck converter
        (VMS terminals,          (5V @ 3A out)
         both boards in                |
         parallel)                     +---> Pi Zero W (5V/GND pins,
              |                        |      not micro-USB)
         4x TT motors                  +---> ESP32 (5V/VIN + GND pins,
         (see 3-6V rating,             |      not USB)
          ASSEMBLY.md step 3)          +---> [bulk capacitor, 470-1000uF,
                                              smooths transient dips]

  All GNDs tied together: battery, both L298N boards, buck converter,
  Pi, ESP32 -- even though the two paths are electrically separate supply
  rails, the ESP32's control signals to the L298N need a shared ground
  reference or nothing works.
```

## Battery: 2x 18650, protected cells, in series

- The kit includes an **18650 x2 battery holder** (cells not included) --
  2S, ~7.4V nominal, ~8.4V full charge, ~6V discharged cutoff.
- **Buy protected cells** (they have a small protection PCB built into the
  cell itself, slightly longer than bare cells) -- not raw/unprotected ones.
  This is the actual safety-critical part of this whole design: an
  unprotected Li-ion cell that gets over-discharged, shorted, or
  over-charged is a real fire risk, and this build has no separate BMS.
  Protected cells cost a couple dollars more each and remove that risk at
  the cell level.
- Capacity: 2000-3500mAh per cell depending on brand/quality -- higher
  capacity costs more but directly extends patrol runtime (see the budget
  below).

## Charging

The basic series holder almost certainly has no balance-charge tap (just
2 cells wired + to - internally, with only the pack's overall + and -
exposed). **Don't try to charge it as an assembled pack.** Remove the 2
cells and charge them individually in a standalone Li-ion bay charger
(e.g. a Nitecore/XTAR-style charger) -- simplest and safest option, and
protected cells make this even lower-risk.

## Motor power path

Battery -> switch -> fuse -> both L298N boards' VMS/power terminals, wired
in parallel (both boards see the same ~7.4V). No additional regulation --
the L298N's own ~2V drop is what brings the ~7.4V down to a motor-safe
~5.4V (see README's Motor voltage note). This path carries the real current
-- use wire thick enough for it (20-22AWG for individual motor leads is
typically fine; use thicker, e.g. 18AWG, for the main trunk line from the
battery to the switch/fuse, since that carries all 4 motors' combined
current).

**Fuse sizing**: a rough starting point is 5-10A on the main battery line --
size it below what your wire gauge can safely carry, and above realistic
worst-case draw (TT gear motor stall current is commonly 1-2A each; not all
4 motors stall simultaneously in practice, but the fuse should tolerate a
brief spike without nuisance-tripping on normal starts). Treat this as a
starting point to tune, not a precise calculation -- this kit's motors
don't have a published stall-current spec to compute from exactly.

## Logic power path (Pi + ESP32)

Battery -> UBEC buck converter (5V @ 3A) -> a separate 5V rail feeding both
boards:

- **Pi Zero W**: wire the buck converter's 5V/GND output directly to the
  Pi's GPIO pins (pin 2 = 5V, pin 6 = GND), not through the micro-USB
  connector. USB connectors are a common failure point on a robot that
  vibrates/moves -- a direct soldered/header connection is far more
  reliable for something permanently installed.
- **ESP32**: same reasoning -- wire 5V/GND to the dev board's VIN + GND
  pins directly rather than through its USB port. The board's onboard
  regulator steps 5V down to 3.3V for the chip itself.
- Add a **bulk capacitor** (470-1000uF electrolytic, cheap) across the buck
  converter's output -- cheap insurance against brief voltage dips when
  WiFi or motors draw a current spike.

### Power budget (why 3A headroom matters)

| Load | Typical | Peak |
|---|---|---|
| Pi Zero W | ~150-350mA | ~450mA (WiFi active) |
| ESP32 | ~80-160mA | ~500mA (WiFi TX burst) |
| **Total logic draw** | **~300-500mA** | **~1A worst case** |

The 3A buck converter has roughly 2-3x headroom over worst-case logic draw
-- comfortable margin, and room for Phase 4/5 additions later (camera
~250mA, USB mic ~100mA; Bluetooth is already on the Pi's onboard chip, no
extra draw beyond what's counted here).

### Rough runtime estimate

A 2500mAh 18650 pair mostly powers motors intermittently plus continuous
logic draw -- expect on the order of an hour or so of active patrol time,
highly dependent on how much the motors actually run vs idle. Worth
measuring in practice once assembled rather than trusting this estimate.

## Open items

- **Not yet decided**: single shared battery (this doc's design) vs fully
  separate battery packs for motors vs logic. Shared is simpler/lighter/
  cheaper and should be fine with proper regulation; if brownouts or noise
  show up in practice despite the buck converter + bulk capacitor, a fully
  separate logic battery is the fallback.
