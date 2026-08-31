# hello-electronics

Progressive learning series for electronics fundamentals -- voltage, current,
resistance, power, capacitors -- as small interactive Python calculators
rather than static notes, so each concept is something you can plug real
numbers into and see the result. Each step is a standalone script that
prompts for a couple of values and computes the rest; later steps apply the
earlier math to parts actually in this repo's kit (an LED + resistor,
robo-car's 18650 battery pack) instead of staying abstract.

Run with the repo's existing `venv/` from the repo root:

```sh
venv/bin/python hello-electronics/hello_01_ohms_law.py
```

## Phase plan

1. Ohm's Law -- voltage, current, resistance, and how the three relate
2. Series circuits -- same current, voltage divides
3. Parallel circuits -- same voltage, current divides
4. Power -- P=IV / P=I²R / P=V²/R, and resistor wattage ratings
5. LED current-limiting resistor -- applies steps 1+4 to a real component
   (the calc behind [hello-esp32](../hello-esp32)'s resistor+LED step)
6. Capacitors -- RC charge/discharge time constant
7. Battery cells in series vs parallel -- mirrors steps 2+3, but for a
   source: voltage adds in series, capacity adds in parallel
8. Battery runtime -- applies steps 4+7 to
   [robo-car](../robo-car)'s 2S1P 18650 pack (see its [POWER.md](../robo-car/POWER.md))
9. Voltage regulation -- linear vs switching (buck) regulators, why
   robo-car's logic rail uses a buck converter instead of just eating the
   drop as heat
10. Voltage sag under load + shared ground reference -- why robo-car
    splits into two power rails but still ties every board's GND together
