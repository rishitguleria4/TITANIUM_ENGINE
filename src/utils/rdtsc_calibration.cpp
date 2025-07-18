#include "titanium/utils/rdtsc_calibration.hpp"
#include "titanium/utils/rdtsc.hpp"
#include <chrono>
#include <thread>

namespace titanium {

void calibrate_rdtsc() {
  using namespace std::chrono;

  auto start_ns = high_resolution_clock::now();
  uint64_t start_cycles = rdtsc();

  std::this_thread::sleep_for(milliseconds(100));

  uint64_t end_cycles = rdtsc();
  auto end_ns = high_resolution_clock::now();

  double ns_elapsed = duration_cast<nanoseconds>(end_ns - start_ns).count();
  double cycles_elapsed = static_cast<double>(end_cycles - start_cycles);
  double cycles_per_ns = cycles_elapsed / ns_elapsed;

  set_cycles_per_ns(cycles_per_ns);
}

}
