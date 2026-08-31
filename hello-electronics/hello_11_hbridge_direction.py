#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_11_hbridge_direction.py
#
# Step 11: how a motor driver chip like the L298N reverses a DC motor's
# direction. A single switch/transistor can only connect a motor to power or
# not -- it can't reverse which way current flows through it, which is what
# reverses spin direction. An H-bridge wires 4 switches around the motor (in
# the shape of an "H", motor as the crossbar) so either terminal can be tied
# to +V or GND: diagonal pairs on give forward or reverse, same-side pairs
# on short the motor's terminals together (braking), all off leaves it
# floating (coasting). The L298N packages one H-bridge per channel behind 2
# logic pins (IN1/IN2) plus an EN pin that PWM-gates the whole channel for
# speed -- this is exactly IN1/IN2/EN in robo-car's step1_drive.ino.

def h_bridge_state(in1, in2, duty, v_supply, max_duty=255):
    if duty == 0:
        return "coasting -- EN low, both outputs high-impedance, motor free-spins down"
    if in1 == in2:
        return "braking -- IN1==IN2, both motor terminals shorted together through the bridge"
    direction = "forward" if in1 and not in2 else "reverse"
    # PWM at the EN pin switches the channel on/off duty_cycle fraction of
    # each cycle, so the motor sees v_supply that fraction of the time --
    # its own inductance/inertia smooths that into an effective average
    # voltage (same duty-cycle idea as hello-esp32's hello_06_pwm_brightness,
    # just averaging voltage instead of light output).
    v_avg = v_supply * duty / max_duty
    return f"{direction} -- PWM duty {duty}/{max_duty}, effective avg motor voltage ~{v_avg:.3g}V"


def ask(prompt, default):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return raw.strip() if raw.strip() else default


if __name__ == "__main__":
    print("L298N H-bridge: IN1/IN2 pick direction, EN's PWM duty picks speed.\n")

    in1 = ask("IN1 (1/0) [default 1]: ", "1") == "1"
    in2 = ask("IN2 (1/0) [default 0]: ", "0") == "1"
    duty = int(ask("EN PWM duty, 0-255 [default 200]: ", "200"))
    v_supply = float(ask("Supply voltage (V) [default 7.4, robo-car's 2S pack]: ", "7.4"))

    print(f"\nIN1={int(in1)} IN2={int(in2)} duty={duty} -> {h_bridge_state(in1, in2, duty, v_supply)}")
