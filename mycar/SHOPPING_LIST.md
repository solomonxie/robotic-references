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

## Power (confirm before Phase 1 if needed)

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| [UBEC buck converter](https://www.adafruit.com/product/1385) | 5V @ 3A output | ~$8-17 | Only needed if the Electronic Fun Kit's power module can't supply a clean, separate 5V rail for the Pi alongside motor power -- see README's "Power" assumption |
| On/off switch + inline fuse | For the main battery line | ~$3-5 | Not strictly required, but standard practice for a battery-powered mobile robot |

## Flag if missing

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| MicroSD card | 16-32GB, Class 10 / A1 | ~$8-10 | Needed to flash the Pi Zero's OS -- not in your listed inventory, confirm you have one |
