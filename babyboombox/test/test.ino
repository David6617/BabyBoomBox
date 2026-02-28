/*
 * Test: play WAV from flash on buzzer (pin A0), repeat forever.
 * Uses test/001.wav converted to wav_data.h (run convert_wav_to_header.py).
 * Buzzer: A0 (+) and GND (-). A0 is not PWM on Uno — playback is 1-bit (on/off).
 */

#include <avr/pgmspace.h>
#include "wav_data.h"

const int BUZZER_PIN = A0;  // Analog pin A0 (digital 14) on Uno
const unsigned int SAMPLE_RATE_HZ = 8000;

void playOnce() {
  unsigned long usPerSample = 1000000UL / SAMPLE_RATE_HZ;
  for (unsigned int i = 0; i < wav_001_len; i++) {
    uint8_t s = pgm_read_byte(wav_001 + i);
    digitalWrite(BUZZER_PIN, (s > 128) ? HIGH : LOW);
    delayMicroseconds(usPerSample);
  }
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  playOnce();
  // Optional: short gap between repeats
  delay(50);
}
