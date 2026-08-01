# DRAKE_2_0_PAWB

Arduino Pro Mini (5V / 16 MHz) **paw / claw** firmware.

- 5× NeoPixels pin 8  
- ASK RX @ 2000 baud on **A0** (matches Tail TX pin 17)  
- **Modes 0–10 non-blocking** (follow Tail over ASK)

## ASK protocol (from Tail)

| Packet | When | Purpose |
|--------|------|---------|
| `M0`…`M9` / `MA` | Mode change + ~30 s suit sync | Mode select (EEPROM) |
| `C<r>,<g>,<b>` | Solid color | Mode 9 + RGB |
| `L` | Flash | White flash |
| `R0` | Idle resync | Reset fade / mode state |
| `m####` | **M0 / M1 / M2 only** | Mic **pulse** (excess level, ~25 Hz max, hits only) |

Modes **3–10** do **not** receive continuous mic — only `M#` / `C` / `R` / `L`.

## Modes

| ID | Name | Mic |
|----|------|-----|
| 0 | Sound Phase | ASK pulse → gated flood |
| 1 | Sound Pulse | ASK pulse → stepped color flood |
| 2 | VU Meter | ASK pulse → bar (excess scale) |
| 3–8 | Rainbow, Comet, Breath, Fire, Sparkle, Wave | Free-run after `M#` |
| 9–10 | Solid, Off | — |

## Flash (Pro Mini 5V 16 MHz)

```bash
cd DRAKE_2_0_PAWB
arduino-cli compile --fqbn arduino:avr:pro:cpu=16MHzatmega328 .
arduino-cli upload -p /dev/cu.usbserial-XXXX --fqbn arduino:avr:pro:cpu=16MHzatmega328 .
```

Use a USB–serial adapter (or ISP). Flash **each** paw separately.

## Repo contents

Mostly `.ino` + text. Vendored libraries required for the build are welcome.  
See [REPO.md](https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/REPO.md).

## Firmware team

- [SYSTEM.md](https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/SYSTEM.md)  
- [FIRMWARE_NOTES.md](https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/FIRMWARE_NOTES.md)  

http://tmdrake.com
