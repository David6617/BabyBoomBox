# BabyBoomBox

Physical soundboard: keypad → MP3 playback (Phase 1).

## Key → audio file mapping

| Key | File    | Key | File    |
|-----|---------|-----|---------|
| 1   | 001.mp3 | 9   | 009.mp3 |
| 2   | 002.mp3 | A   | 010.mp3 |
| 3   | 003.mp3 | B   | 011.mp3 |
| 4   | 004.mp3 | C   | 012.mp3 |
| 5   | 005.mp3 | *   | 013.mp3 |
| 6   | 006.mp3 | 0   | 014.mp3 |
| 7   | 007.mp3 | #   | 015.mp3 |
| 8   | 008.mp3 | D   | 016.mp3 |

Place **001.mp3** … **016.mp3** in the root of the SD card (FAT32, insert before power-on).

## Setup

- **Hardware:** Arduino, DFPlayer Mini, 4×4 keypad, buzzer (output from DFPlayer SPK_1/SPK_2), MicroSD.
- Open **BabyBoomBox/BabyBoomBox.ino** (or **babyboombox/BabyBoomBox.ino**) in Arduino IDE.
- Install libraries: **Sketch → Include Library → Manage Libraries** → **DFPlayer Mini** (DFRobot) and **Keypad**.
- Wiring is in the comment block at the top of the sketch.