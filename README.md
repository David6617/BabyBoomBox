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

---

## RealBabyBoo (WAV in flash, no SD card)

- **Sketch:** `babyboombox/RealBabyBoo/RealBabyBoo.ino` — same 4×4 keypad mapping (001.wav..016.wav).
- Put **001.wav** … **016.wav** in `babyboombox/RealBabyBoo/`. Then run the converter to embed them in the sketch:
  ```bash
  python tools/convert_all_wav_to_header.py --folder babyboombox/RealBabyBoo
  ```
  This overwrites `RealBabyBoo/wav_data.h`. Missing files get a short silence. Default **~0.19 s per file** (`--max-samples 1500`) so 16 slots fit on Uno flash.

---

## Piano (4×4 keypad as piano)

- **Sketch:** `babyboombox/Piano/Piano.ino` — each key = one piano note (C4 through D#5), played on buzzer via PWM (`tone()`).
- **Wiring:** Buzzer on pin 10 and GND; keypad rows 9,8,7,6 and cols 5,4,3,2.
- **Key → note:** 1→C4, 2→C#4, 3→D4, A→D#4, 4→E4, 5→F4, 6→F#4, B→G4, 7→G#4, 8→A4, 9→A#4, C→B4, *→C5, 0→C#5, #→D5, D→D#5.
- **Library:** Keypad.

---

## Converting WAV files to headers

- **All 16 slots (e.g. RealBabyBoo):**  
  `python tools/convert_all_wav_to_header.py [--folder PATH] [--output PATH] [--max-samples N]`  
  Converts **001.wav** .. **016.wav** in the given folder to one **wav_data.h** (PROGMEM). Default folder: `babyboombox/RealBabyBoo`. Default **--max-samples 1500** (~0.19 s per file, fits Uno).
- **Single file (e.g. test):**  
  `python babyboombox/test/convert_wav_to_header.py` (run from `babyboombox/test/`, uses **001.wav** → **wav_data.h**).
