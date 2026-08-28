#!/usr/bin/env bash
# Install base software over SSH. Safe to re-run.
# Usage: ./provision.sh <user@host> [hostname]
set -euo pipefail
TARGET="${1:?Usage: ./provision.sh <user@host> [hostname]}"
NEW_HOSTNAME="${2:-nanopi-r2s}"

ssh "$TARGET" bash -s <<EOF
set -euo pipefail
hostnamectl set-hostname "$NEW_HOSTNAME"

apt update && apt full-upgrade -y
apt install -y git build-essential python3 python3-pip vim htop tmux
EOF
