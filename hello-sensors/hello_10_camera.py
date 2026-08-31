#!/usr/bin/env python3
# Run ON THE PI (not the dev machine, and not through arduino-cli/deploy.sh --
# steps 1-9's ESP32 has no camera interface). Platform switches to the Pi
# here because that's what actually has one (see robo-car/README.md's
# architecture: Pi = "brain", ESP32 = motor/sensor controller).
#   python3 -m venv venv && venv/bin/pip install picamera
#   venv/bin/python hello_10_camera.py
#
# Uses the legacy `picamera` library, not the newer `picamera2`/libcamera
# stack -- this matches hello-raspberrypi's Bullseye Legacy image (see its
# README), since Bookworm's libcamera stack doesn't support this Pi Zero
# W's ARMv6 CPU.
#
# Step 10: capture a single still image from the Pi Camera module (OV5647,
# see ../robo-car/SHOPPING_LIST.md's Phase 4 "See") -- the same module
# robo-car's "See" phase uses for live video + vision-model snapshots.

from picamera import PiCamera
from time import sleep

camera = PiCamera()
camera.resolution = (1024, 768)

camera.start_preview()
sleep(2)  # let auto-exposure/white-balance settle before capturing
camera.capture("snapshot.jpg")
camera.stop_preview()

print("Saved snapshot.jpg")
