#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_07_battery_runtime.py
#
# Step 7: applies steps 1+4 to robo-car's own 18650 battery pack (see
# ../robo-car/POWER.md). Battery capacity is charge, not energy: milliamp-
# hours (mAh) x hours = mAh delivered. Divide capacity by the load's actual
# current draw to estimate runtime -- the same "how long until empty" math
# for any battery-powered circuit.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Battery runtime = capacity (mAh) / load current (mA)\n")

    capacity_mah = ask("Battery capacity per cell (mAh) [default 3000, typical 18650]: ", 3000.0)
    cells_parallel = ask("Cells in parallel (adds capacity) [default 2, robo-car's holder]: ", 2.0)
    load_ma = ask("Total load current (mA) [default 800, e.g. 4 DC motors + Pi]: ", 800.0)

    total_mah = capacity_mah * cells_parallel
    hours = total_mah / load_ma

    print(f"\nTotal pack capacity = {capacity_mah:.4g} mAh x {cells_parallel:.4g} "
          f"cells = {total_mah:.4g} mAh")
    print(f"Estimated runtime at {load_ma:.4g} mA draw = {hours:.4g} hours "
          f"({hours * 60:.0f} minutes)")
    print("\nReal runtime is usually shorter: this ignores voltage sag under "
          "load and the ~20% capacity margin manufacturers build in.")
