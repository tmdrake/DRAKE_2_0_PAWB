# DRAKE_2_0_PAWB

Arduino Pro Mini (5V / 16 MHz) **paw / claw** firmware.

- 5× NeoPixels pin 8
- ASK RX @ 2000 baud on A0 (same rate as Tail TX pin 17)
- **Modes 0–10 non-blocking** (follow Tail over ASK)

## Modes

| ID | Name |
|----|------|
| 0–2 | Sound Phase, Distinct, VU |
| 3–8 | Rainbow, Comet, Breath, Fire, Sparkle, Wave |
| 9–10 | Solid, Off |

ASK: `M0`–`M9` / `MA`, `L`, `R`, `m####` (mic).

## Firmware team
https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/FIRMWARE_NOTES.md  
https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/SYSTEM.md

http://tmdrake.com
