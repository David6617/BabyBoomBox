#!/usr/bin/env python3
"""
Convert test/001.wav to test/wav_data.h for use with test.ino.
Requires: 8-bit or 16-bit WAV, mono or stereo (stereo → mono mix).
Output: 8-bit unsigned PCM at 8000 Hz. Truncates to ~24 KB to fit Arduino flash.
Usage: run from the test folder:  python convert_wav_to_header.py
"""

import struct
import wave
import os

WAV_PATH = "001.wav"
OUT_PATH = "wav_data.h"
TARGET_RATE = 8000
MAX_SAMPLES = 24000  # ~3 sec at 8 kHz, fits in flash

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    wav_file = os.path.join(script_dir, WAV_PATH)
    out_file = os.path.join(script_dir, OUT_PATH)

    if not os.path.isfile(wav_file):
        print(f"Missing {wav_file}")
        return

    with wave.open(wav_file, "rb") as w:
        nch = w.getnchannels()
        sampwidth = w.getsampwidth()
        framerate = w.getframerate()
        nframes = w.getnframes()
        raw = w.readframes(nframes)

    # Convert to 8-bit mono
    if sampwidth == 1:
        samples = list(struct.unpack_from(f"{nframes * nch}B", raw))
    elif sampwidth == 2:
        fmt = f"<{nframes * nch}h"
        samples_int = struct.unpack_from(fmt, raw)
        samples = [max(0, min(255, (s >> 8) + 128)) for s in samples_int]
    else:
        print("Unsupported sample width")
        return

    if nch == 2:
        samples = [(samples[i] + samples[i + 1]) // 2 for i in range(0, len(samples), 2)]

    # Resample to TARGET_RATE if needed
    if framerate != TARGET_RATE:
        new_len = int(len(samples) * TARGET_RATE / framerate)
        resampled = []
        for i in range(new_len):
            idx = i * framerate / TARGET_RATE
            i0 = int(idx) % len(samples)
            i1 = (i0 + 1) % len(samples)
            frac = idx - int(idx)
            v = int(samples[i0] * (1 - frac) + samples[i1] * frac)
            resampled.append(max(0, min(255, v)))
        samples = resampled

    samples = samples[:MAX_SAMPLES]
    n = len(samples)

    with open(out_file, "w") as f:
        f.write("/* Generated from 001.wav - 8-bit mono, 8000 Hz */\n")
        f.write("#include <avr/pgmspace.h>\n\n")
        f.write(f"const unsigned int wav_001_len = {n};\n\n")
        f.write("const uint8_t wav_001[] PROGMEM = {\n")
        for i in range(0, n, 16):
            chunk = samples[i : i + 16]
            line = ", ".join(f"0x{b:02x}" for b in chunk)
            f.write(f"  {line},\n")
        f.write("};\n")

    print(f"Wrote {out_file}: {n} samples ({n / TARGET_RATE:.2f} s at {TARGET_RATE} Hz)")

if __name__ == "__main__":
    main()
