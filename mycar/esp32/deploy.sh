#!/usr/bin/env bash
# Build + upload one sketch file to the connected ESP32.
# Usage: ./deploy.sh <path/to/sketch.ino> [speed]
set -euo pipefail
cd "$(dirname "$0")"
source ./detect.sh

FILE="${1:?Usage: ./deploy.sh <path/to/sketch.ino> [speed]}"
SPEED="${2:-}"
BOARD_ID="esp32:esp32:esp32"

# Known-good upload speed per chip family, from what has actually worked on
# that hardware (USB-serial adapters, not the chip, are usually the limit).
# Unlisted families fall back to 115200, the most broadly compatible rate.
preset_speed() {
  case "$1" in
    ESP32-*) echo "115200" ;;  # verified on this ESP32 Dev Board Kit
    *)       echo "115200" ;;
  esac
}

[ -f "$FILE" ] || { echo "File not found: $FILE"; exit 1; }

SKETCH_NAME="$(basename "${FILE%.*}")"
BUILD_DIR="build/$SKETCH_NAME"

command -v arduino-cli >/dev/null || { echo "arduino-cli not found. Install it: brew install arduino-cli"; exit 1; }

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

SPEED="${SPEED:-$(preset_speed "$DETECTED_CHIP_TYPE")}"
echo "--- uploading at $SPEED baud ---"
if arduino-cli upload -p "$PORT" --fqbn "$BOARD_ID:UploadSpeed=$SPEED" "$BUILD_DIR"; then
  echo "Uploaded at $SPEED baud."
  exit 0
fi

echo "Upload failed at $SPEED baud."
echo "Pick a speed to retry:"
OTHER_SPEEDS=(921600 460800 230400 115200 57600)
select CHOSEN in "${OTHER_SPEEDS[@]}"; do
  [ -n "$CHOSEN" ] && break
  echo "Invalid choice, try again."
done

echo "--- retrying upload at $CHOSEN baud ---"
if arduino-cli upload -p "$PORT" --fqbn "$BOARD_ID:UploadSpeed=$CHOSEN" "$BUILD_DIR"; then
  echo "Uploaded at $CHOSEN baud."
  exit 0
fi
echo "Upload failed at $CHOSEN baud too."
exit 1
