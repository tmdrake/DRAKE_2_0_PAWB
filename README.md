# DRAKE_2_0_PAWB

Arduino Pro Mini (5V / 16 MHz) firmware for the dragonsuit **paws / claws**.

## Role
- 5× NeoPixels on pin **8**
- **ASK receiver** on **A0**, RadioHead `RH_ASK(2000, A0, 0, 0)` — same 2000 baud as Tail TX
- Follows Tail modes 0–10 over ASK
- Uses Tail mic packets (`m####`) for sound-reactive modes 0–2

## ASK packets (from Tail)
| Packet | Action |
|--------|--------|
| `M0`–`M9` | Mode 0–9 |
| `MA` | Mode 10 (Off) |
| `L…` | Flash |
| `R…` | Resync / reset fade |
| `m####` | Mic level |

## Modes
| Mode | Name |
|------|------|
| 0 | Sound Phase |
| 1 | Sound Distinct |
| 2 | VU Meter |
| 3 | Rainbow Chase |
| 4 | Comet |
| 5 | Breathing |
| 6 | Fire |
| 7 | Sparkle |
| 8 | Wave |
| 9 | Solid |
| 10 | Off |

Modes 3–10 run continuously. Modes 0–2 are sound-reactive.

## Full system docs
**https://github.com/tmdrake/DRAKE_2_0_TAIL/blob/main/SYSTEM.md**

http://tmdrake.com
