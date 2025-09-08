#pragma once
#include <cstdint>

#ifdef __x86_64__
#include <x86intrin.h>  // gives us __rdtsc() and __rdtscp()

namespace titanium {

inline uint64_t rdtsc() {
    return __rdtsc();
}

inline uint64_t rdtscp() {
    unsigned int aux;
    return __rdtscp(&aux);
}

// convert cycles to ns (implemented in rdtsc.cpp)
uint64_t rdtsc_to_ns(uint64_t cycles);
void set_cycles_per_ns(double cpn);
double get_cycles_per_ns();

} // namespace titanium

#else
// fallback for non-x86 (WSL ARM, etc.)
#include <chrono>

namespace titanium {
inline uint64_t rdtsc() {
    return std::chrono::steady_clock::now().time_since_epoch().count();
}
inline uint64_t rdtscp() { return rdtsc(); }
uint64_t rdtsc_to_ns(uint64_t cycles);
void set_cycles_per_ns(double cpn);
double get_cycles_per_ns();
} // namespace titanium

#endif
