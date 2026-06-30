#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_08_battery_runtime.py
#
# Step 8: applies step 7's NSxMP pack math to robo-car's own 18650 pack (see
# ../robo-car/POWER.md) -- a 2S1P holder, so pack capacity is just ONE
# cell's mAh, not doubled (see step 7's note). Runtime = pack capacity
# (mAh) / load current (mA) -- the same "how long until empty" math for
# any battery-powered circuit.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Battery runtime = pack capacity (mAh) / load current (mA)\n")
    print("robo-car's holder is 2S1P (2 cells in series, 1 string) -- "
          "capacity is one cell's, not doubled.\n")

    cell_mah = ask("Capacity per cell (mAh) [default 3000, typical 18650]: ", 3000.0)
    parallel = ask("Strings in parallel (P) [default 1, robo-car's holder]: ", 1.0)
    load_ma = ask("Total load current (mA) [default 800, e.g. 4 DC motors + Pi]: ", 800.0)

    pack_mah = cell_mah * parallel
    hours = pack_mah / load_ma

    print(f"\nPack capacity = {cell_mah:.4g} mAh * {parallel:.0g} parallel string(s) "
          f"= {pack_mah:.4g} mAh")
    print(f"Estimated runtime at {load_ma:.4g} mA draw = {hours:.4g} hours "
          f"({hours * 60:.0f} minutes)")
    print("\nReal runtime is usually shorter: this ignores voltage sag under "
          "load (step 10) and the ~20% capacity margin manufacturers build in.")
