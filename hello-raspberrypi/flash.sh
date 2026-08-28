#!/usr/bin/env bash
# Flash Raspberry Pi OS onto an SD card and pre-configure it for headless boot
# (SSH + WiFi, no monitor/keyboard needed). macOS only (uses diskutil).
#
# Pi Zero W (2017) is ARMv6 -- Raspberry Pi OS Bookworm dropped ARMv6 support,
# so you need a "Legacy" (Bullseye-based) 32-bit Lite image, e.g. from:
#   https://downloads.raspberrypi.com/raspios_lite_armhf/images/
#
# Usage: ./flash.sh <image.img|image.img.xz> <target-disk> <wifi-ssid> <wifi-password>
#   target-disk: whole-disk device from `diskutil list`, e.g. /dev/disk4 (NOT /dev/disk4s1)
set -euo pipefail

IMG="${1:?Usage: ./flash.sh <image.img[.xz]> <target-disk> <wifi-ssid> <wifi-password>}"
DISK="${2:?Target disk required, e.g. /dev/disk4 -- check with: diskutil list}"
WIFI_SSID="${3:?WiFi SSID required}"
WIFI_PASS="${4:?WiFi password required}"
PI_HOSTNAME="${PI_HOSTNAME:-pi-zero}"
PI_USER="${PI_USER:-pi}"

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

BOOT_MOUNT="/Volumes/bootfs"
[ -d "$BOOT_MOUNT" ] || BOOT_MOUNT="/Volumes/boot"
[ -d "$BOOT_MOUNT" ] || { echo "Couldn't find the boot partition mount point."; exit 1; }

touch "$BOOT_MOUNT/ssh"

echo "Set a login password for user '$PI_USER':"
read -rs PI_PASSWORD
echo
PASS_HASH=$(openssl passwd -6 "$PI_PASSWORD")
echo "$PI_USER:$PASS_HASH" > "$BOOT_MOUNT/userconf.txt"

cat > "$BOOT_MOUNT/wpa_supplicant.conf" <<EOF
country=US
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
    ssid="$WIFI_SSID"
    psk="$WIFI_PASS"
}
EOF

diskutil unmountDisk "$DISK"
echo "Done. Insert the SD card into the Pi Zero and power it on."
echo "It should appear on the network in ~1-2 minutes as: $PI_HOSTNAME.local"
echo "SSH in with: ssh $PI_USER@$PI_HOSTNAME.local"
