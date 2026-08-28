#!/usr/bin/env bash
# Prints the connected ESP32's serial port and chip info.
set -euo pipefail
cd "$(dirname "$0")"

detect_port() {
  arduino-cli board list 2>/dev/null | awk '/Serial Port \(USB\)/{print $1; exit}'
}

# Spec lookup keyed on the chip family `esptool` actually detects over serial
# (the "Chip type:" line, e.g. ESP32-D0WD-V3 vs ESP32-S3 vs ESP32-C3). esptool
# reports the silicon part number but not RAM size, Bluetooth version, or a
# marketing name, so those three come from Espressif's datasheets per family.
chip_specs() {
  case "$1" in
    ESP32-S2*) echo "ESP32-S2 Dev Kit|320KB SRAM|Single-core, 32-bit, 240MHz, 2.4GHz Wi-Fi (no Bluetooth)" ;;
    ESP32-S3*) echo "ESP32-S3 Dev Kit|512KB SRAM|Dual-core, 32-bit, 240MHz, 2.4GHz Wi-Fi, Bluetooth 5 (LE)" ;;
    ESP32-C3*) echo "ESP32-C3 Dev Kit|400KB SRAM|Single-core RISC-V, 32-bit, 160MHz, 2.4GHz Wi-Fi, Bluetooth 5 (LE)" ;;
    ESP32-C6*) echo "ESP32-C6 Dev Kit|512KB SRAM|Single-core RISC-V, 32-bit, 160MHz, Wi-Fi 6, Bluetooth 5 (LE), 802.15.4" ;;
    ESP32-*)   echo "ESP32 Dev Board Kit|520KB SRAM|Dual-core, 32-bit, 240MHz, 2.4GHz Wi-Fi, Bluetooth 4.2" ;;
    *)         echo "Unknown ($1)|unknown|unknown" ;;
  esac
}

detect_chip() {
  local port="$1"
  python3 -m venv venv >/dev/null 2>&1
  venv/bin/pip install -q --upgrade pip esptool
  local out
  out=$(venv/bin/python -m esptool --port "$port" flash-id 2>/dev/null)
  local chip_type flash_size
  chip_type=$(echo "$out" | grep "Chip type:" | sed -E 's/^Chip type:[[:space:]]*//')
  flash_size=$(echo "$out" | grep "Detected flash size:" | sed -E 's/^Detected flash size:[[:space:]]*//')

  local model ram features
  IFS='|' read -r model ram features <<< "$(chip_specs "$chip_type")"

  echo "Chip Model:  $model"
  echo "Flash size:  $flash_size"
  echo "RAM size:    $ram"
  echo "Chip type:   $chip_type"
  echo "Features:    $features"
  echo "$out" | grep -E "Crystal|MAC:"
}

# Only run standalone when executed directly, not when sourced by deploy.sh.
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
  PORT=$(detect_port)
  [ -n "$PORT" ] || { echo "No ESP32 serial port detected. Plug in the board."; exit 1; }
  echo "Port: $PORT"
  detect_chip "$PORT"
fi
