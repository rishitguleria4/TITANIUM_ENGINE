#pragma once
#include <cstdint>
#include <immintrin.h>

namespace titanium {

inline uint64_t rdtsc() {
  return __rdtsc();
}

uint64_t rdtsc_to_ns(uint64_t cycles);
void set_cycles_per_ns(double);
double get_cycles_per_ns(); // Also useful for calibration

}
