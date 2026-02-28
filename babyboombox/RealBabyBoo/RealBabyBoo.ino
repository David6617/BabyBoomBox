/*
 * RealBabyBoo - 4×4 keypad → WAV playback from flash (16 buttons, 001.wav..016.wav)
 *
 * WAV data is in wav_data.h (generated from RealBabyBoo/*.wav by convert_all_wav_to_header.py).
 * Library: Keypad only.
 *
 * --- WIRING ---
 * Buzzer → Pin 10, GND
 * Keypad: Rows 9,8,7,6; Cols 5,4,3,2
 *
 * --- KEY → FILE ---
 * 1→001, 2→002, 3→003, A→010, 4→004, 5→005, 6→006, B→011,
 * 7→007, 8→008, 9→009, C→012, *→013, 0→014, #→015, D→016
 *
 * Generate wav_data.h:  python tools/convert_all_wav_to_header.py --folder babyboombox/RealBabyBoo
 */

#include <avr/pgmspace.h>
#include <Keypad.h>
#include "wav_data.h"

const int SPEAKER_PIN = 10;
const unsigned int SAMPLE_RATE_HZ = 8000;

static const uint8_t* const wav_ptrs[16] = {
  wav_000, wav_001, wav_002, wav_003, wav_004, wav_005, wav_006, wav_007,
  wav_008, wav_009, wav_010, wav_011, wav_012, wav_013, wav_014, wav_015
};
static const unsigned int wav_lens[16] = {
  wav_000_len, wav_001_len, wav_002_len, wav_003_len, wav_004_len, wav_005_len,
  wav_006_len, wav_007_len, wav_008_len, wav_009_len, wav_010_len, wav_011_len,
  wav_012_len, wav_013_len, wav_014_len, wav_015_len
};

void wavGet(uint8_t slot, const uint8_t** data, unsigned int* len) {
  if (slot >= 16) { *data = nullptr; *len = 0; return; }
  *data = wav_ptrs[slot]; *len = wav_lens[slot];
}

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

uint8_t keyToSlot(char key) {
  switch (key) {
    case '1': return 0;  case '2': return 1;  case '3': return 2;  case 'A': return 9;
    case '4': return 3;  case '5': return 4;  case '6': return 5;  case 'B': return 10;
    case '7': return 6;  case '8': return 7;  case '9': return 8;  case 'C': return 11;
    case '*': return 12; case '0': return 13; case '#': return 14; case 'D': return 15;
    default:  return 255;
  }
}

void playFromProgmem(const uint8_t* data, unsigned int length) {
  if (!data || length == 0) return;
  unsigned long usPerSample = 1000000UL / SAMPLE_RATE_HZ;
  for (unsigned int i = 0; i < length; i++) {
    uint8_t s = pgm_read_byte(data + i);
    analogWrite(SPEAKER_PIN, s >> 1);
    delayMicroseconds(usPerSample);
  }
  analogWrite(SPEAKER_PIN, 0);
}

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
}

void loop() {
  char key = customKeypad.getKey();
  if (!key) return;
  uint8_t slot = keyToSlot(key);
  if (slot > 15) return;
  const uint8_t* data;
  unsigned int len;
  wavGet(slot, &data, &len);
  playFromProgmem(data, len);
}
