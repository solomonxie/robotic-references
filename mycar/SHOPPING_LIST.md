# mycar shopping list

Prices are rough estimates from a spot search (Aug 2026), not live quotes --
check current listings before buying. Nothing here is needed before Phase 1
("Drive") in [README.md](./README.md); add items as each phase needs them.

## Phase 4 (See)

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| [Raspberry Pi Camera Module 3](https://www.raspberrypi.com/products/camera-module-3/) | Standard (non-wide) | ~$25 | Wide-angle version is ~$35 if you want a broader patrol field of view |
| [Pi Zero camera ribbon adapter](https://www.amazon.com/Makerfocus-Raspberry-Camera-Cable-Ribbon/dp/B0716TB6X3) | 15-pin to 22-pin FPC cable | ~$5-7 | Required -- the Pi Zero's CSI connector is smaller than a full-size Pi's; the camera module's stock cable won't fit without this |

## Phase 5 (Hear)

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| [USB mini microphone](https://www.amazon.com/SunFounder-Microphone-Raspberry-Recognition-Software/dp/B01KLRBHGM) | USB 2.0, plug-and-play | ~$6-12 | Any cheap USB mic works for Whisper API transcription; no driver needed |
| Micro-USB OTG adapter | Micro-USB (Pi Zero) to USB-A female | ~$5-8 | Pi Zero has one micro-USB data port; skip if you already have one |

Talk output reuses the Bluetooth speaker you already have -- nothing to buy there.

## Power (needed before Phase 1)

Full design/reasoning in [POWER.md](./POWER.md).

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| **Protected** 18650 Li-ion cells | 2x, for the kit's included battery box (count its cell slots to confirm 2 is right) | ~$10-18 for 2 | Protected, not bare cells -- the built-in protection PCB is the actual safety measure here, this build has no separate BMS |
| 18650 bay charger | Standalone charger, e.g. Nitecore/XTAR-style | ~$10-15 | The battery holder has no balance-charge tap -- remove cells to charge individually, don't try to charge the assembled pack |
| [UBEC buck converter](https://www.adafruit.com/product/1385) | 5V @ 3A output | ~$8-17 | Separate clean 5V rail for the Pi + ESP32 -- don't power logic boards from the L298N's onboard regulator or share the motor battery directly |
| Bulk capacitor | 470-1000uF electrolytic | ~$1-3 | Across the buck converter's output -- smooths transient dips from WiFi/motor current spikes |
| On/off switch + inline fuse | 5-10A fuse, for the main battery line | ~$3-5 | Not strictly required, but standard practice for a battery-powered mobile robot |

## Flag if missing

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| MicroSD card | 16-32GB, Class 10 / A1 | ~$8-10 | Needed to flash the Pi Zero's OS -- not in your listed inventory, confirm you have one |
