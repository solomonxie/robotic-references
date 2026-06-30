#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_09_voltage_regulation.py
#
# Step 9: why robo-car uses a buck converter (UBEC), not a plain resistor
# or a linear regulator, to turn its ~7.4V pack into a clean 5V logic rail
# (see ../robo-car/POWER.md). A linear regulator forces Vout by burning the
# extra voltage off as heat -- it draws as much current as the load needs,
# at the *higher* input voltage, so the wasted power is (Vin-Vout)*Iout
# (step 4's P=IV again). A switching/buck regulator instead converts the
# extra voltage into extra current, so input current drops roughly in
# proportion -- most of the input power reaches the output instead of
# becoming heat.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Stepping Vin down to Vout: linear regulator vs switching (buck) regulator.\n")

    vin = ask("Input voltage (V) [default 7.4, robo-car's 2S pack]: ", 7.4)
    vout = ask("Output voltage (V) [default 5.0, logic rail]: ", 5.0)
    iout_ma = ask("Load current (mA) [default 500, Pi + ESP32]: ", 500.0)
    efficiency = ask("Buck converter efficiency (0-1) [default 0.85]: ", 0.85)

    iout_a = iout_ma / 1000

    # Linear regulator: input current == output current (all current passes
    # through), just at Vin instead of Vout. The gap becomes heat.
    linear_waste_w = (vin - vout) * iout_a
    linear_in_w = vin * iout_a

    # Buck converter: near-lossless power conversion, so input power just
    # covers output power plus the inefficiency.
    out_w = vout * iout_a
    buck_in_w = out_w / efficiency
    buck_waste_w = buck_in_w - out_w
    buck_in_a = buck_in_w / vin

    print(f"\nOutput power needed: {out_w:.4g} W")
    print(f"\nLinear regulator:")
    print(f"  input current = output current = {iout_a:.4g} A (unchanged)")
    print(f"  power wasted as heat = ({vin:.4g}-{vout:.4g}) * {iout_a:.4g} "
          f"= {linear_waste_w:.4g} W  <- must be dissipated, often needs a heatsink")
    print(f"\nBuck (switching) regulator (~{efficiency*100:.0f}% efficient):")
    print(f"  input current ~= {buck_in_a:.4g} A (less than {iout_a:.4g} A -- "
          f"traded for the voltage step-down instead of burning it as heat)")
    print(f"  power wasted as heat ~= {buck_waste_w:.4g} W")
