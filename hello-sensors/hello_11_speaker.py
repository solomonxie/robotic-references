#!/usr/bin/env python3
# Run ON THE PI, over ssh. Needs a Bluetooth speaker already paired and set
# as the default audio output (bluetoothctl: scan/pair/connect/trust, then
# raspi-config's System Options -> Audio, or `pactl set-default-sink`).
#   venv/bin/python hello_11_speaker.py
#
# Step 11: play a tone through the speaker -- the output side of robo-car's
# "Hear + Talk" phase (its OpenAI TTS audio plays through this same path).
# Stdlib only: builds a WAV in memory and hands it to `aplay` (ALSA's CLI
# player, present on Raspberry Pi OS by default) rather than pulling in an
# audio library just for a beep.

import io
import math
import struct
import subprocess
import wave

SAMPLE_RATE = 44100
FREQ_HZ = 440      # A4, standard tuning reference tone
DURATION_S = 1.0

buf = io.BytesIO()
with wave.open(buf, "wb") as wav:
    wav.setnchannels(1)
    wav.setsampwidth(2)  # 16-bit
    wav.setframerate(SAMPLE_RATE)
    n_samples = int(SAMPLE_RATE * DURATION_S)
    frames = b"".join(
        struct.pack("<h", int(32767 * math.sin(2 * math.pi * FREQ_HZ * i / SAMPLE_RATE)))
        for i in range(n_samples)
    )
    wav.writeframes(frames)

subprocess.run(["aplay"], input=buf.getvalue(), check=True)
print(f"Played a {FREQ_HZ}Hz tone")
