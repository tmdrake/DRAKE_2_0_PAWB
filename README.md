# DRAKE_2_0_PAWB

Arduino Pro Mini (5V / 16 MHz) firmware for the dragonsuit **paws / claws**.

- 5× NeoPixels on pin 8
- ASK RF receiver (syncs mode + mic from Tail)

## Modes (follow Tail / Head)

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

Modes 3–10 run continuously. Modes 0–2 are sound-reactive using mic levels received over ASK (`m####` packets).

Mode changes arrive as ASK packets `M0`…`M9` / `MA` (mode 10) from the Tail.
