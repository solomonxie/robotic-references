#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_05_led_resistor_calc.py
#
# Step 5: applies steps 1+4 to a real component from the kit -- picking a
# current-limiting resistor for an LED, the calculation behind
# hello-esp32/hello_04_resistor_led.ino. An LED has a roughly fixed
# "forward voltage" drop (not ohms-law linear like a resistor); anything
# above that drop across it would let current run away and burn it out, so
# a series resistor absorbs the rest of the supply voltage and limits
# current to a safe value.

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


# Typical forward voltages by LED color (silicon diode physics, not ohms).
FORWARD_VOLTAGE = {
    "red": 2.0,
    "yellow": 2.1,
    "green": 2.2,
    "blue": 3.2,
    "white": 3.2,
}

if __name__ == "__main__":
    print("LED series resistor: R = (supply V - LED forward V) / desired current\n")
    print(f"Typical forward voltages: {FORWARD_VOLTAGE}\n")

    supply = ask("Supply voltage (V) [default 5, e.g. ESP32/Pi GPIO]: ", 5.0)
    color = input("LED color [default red]: ").strip().lower() or "red"
    vf = FORWARD_VOLTAGE.get(color, 2.0)
    target_ma = ask("Desired current in mA [default 15, safe for a 5mm LED]: ", 15.0)

    drop = supply - vf
    if drop <= 0:
        raise SystemExit(f"Supply ({supply}V) must exceed the LED's forward "
                          f"voltage ({vf}V) or it won't light at all.")

    target_a = target_ma / 1000
    resistance = drop / target_a
    power = target_a * target_a * resistance

    print(f"\n{color} LED forward voltage ~= {vf} V")
    print(f"Resistor must drop {drop:.4g} V at {target_ma:.4g} mA")
    print(f"R = {resistance:.4g} ohms -> round up to the nearest standard "
          f"value (e.g. 220, 330, 470, 1000)")
    print(f"Resistor power dissipation = {power:.4g} W (a 1/4W resistor is fine here)")
