#!/usr/bin/env bash
# Prints the connected ESP32's serial port and chip info.
set -euo pipefail
cd "$(dirname "$0")"

detect_port() {
  arduino-cli board list 2>/dev/null | awk '/Serial Port \(USB\)/{print $1; exit}'
}

detect_chip() {
  local port="$1"
  python3 -m venv venv >/dev/null 2>&1 || true
  venv/bin/pip install -q --upgrade pip esptool
  venv/bin/python -m esptool --port "$port" flash_id 2>/dev/null \
    | grep -E "Chip is|Features|Crystal|MAC:" || true
}

# Only run standalone when executed directly, not when sourced by deploy.sh.
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
  PORT=$(detect_port)
  [ -n "$PORT" ] || { echo "No ESP32 serial port detected. Plug in the board."; exit 1; }
  echo "Port: $PORT"
  detect_chip "$PORT"
fi
