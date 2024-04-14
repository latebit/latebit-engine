#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE 4096
#define SAMPLE_RATE 44100

// A 0-255 integer
using sid_byte = unsigned char;
using sid_hex = sid_byte;

namespace sid {

// Clamp a float between a minimum and maximum
auto clamp(float value, float min, float max) -> float;

// Retuns the maximum of two integers
auto max(int a, int b) -> int;

};  // namespace sid

#endif
