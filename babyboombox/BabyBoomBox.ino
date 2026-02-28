/*
 * BabyBoomBox - Keypad → DFPlayer: one sound per button (16 buttons)
 *
 * Libraries:
 *   - DFRobot DFPlayer Mini (Manage Libraries → "DFPlayer Mini")
 *   - Keypad (Manage Libraries → "Keypad")
 *
 * --- WIRING ---
 * Arduino → DFPlayer Mini:
 *   - 5V    → VCC, GND → GND
 *   - Pin 11 (TX) → [1kΩ resistor] → DFPlayer RX
 *   - Pin 10 (RX) → DFPlayer TX
 *
 * Keypad 4×4 → Arduino (rows then cols):
 *   - Row pins: 9, 8, 7, 6
 *   - Col pins: 5, 4, 3, 2
 *
 * Speaker → DFPlayer: 8Ω between SPK_1 and SPK_2
 *
 * --- SD CARD ---
 *   - FAT32, root: 001.mp3 .. 016.mp3 (one per button, insert before power-on)
 *
 * Button → file: 1→001, 2→002, ... 9→009, A→010, B→011, C→012, *→013, 0→014, #→015, D→016
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Keypad.h>

SoftwareSerial mySerial(10, 11);  // RX, TX for DFPlayer
DFRobotDFPlayerMini myDFPlayer;

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

// Map key character to track number 1..16 (001.mp3 .. 016.mp3). Returns 0 if unknown.
uint8_t keyToTrack(char key) {
  if (key >= '1' && key <= '9') return (uint8_t)(key - '0');      // 1-9 → 1-9
  if (key == '0') return 14;
  if (key == 'A') return 10;
  if (key == 'B') return 11;
  if (key == 'C') return 12;
  if (key == 'D') return 16;
  if (key == '*') return 13;
  if (key == '#') return 15;
  return 0;
}

void setup() {
  mySerial.begin(9600);
  delay(2000);

  if (!myDFPlayer.begin(mySerial)) {
    while (true) { delay(1000); }
  }

  myDFPlayer.volume(20);   // 0-30
  // No auto-play; wait for keypad
}

void loop() {
  char key = customKeypad.getKey();
  if (!key) return;

  uint8_t track = keyToTrack(key);
  if (track != 0) {
    myDFPlayer.play(track);
  }
}
