# hello-nanopi

Setup for the NanoPi R2S (Rockchip RK3328, dual-core ARM Cortex-A53). Unlike
hello-esp32/hello-raspberrypi, there's no progressive lesson series here: the R2S
is built as a router/firewall box -- 2x Gigabit Ethernet, no WiFi radio, no CSI
camera port, no hobbyist GPIO header -- so there's no breadboard/sensor work to do
on it directly. Its role in [mycar](../mycar) is still open (optional network node);
this folder just gets it flashed and provisioned so it's ready when needed.

Runs Armbian (Debian-based).

## Setup

```sh
./flash.sh armbian-nanopi-r2s.img.xz /dev/disk4   # write SD card + first-boot network config
make provision HOST=root@<its-ip>   # sets hostname, installs base packages
```

`flash.sh` writes the OS image and configures first-boot networking (DHCP over the
LAN1 Ethernet port -- there's no WiFi to join). `provision.sh` runs its commands over
SSH -- plain and safe to re-run, nothing more to install locally.

Default login after first boot: `root` / `1234` (forced password change on first SSH).
