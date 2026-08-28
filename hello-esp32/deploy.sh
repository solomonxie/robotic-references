#!/usr/bin/env bash
# Build + upload one sketch file to the connected ESP32.
# Usage: ./deploy.sh <sketch>.ino [speed]
set -euo pipefail
cd "$(dirname "$0")"
source ./detect.sh

FILE="${1:?Usage: ./deploy.sh <sketch>.ino [speed]}"
SPEED="${2:-}"
BOARD_ID="esp32:esp32:esp32"
SKETCH_NAME="$(basename "${FILE%.*}")"
BUILD_DIR="build/$SKETCH_NAME"

command -v arduino-cli >/dev/null || { echo "arduino-cli not found. Install it: brew install arduino-cli"; exit 1; }
[ -f "$FILE" ] || { echo "File not found: $FILE"; exit 1; }

arduino-cli core list | grep -q '^esp32:esp32' || {
  echo "Installing esp32 core..."
  arduino-cli core update-index && arduino-cli core install esp32:esp32
}

PORT=$(detect_port)
[ -n "$PORT" ] || { echo "No ESP32 serial port detected. Plug in the board."; exit 1; }
echo "Port: $PORT"
detect_chip "$PORT"

mkdir -p "$BUILD_DIR"
cp "$FILE" "$BUILD_DIR/$SKETCH_NAME.ino"
arduino-cli compile --fqbn "$BOARD_ID" "$BUILD_DIR"

SPEEDS="${SPEED:-921600 460800 230400 115200 57600}"
for speed in $SPEEDS; do
  echo "--- trying upload at $speed baud ---"
  if arduino-cli upload -p "$PORT" --fqbn "$BOARD_ID:UploadSpeed=$speed" "$BUILD_DIR"; then
    echo "Uploaded at $speed baud."
    exit 0
  fi
done
echo "Upload failed at all speeds: $SPEEDS"
exit 1
