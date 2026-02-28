/*
 * BabyBoomBox - Keypad → WAV playback on buzzer (16 buttons, 16 WAV files)
 *
 * Plays .wav files from SD card through the buzzer. No DFPlayer.
 * Libraries: Keypad, TMRpcm, SD (built-in), SPI (built-in)
 *
 * --- WIRING ---
 * Buzzer → Arduino:
 *   - Buzzer + → Pin 10, Buzzer - → GND
 *
 * SD card module (SPI) → Arduino:
 *   - CS  → A0 (pin 14)
 *   - MOSI→ 11, MISO→ 12, SCK→ 13
 *   - 5V  → 5V, GND → GND
 *
 * Keypad 4×4 → Arduino:
 *   - Row pins: 9, 8, 7, 6
 *   - Col pins: 5, 4, 3, 2
 *
 * --- SD CARD ---
 *   - FAT16 or FAT32. In root: 001.wav, 002.wav, ... 016.wav
 *   - WAV format: 8-bit unsigned PCM, mono, 8–32 kHz (16 kHz recommended)
 *   - Use Audacity: Tracks → Resample → 16000; Export → WAV → Unsigned 8-bit PCM
 */

#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>
#include <Keypad.h>

#define SD_ChipSelectPin A0  // SD card module CS (keypad uses 2–9, speaker uses 10)

const int SPEAKER_PIN = 10;  // Buzzer/speaker (PWM pin for TMRpcm)
TMRpcm tmrpcm;

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Key → WAV filename (001.wav .. 016.wav). Returns nullptr if unknown.
const char* keyToWav(char key) {
  switch (key) {
    case '1': return "001.wav";
    case '2': return "002.wav";
    case '3': return "003.wav";
    case 'A': return "010.wav";
    case '4': return "004.wav";
    case '5': return "005.wav";
    case '6': return "006.wav";
    case 'B': return "011.wav";
    case '7': return "007.wav";
    case '8': return "008.wav";
    case '9': return "009.wav";
    case 'C': return "012.wav";
    case '*': return "013.wav";
    case '0': return "014.wav";
    case '#': return "015.wav";
    case 'D': return "016.wav";
    default:  return nullptr;
  }
}

void setup() {
  tmrpcm.speakerPin = SPEAKER_PIN;

  if (!SD.begin(SD_ChipSelectPin)) {
    // SD init failed — hold in a loop (or blink LED if you add one)
    while (1) { delay(1000); }
  }

  tmrpcm.volume(1);
  tmrpcm.quality(1);
}

void loop() {
  char key = customKeypad.getKey();
  if (!key) return;

  const char* fname = keyToWav(key);
  if (fname) {
    if (tmrpcm.isPlaying()) tmrpcm.stop();
    tmrpcm.play(fname);
  }
}
