#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_12_l298n_current_budget.py
#
# Step 12: the L298N isn't an ideal switch -- it's built from BJTs, so each
# channel has a real voltage drop (~2V typical, see robo-car/POWER.md's
# "Motor power path": ~7.4V pack -> ~5.4V at the motor) and a real current
# ceiling per channel (2A continuous is the commonly-quoted safe rating,
# datasheet peaks go higher but not for sustained current). robo-car wires
# each side's 2 motors in parallel onto one channel, so that channel's total
# current is both motors' draw summed -- same series/parallel current rule
# as step 3, just for a driver chip's channel instead of a breadboard.

L298N_V_DROP = 2.0        # typical H-bridge drop, step 9's linear-regulator waste idea again
L298N_CHANNEL_RATED_A = 2.0  # commonly-quoted continuous rating per channel


def ask(prompt, default):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("L298N channel budget: voltage drop + current from motors sharing a channel.\n")

    v_supply = ask("Supply voltage (V) [default 7.4, robo-car's 2S pack]: ", 7.4)
    motors_per_channel = int(ask("Motors wired in parallel on this channel [default 2]: ", 2))
    per_motor_stall_a = ask("Stall current per motor (A) [default 1.5]: ", 1.5)

    v_motor = v_supply - L298N_V_DROP
    total_stall_a = motors_per_channel * per_motor_stall_a

    print(f"\nMotor terminal voltage ~= {v_supply:.3g} - {L298N_V_DROP:.3g} = {v_motor:.3g}V "
          f"(the chip's drop, not I*R -- roughly fixed regardless of current)")
    print(f"Channel current if all {motors_per_channel} motor(s) stall at once: "
          f"{motors_per_channel} * {per_motor_stall_a:.3g}A = {total_stall_a:.3g}A")

    if total_stall_a > L298N_CHANNEL_RATED_A:
        print(f"OVER the ~{L298N_CHANNEL_RATED_A:.3g}A per-channel rating -- risks overheating/thermal "
              f"shutdown; split the motors across separate channels/boards instead of paralleling them.")
    else:
        print(f"Within the ~{L298N_CHANNEL_RATED_A:.3g}A per-channel rating.")
