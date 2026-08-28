#!/usr/bin/env bash
# Build + upload a sketch to the connected ESP32.
# Scans this folder for .ino files and prompts to pick one (skips the
# prompt if there's only one). Usage: ./deploy.sh [speed]
set -euo pipefail
cd "$(dirname "$0")"
source ./detect.sh

SPEED="${1:-}"
BOARD_ID="esp32:esp32:esp32"

SKETCHES=()
for f in *.ino; do
  [ -e "$f" ] && SKETCHES+=("$f")
done
[ ${#SKETCHES[@]} -gt 0 ] || { echo "No .ino files found in $(pwd)"; exit 1; }

if [ ${#SKETCHES[@]} -eq 1 ]; then
  FILE="${SKETCHES[0]}"
  echo "Deploying $FILE"
else
  echo "Select a sketch to deploy:"
  select FILE in "${SKETCHES[@]}"; do
    [ -n "$FILE" ] && break
    echo "Invalid choice, try again."
  done
fi

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
