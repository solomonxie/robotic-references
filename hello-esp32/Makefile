# Standalone deploy pipeline for hello-esp32 sketches.
# Usage: make deploy FILE=hello_03_3lights.ino
#        make deploy FILE=hello_03_3lights.ino SPEED=115200   # skip auto speed detection
#        make detect                                          # just show port + chip info

BOARD_ID     := esp32:esp32:esp32
UPLOAD_SPEEDS ?= 921600 460800 230400 115200 57600
SPEED         ?=
BUILD_ROOT    := build
VENV          := venv

ifdef SPEED
UPLOAD_SPEEDS := $(SPEED)
endif

SKETCH_NAME := $(basename $(notdir $(FILE)))
BUILD_DIR   := $(BUILD_ROOT)/$(SKETCH_NAME)

PORT ?= $(shell arduino-cli board list 2>/dev/null | awk '/Serial Port \(USB\)/{print $$1; exit}')

.PHONY: deploy build detect port chip check-cli check-core venv clean

check-cli:
	@command -v arduino-cli >/dev/null || { \
		echo "arduino-cli not found. Install it, e.g.: brew install arduino-cli"; exit 1; }

check-core: check-cli
	@arduino-cli core list | grep -q '^esp32:esp32' || { \
		echo "Installing esp32 core..."; \
		arduino-cli core update-index && arduino-cli core install esp32:esp32; }

port:
	@if [ -z "$(PORT)" ]; then \
		echo "No ESP32 serial port detected. Plug in the board, or pass PORT=/dev/cu.xxx"; exit 1; \
	fi
	@echo "Port: $(PORT)"

$(VENV)/bin/python:
	python3 -m venv $(VENV)
	$(VENV)/bin/pip install -q --upgrade pip esptool

chip: port $(VENV)/bin/python
	@$(VENV)/bin/python -m esptool --port $(PORT) flash_id 2>/dev/null \
		| grep -E "Chip is|Features|Crystal|MAC:" || true

detect: port chip

build: check-core
	@test -n "$(FILE)" || { echo "Usage: make deploy FILE=<sketch>.ino"; exit 1; }
	@test -f "$(FILE)" || { echo "File not found: $(FILE)"; exit 1; }
	mkdir -p $(BUILD_DIR)
	cp $(FILE) $(BUILD_DIR)/$(SKETCH_NAME).ino
	arduino-cli compile --fqbn $(BOARD_ID) $(BUILD_DIR)

deploy: detect build
	@for speed in $(UPLOAD_SPEEDS); do \
		echo "--- trying upload at $$speed baud ---"; \
		if arduino-cli upload -p $(PORT) --fqbn $(BOARD_ID):UploadSpeed=$$speed $(BUILD_DIR); then \
			echo "Uploaded at $$speed baud."; \
			exit 0; \
		fi; \
	done; \
	echo "Upload failed at all speeds: $(UPLOAD_SPEEDS)"; exit 1

clean:
	rm -rf $(BUILD_ROOT)
