#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_01_ohms_law.py
#
# Step 1: Voltage, current, resistance -- and the one equation that relates
# them (Ohm's Law: V = I * R). Voltage (V, volts) is the electrical "push"
# between two points; current (I, amps) is how much charge flows per
# second; resistance (R, ohms) is how much a component resists that flow.
# Enter any two and this computes the third.

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


def ask(prompt, default):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Ohm's Law: V = I * R")
    print("Leave a field blank (just press Enter) to solve for it.\n")

    v_raw = ask("Voltage (V), blank to solve: ", None)
    i_raw = ask("Current (A), blank to solve: ", None)
    r_raw = ask("Resistance (ohms), blank to solve: ", None)

    v, i, r = solve(v_raw, i_raw, r_raw)
    print(f"\nV = {v:.4g} volts")
    print(f"I = {i:.4g} amps")
    print(f"R = {r:.4g} ohms")
