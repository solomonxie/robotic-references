#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_10_voltage_sag_grounding.py
#
# Step 10: why robo-car splits into two power rails (motors straight off
# the battery, logic through its own regulator) but still ties every GND
# together (see ../robo-car/POWER.md's diagram). A real battery has some
# internal resistance, so its terminal voltage sags under load: V_terminal
# = V_nominal - I * R_internal (Ohm's Law again, step 1, just applied
# inside the battery). A motor stall can spike current enough to sag the
# rail below what logic needs -- so logic gets its own regulated rail
# instead of sharing the motors' path. That still leaves one requirement:
# every board's GND must be tied to the same reference, or a signal like
# "3.3V = logic high" means different things to boards that disagree on
# where 0V is.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Battery terminal voltage sags under load: V = V_nominal - I * R_internal\n")

    v_nominal = ask("Pack nominal voltage (V) [default 7.4]: ", 7.4)
    r_internal_mohm = ask("Pack internal resistance (milliohms) [default 100]: ", 100.0)
    idle_ma = ask("Idle current -- logic only (mA) [default 400]: ", 400.0)
    stall_ma = ask("Motor stall current spike (mA) [default 6000, ~4 motors stalling]: ", 6000.0)

    r_internal = r_internal_mohm / 1000

    v_idle = v_nominal - (idle_ma / 1000) * r_internal
    v_stall = v_nominal - (stall_ma / 1000) * r_internal

    print(f"\nAt idle ({idle_ma:.4g} mA):  terminal voltage = {v_idle:.4g} V")
    print(f"At motor stall ({stall_ma:.4g} mA): terminal voltage = {v_stall:.4g} V "
          f"({v_nominal - v_stall:.4g} V sag)")

    print("\nIf logic shared this same unregulated rail, that sag could drop "
          "below the Pi/ESP32's minimum input voltage during a stall --")
    print("hence the separate regulated 5V rail (step 9) for logic.")
    print("\nBut a separate rail only isolates the *voltage* -- every board's "
          "GND (battery, both regulation paths, Pi, ESP32) still needs to be")
    print("tied to one common reference, or a '3.3V = HIGH' signal from one "
          "board reads as a different level to a board with a different 0V.")
