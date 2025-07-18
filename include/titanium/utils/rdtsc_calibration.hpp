#pragma once
#include <cstdint>

namespace titanium {

void calibrate_rdtsc();
uint64_t rdtsc_to_ns(uint64_t cycles);

}
