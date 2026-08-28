#!/usr/bin/env bash
# Flash Armbian onto the NanoPi R2S's SD card and pre-configure first boot.
# macOS only (uses diskutil).
#
# Download the current NanoPi R2S image from https://www.armbian.com/nanopi-r2s/
#
# The R2S has NO built-in WiFi -- only 2x Gigabit Ethernet (WAN/LAN1). Plug
# LAN1 into your router/switch; it gets a DHCP address by default.
#
# Usage: ./flash.sh <armbian-image.img|.img.xz> <target-disk>
#   target-disk: whole-disk device from `diskutil list`, e.g. /dev/disk4 (NOT /dev/disk4s1)
set -euo pipefail

IMG="${1:?Usage: ./flash.sh <armbian-image.img[.xz]> <target-disk>}"
DISK="${2:?Target disk required, e.g. /dev/disk4 -- check with: diskutil list}"

echo "This will ERASE ALL DATA on $DISK. Ctrl+C now to abort."
sleep 5

RAW_IMG="$IMG"
if [[ "$IMG" == *.xz ]]; then
  RAW_IMG="${IMG%.xz}"
  [ -f "$RAW_IMG" ] || xz -dk "$IMG"
fi

diskutil unmountDisk "$DISK"
sudo dd if="$RAW_IMG" of="$DISK" bs=4m status=progress
sync

diskutil unmountDisk "$DISK"
diskutil mountDisk "$DISK"

BOOT_MOUNT="/Volumes/armbi_boot"
if [ -d "$BOOT_MOUNT" ] && [ -f "$BOOT_MOUNT/armbian_first_run.txt.template" ]; then
  cp "$BOOT_MOUNT/armbian_first_run.txt.template" "$BOOT_MOUNT/armbian_first_run.txt"
  sed -i '' 's/FR_net_change_defaults=.*/FR_net_change_defaults=1/' "$BOOT_MOUNT/armbian_first_run.txt"
  sed -i '' 's/FR_net_ethernet_enabled=.*/FR_net_ethernet_enabled=1/' "$BOOT_MOUNT/armbian_first_run.txt"
  sed -i '' 's/FR_net_use_static=.*/FR_net_use_static=0/' "$BOOT_MOUNT/armbian_first_run.txt"
else
  echo "Note: no armbian_first_run.txt.template found on boot partition -- skipping first-run network config (defaults to DHCP anyway)."
fi

diskutil unmountDisk "$DISK"
echo "Done. Insert into the NanoPi R2S, connect LAN1 to your network, power it on."
echo "Find its DHCP lease in your router's client list, then:"
echo "  ssh root@<its-ip>   (default password: 1234, you'll be forced to change it)"
