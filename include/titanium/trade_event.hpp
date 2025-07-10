#pragma once
#include <cstdint> // ✅ Required for uint64_t, uint32_t
#include <string>

namespace titanium {

struct TradeEvent {
  uint64_t timestamp;
  uint64_t buy_order_id;
  uint64_t sell_order_id;
  uint32_t price;
  uint32_t quantity;

  std::string to_string() const; // ✅ Declaration only (no body here)
};

} // namespace titanium
