#include "titanium/utils/rdtsc.hpp"
#include <cstdint>
namespace titanium {

// 🔒 Store global cycles_per_ns
static double cycles_per_ns = 0.0;

uint64_t rdtsc_to_ns(uint64_t cycles) {
  return static_cast<uint64_t>(cycles / cycles_per_ns);
}

void set_cycles_per_ns(double cpn) {
  cycles_per_ns = cpn;
}

double get_cycles_per_ns() {
  return cycles_per_ns;
}

}
