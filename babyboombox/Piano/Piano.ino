/*
 * Piano - 4×4 keypad as piano (16 notes from C), buzzer via PWM/tone()
 *
 * Each button = one piano note (C4 through D#5). Sound through buzzer on a PWM-capable pin.
 * Library: Keypad
 *
 * --- WIRING ---
 * Buzzer → Pin 9 (+), GND (-)
 * Keypad: Rows 10, 8, 7, 6; Cols 5, 4, 3, 2
 *
 * --- KEY → NOTE ---
 * 1→C4, 2→C#4, 3→D4, A→D#4, 4→E4, 5→F4, 6→F#4, B→G4,
 * 7→G#4, 8→A4, 9→A#4, C→B4, *→phrase (eighth notes), 0→phrase (quarter notes), #→phrase (quarter notes), D→phrase (eighth notes)
 */

#include <Keypad.h>

const int BUZZER_PIN = 9;
const unsigned int NOTE_MS = 250;    // note length per key press (other keys)
const unsigned int EIGHTH_MS = 250;  // eighth-note length for D phrase
const unsigned int QUARTER_MS = 300; // quarter-note length for # phrase

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {10, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Piano note frequencies (Hz), C4 through D#5 (equal temperament, A4 = 440)
unsigned int keyToFreq(char key) {
  switch (key) {
    case '1': return 262;   // C4
    case '2': return 277;   // C#4
    case '3': return 294;   // D4
    case 'A': return 311;   // D#4
    case '4': return 330;   // E4
    case '5': return 349;   // F4
    case '6': return 370;   // F#4
    case 'B': return 392;   // G4
    case '7': return 415;   // G#4
    case '8': return 440;   // A4
    case '9': return 466;   // A#4
    case 'C': return 494;   // B4
    case '*': return 523;   // C5
    case '0': return 554;   // C#5
    case '#': return 587;   // D5
    case 'D': return 622;   // D#5 (used only if not playing phrase)
    default:  return 0;
  }
}

// D key: play A#3, A#3, A#3, A#3, G3, D#3, F3 (eighth notes)
const unsigned int PHRASE_D_LEN = 7;
const unsigned int PHRASE_D[] = { 233, 233, 233, 233, 196, 156, 175 }; // A#3, A#3, A#3, A#3, G3, D#3, F3

void playPhraseD() {
  for (int i = 0; i < PHRASE_D_LEN; i++) {
    tone(BUZZER_PIN, PHRASE_D[i], EIGHTH_MS);
    delay(EIGHTH_MS);
  }
  noTone(BUZZER_PIN);
}

// # key: F#3, F#3, F#3, C#4, B3, B3, B3, C#4, A3, A3, A3, B3, G#3, G#3, G#3 (quarter notes)
const unsigned int PHRASE_HASH_LEN = 15;
const unsigned int PHRASE_HASH[] = { 185, 185, 185, 277, 247, 247, 247, 277, 220, 220, 220, 247, 208, 208, 208 };

void playPhraseHash() {
  for (int i = 0; i < PHRASE_HASH_LEN; i++) {
    tone(BUZZER_PIN, PHRASE_HASH[i], QUARTER_MS);
    delay(QUARTER_MS);
  }
  noTone(BUZZER_PIN);
}

// 0 key: D#3, D#3, D#3, F#3, G#3, G#3, A#3, A#3, G#3, G#3, G#3, F#3, A#3, A#3, A#3 (quarter notes)
const unsigned int PHRASE_0_LEN = 15;
const unsigned int PHRASE_0[] = { 156, 156, 156, 185, 208, 208, 233, 233, 208, 208, 208, 185, 233, 233, 233 };

void playPhrase0() {
  for (int i = 0; i < PHRASE_0_LEN; i++) {
    tone(BUZZER_PIN, PHRASE_0[i], QUARTER_MS);
    delay(QUARTER_MS);
  }
  noTone(BUZZER_PIN);
}

// * key: D#3, A#2, C#2, C#2, D#2, D#2, D#2 (eighth notes)
const unsigned int PHRASE_STAR_LEN = 7;
const unsigned int PHRASE_STAR[] = { 156, 117, 69, 69, 78, 78, 78 }; // D#3, A#2, C#2, C#2, D#2, D#2, D#2

void playPhraseStar() {
  for (int i = 0; i < PHRASE_STAR_LEN; i++) {
    tone(BUZZER_PIN, PHRASE_STAR[i], EIGHTH_MS);
    delay(EIGHTH_MS);
  }
  noTone(BUZZER_PIN);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  char key = customKeypad.getKey();
  if (!key) return;

  int speedometerValue = analogRead(A4);
  float playLength = map(speedometerValue, 0, 1023, 50, 400);

  int pitchometerValue = analogRead(A5);
  float pitchMultiplier = map(pitchometerValue, 0, 1023, 50, 200) / 100.0;

  if (key == 'D') {
    playPhraseD();
    return;
  }
  if (key == '#') {
    playPhraseHash();
    return;
  }
  if (key == '0') {
    playPhrase0();
    return;
  }
  if (key == '*') {
    playPhraseStar();
    return;
  }

  unsigned int freq = keyToFreq(key) * pitchMultiplier;
  if (freq > 0) {
    tone(BUZZER_PIN, freq, playLength);
  }
}