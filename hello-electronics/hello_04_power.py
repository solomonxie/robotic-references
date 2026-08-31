#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_04_power.py
#
# Step 4: power -- how fast a component turns electrical energy into heat
# or work, in watts. Three equivalent formulas (P=IV, P=I^2R, P=V^2/R) fall
# straight out of Ohm's Law; which one to use just depends on which two
# values you already know. This matters for picking a resistor's *wattage*
# rating, not just its ohms -- too low and it overheats/burns out.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Power (watts) = energy converted to heat/work per second.\n")
    print("Enter any two of voltage/current/resistance; power is derived from them.\n")

    v = ask("Voltage (V), blank to skip: ", None)
    i = ask("Current (A), blank to skip: ", None)
    r = ask("Resistance (ohms), blank to skip: ", None)

    if v is not None and i is not None:
        p = v * i
        formula = "P = V * I"
    elif i is not None and r is not None:
        p = i * i * r
        formula = "P = I^2 * R"
    elif v is not None and r is not None:
        p = v * v / r
        formula = "P = V^2 / R"
    else:
        raise SystemExit("give at least two of voltage/current/resistance")

    print(f"\n{formula} = {p:.4g} watts")

    # A common Electronic Fun Kit resistor is rated 1/4W (0.25W). Warn if
    # this circuit would exceed a typical rating.
    for rating in (0.125, 0.25, 0.5):
        headroom = "OK" if p < rating * 0.5 else ("tight" if p < rating else "EXCEEDS rating")
        print(f"  vs a {rating}W resistor: {headroom}")
