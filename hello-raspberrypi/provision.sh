#!/usr/bin/env bash
# Install base software and apply Pi-specific config over SSH. Safe to re-run.
# Usage: ./provision.sh <user@host> [hostname]
set -euo pipefail
TARGET="${1:?Usage: ./provision.sh <user@host> [hostname]}"
NEW_HOSTNAME="${2:-pi-zero}"

ssh "$TARGET" bash -s <<EOF
set -euo pipefail
sudo hostnamectl set-hostname "$NEW_HOSTNAME"

sudo apt update && sudo apt full-upgrade -y
sudo apt install -y git build-essential python3 python3-pip vim htop tmux

sudo raspi-config nonint do_camera 0
sudo raspi-config nonint do_i2c 0
sudo raspi-config nonint do_spi 0
EOF
