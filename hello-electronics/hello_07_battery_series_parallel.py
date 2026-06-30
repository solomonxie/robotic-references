#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_07_battery_series_parallel.py
#
# Step 7: batteries wired series-vs-parallel mirror steps 2+3's resistor
# circuits, but for a source instead of a load. Cells in *series* add their
# voltages (same current flows through the whole string, like series
# resistors) -- the string's capacity stays at one cell's rating. Cells in
# *parallel* keep one cell's voltage but add their capacities (each string
# contributes current independently, like parallel resistors). Packs are
# described as "NSxMP" -- N cells in series per string, M strings in
# parallel. robo-car's 18650 holder is 2S1P: 2 cells in series, only one
# string, per ../robo-car/POWER.md.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Battery pack voltage/capacity from an NSxMP configuration.\n")
    print("robo-car's pack is 2S1P: try the defaults below.\n")

    cell_v = ask("Nominal voltage per cell (V) [default 3.7, typical Li-ion]: ", 3.7)
    cell_mah = ask("Capacity per cell (mAh) [default 3000]: ", 3000.0)
    series = ask("Cells in series (S) [default 2]: ", 2.0)
    parallel = ask("Strings in parallel (P) [default 1]: ", 1.0)

    pack_v = cell_v * series
    pack_mah = cell_mah * parallel

    print(f"\n{series:.0g}S{parallel:.0g}P pack:")
    print(f"  Pack voltage  = {cell_v:.4g} V * {series:.0g} series = {pack_v:.4g} V")
    print(f"  Pack capacity = {cell_mah:.4g} mAh * {parallel:.0g} parallel = {pack_mah:.4g} mAh")
    print(f"\nNote: capacity did NOT multiply by the series count -- only one "
          f"cell's worth of charge flows through a series string at a time.")
