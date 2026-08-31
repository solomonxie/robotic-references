#!/usr/bin/env python3
# Run: venv/bin/python hello-electronics/hello_03_parallel_circuit.py
#
# Step 3: standalone circuit calculator, this time parallel -- components
# wired across the same two nodes, so each sees the full supply voltage but
# splits the current between them. Total resistance is always *less* than
# the smallest branch (more paths for current = less overall resistance).

def ask(prompt, default=None):
    try:
        raw = input(prompt)
    except EOFError:
        raw = ""
    return float(raw) if raw.strip() else default


if __name__ == "__main__":
    print("Parallel circuit: resistors wired across the same two nodes, "
          "same voltage across each.\n")

    supply = ask("Supply voltage (V) [default 5]: ", 5.0)
    raw = input("Resistor values in ohms, comma-separated [default 220,330,1000]: ")
    resistors = [float(x) for x in raw.split(",")] if raw.strip() else [220.0, 330.0, 1000.0]

    total_r = 1 / sum(1 / r for r in resistors)
    total_current = supply / total_r

    print(f"\nTotal resistance = {total_r:.4g} ohms "
          f"(less than the smallest branch, {min(resistors):.4g} ohms)")
    print(f"Total current draw = {total_current:.4g} amps")
    print("\nCurrent through each branch (all see the full supply voltage):")
    for idx, r in enumerate(resistors, start=1):
        branch_current = supply / r
        print(f"  R{idx} = {r:>7.4g} ohms -> {branch_current:.4g} A")
    print(f"\nBranch currents sum to "
          f"{sum(supply / r for r in resistors):.4g} A "
          f"(should equal the total {total_current:.4g} A)")
