#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_06_capacitor_rc.py
#
# Step 6: capacitors -- store charge (in farads) rather than resist current.
# Wired in series with a resistor, a capacitor charges/discharges
# exponentially rather than instantly; the time constant tau = R * C is how
# long it takes to reach ~63% of the way there. Multiples of tau are the
# rule of thumb for "settled": ~5*tau is considered fully charged/discharged.

import math

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("RC charging: V(t) = V_supply * (1 - e^(-t / tau)), tau = R * C\n")

    resistance = ask("Resistance (ohms) [default 1000]: ", 1000.0)
    capacitance_uf = ask("Capacitance (microfarads) [default 100]: ", 100.0)
    supply = ask("Supply voltage (V) [default 5]: ", 5.0)

    capacitance_f = capacitance_uf * 1e-6
    tau = resistance * capacitance_f

    print(f"\ntau = R * C = {resistance:.4g} ohms * {capacitance_uf:.4g} uF "
          f"= {tau * 1000:.4g} ms")
    print("\nCharging curve (fraction of supply voltage reached):")
    for n in range(1, 6):
        t = n * tau
        v = supply * (1 - math.exp(-t / tau))
        pct = 100 * v / supply
        print(f"  t = {n}*tau ({t * 1000:6.4g} ms) -> {v:.4g} V ({pct:.0f}%)")
    print("\nAfter ~5*tau the capacitor is considered fully charged.")
