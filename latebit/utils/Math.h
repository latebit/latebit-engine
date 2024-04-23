#pragma once

// Clamp value between min and max
auto clamp(float value, float min, float max) -> float;

// Clamp value between min and max
auto clamp(int value, int min, int max) -> int;

// Return true if floats are equal within tolerance
auto equals(float a, float b, float tolerance = 0.0001f) -> bool;
