# hello-raspberrypi

Progressive learning series for Raspberry Pi (C++). Each step is a standalone file
that builds on the previous one, adding exactly one new concept.

Board on hand: Raspberry Pi Zero W (2017) -- single-core ARMv6, 512MB RAM. That CPU
is too old for Raspberry Pi OS Bookworm, so it runs the Legacy (Bullseye-based) image.

## Setup

```sh
./flash.sh raspios-lite-armhf.img.xz /dev/disk4 "MyWiFi" "wifi-password"   # write SD card + headless config
make provision HOST=pi@pi-zero.local   # sets hostname, installs base packages, enables camera/I2C/SPI
```

`flash.sh` writes the OS image and drops in `ssh` + `userconf.txt` + `wpa_supplicant.conf`
so the Pi boots headless and joins your WiFi with no monitor/keyboard needed. `provision.sh`
runs its commands over SSH -- plain and safe to re-run, nothing more to install locally.
