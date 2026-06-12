# mycar shopping list

All sourced from AliExpress. Prices are rough estimates from a spot search
(Aug 2026), not live quotes -- check current listings/seller ratings before
buying, and expect longer shipping times than Amazon. Nothing here is needed
before Phase 1 ("Drive") in [README.md](./README.md); add items as each phase
needs them.

## Power (needed before Phase 1)

Full design/reasoning in [POWER.md](./POWER.md).

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| **Protected** 18650 Li-ion cells | 2x, 2500-3000mAh | ~$3-6 each (~$6-12 for 2) | [Search: 18650 protected](https://www.aliexpress.com/w/wholesale-18650-protect.html) -- protected, not bare cells, this build has no separate BMS |
| 18650 bay charger | Dual-bay, USB | ~$7 | [Specific listing, $6.99](https://www.aliexpress.com/item/32918123553.html) -- the battery holder has no balance-charge tap, charge cells individually, not as an assembled pack |
| UBEC buck converter | 5V @ 3A output | ~$2-5 | [Search: UBEC 5V 3A](https://www.aliexpress.com/w/wholesale-ubec-5v-3a.html) -- separate clean 5V rail for the Pi + ESP32 |
| Bulk capacitor | 470-1000uF electrolytic | ~$1-3 (often sold in assortment packs) | Across the buck converter's output, smooths transient dips |
| Mini rocker switch | On/off, for the main battery line | ~$1-2 | Basic 2-pin toggle/rocker switch |
| Inline fuse holder + fuse | 5-10A | ~$1-3 | [Search: inline fuse holder](https://www.aliexpress.com/w/wholesale-inline-fuse-holder.html) |

## Phase 4 (See)

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| OV5647 camera module | 5MP, for Raspberry Pi | ~$12-15 | [Example listing, $12.14](https://www.aliexpress.com/item/32831783732.html) -- generic OV5647 clone, not the official Pi Camera Module 3, but functionally compatible and the common AliExpress choice |
| Pi Zero camera ribbon cable | 15-pin to 22-pin FFC, 16-30cm | ~$1-2 | [Example listing, $0.76](https://www.aliexpress.com/item/32829537794.html) -- required, the Pi Zero's CSI connector is smaller than a full-size Pi's |

## Phase 5 (Hear)

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| USB mini microphone | USB 2.0, plug-and-play | ~$3-6 | [Search: USB microphone raspberry pi](https://www.aliexpress.com/w/wholesale-usb-microphone-raspberry-pi.html) |
| Micro-USB OTG adapter | Micro-USB (Pi Zero) to USB-A female | ~$1-3 | [Search: micro USB OTG](https://www.aliexpress.com/w/wholesale-micro-usb-otg.html) -- Pi Zero has one micro-USB data port; skip if you already have one |

Talk output reuses the Bluetooth speaker you already have -- nothing to buy there.

## Flag if missing

| Item | Spec | Est. price | Notes |
|---|---|---|---|
| MicroSD card | 16-32GB, Class 10 / A1 | ~$4-8 | Needed to flash the Pi Zero's OS -- not in your listed inventory, confirm you have one. **Buy from a highly-rated store only**: counterfeit/mislabeled-capacity SD cards are a known AliExpress risk, worse than most other categories here |
