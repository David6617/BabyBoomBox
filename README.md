# BabyBoomBox

Physical soundboard: 4×4 keypad → WAV playback on buzzer (16 buttons, 16 .wav files). No DFPlayer; uses SD card module + TMRpcm.

## Key → WAV file mapping

| Key | File     | Key | File     |
|-----|----------|-----|----------|
| 1   | 001.wav  | 9   | 009.wav  |
| 2   | 002.wav  | A   | 010.wav  |
| 3   | 003.wav  | B   | 011.wav  |
| 4   | 004.wav  | C   | 012.wav  |
| 5   | 005.wav  | *   | 013.wav  |
| 6   | 006.wav  | 0   | 014.wav  |
| 7   | 007.wav  | #   | 015.wav  |
| 8   | 008.wav  | D   | 016.wav  |

Place **001.wav** … **016.wav** in the **root** of the SD card (FAT16 or FAT32).

## WAV format (required)

- **8-bit unsigned PCM**, **mono**, **8–32 kHz** (16 kHz recommended).
- In **Audacity:** Tracks → Resample → 16000 → File → Export → Export as WAV → **Unsigned 8-bit PCM**.

## Wiring

- **Buzzer:** Pin 10 → buzzer +, GND → buzzer -.
- **SD card module (SPI):** CS → A0, MOSI → 11, MISO → 12, SCK → 13, 5V, GND.
- **Keypad:** Rows → 9, 8, 7, 6; Cols → 5, 4, 3, 2.

## Setup

- **Hardware:** Arduino, 4×4 keypad, **SD card module** (e.g. generic SPI MicroSD breakout), buzzer.
- **Libraries:** **Keypad** and **TMRpcm** (Library Manager). SD and SPI are built-in.
- Open **babyboombox/BabyBoomBox.ino** in Arduino IDE.
