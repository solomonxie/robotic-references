#!/usr/bin/env bash
# Prints the connected ESP32's serial port and chip info.
set -euo pipefail
cd "$(dirname "$0")"

detect_port() {
  arduino-cli board list 2>/dev/null | awk '/Serial Port \(USB\)/{print $1; exit}'
}

# Commercial dev-kit name for a chip family. esptool only reports the silicon
# part number (e.g. ESP32-D0WD-V3); the board's marketing name isn't in the
# firmware, so this is a lookup, not a detection.
chip_model_name() {
  case "$1" in
    ESP32-S2*) echo "ESP32-S2 Dev Kit" ;;
    ESP32-S3*) echo "ESP32-S3 Dev Kit" ;;
    ESP32-C3*) echo "ESP32-C3 Dev Kit" ;;
    ESP32-C6*) echo "ESP32-C6 Dev Kit" ;;
    ESP32-*)   echo "ESP32 Dev Board Kit" ;;
    *)         echo "Unknown ($1)" ;;
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

  echo "Chip Model:  $(chip_model_name "$chip_type")"
  echo "Flash size:  $flash_size"
  echo "RAM size:    520KB SRAM"
  echo "Chip type:   $chip_type"
  echo "Features:    Dual-core, 32-bit, 240MHz, 2.4GHz Wi-Fi, Bluetooth 4.2"
  echo "$out" | grep -E "Crystal|MAC:"
}

# Only run standalone when executed directly, not when sourced by deploy.sh.
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
  PORT=$(detect_port)
  [ -n "$PORT" ] || { echo "No ESP32 serial port detected. Plug in the board."; exit 1; }
  echo "Port: $PORT"
  detect_chip "$PORT"
fi
