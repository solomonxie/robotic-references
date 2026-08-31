#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_02_series_circuit.py
#
# Step 2: full copy of step 1's Ohm's Law solver, plus series circuits --
# components chained end to end so the same current flows through all of
# them. Total resistance is just the sum; the supply voltage splits across
# each resistor in proportion to its resistance (a "voltage divider").

def solve(voltage=None, current=None, resistance=None):
    known = [x is not None for x in (voltage, current, resistance)]
    if sum(known) != 2:
        raise ValueError("give exactly two of voltage/current/resistance")
    if voltage is None:
        voltage = current * resistance
    elif current is None:
        current = voltage / resistance
    else:
        resistance = voltage / current
    return voltage, current, resistance


def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Series circuit: resistors chained end to end, same current throughout.\n")

    supply = ask("Supply voltage (V) [default 5]: ", 5.0)
    raw = input("Resistor values in ohms, comma-separated [default 220,330,1000]: ")
    resistors = [float(x) for x in raw.split(",")] if raw.strip() else [220.0, 330.0, 1000.0]

    total_r = sum(resistors)
    _, current, _ = solve(voltage=supply, resistance=total_r)

    print(f"\nTotal resistance = {total_r:.4g} ohms")
    print(f"Circuit current   = {current:.4g} amps (same through every resistor)")
    print("\nVoltage drop across each resistor:")
    for idx, r in enumerate(resistors, start=1):
        drop = current * r
        print(f"  R{idx} = {r:>7.4g} ohms -> {drop:.4g} V")
    print(f"\nDrops sum to {sum(current * r for r in resistors):.4g} V "
          f"(should equal the {supply:.4g} V supply)")
