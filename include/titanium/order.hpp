#pragma once

#include <cstdint>

namespace titanium {

enum class Side : uint8_t { Buy = 0, Sell = 1 };

struct Order {
  uint64_t order_id = 0;
  uint64_t timestamp = 0;
  uint32_t price = 0;
  uint32_t quantity = 0;
  Side side = Side::Buy;

  Order() = default;
  Order(uint64_t id, uint64_t ts, uint32_t px, uint32_t qty, Side s)
      : order_id(id), timestamp(ts), price(px), quantity(qty), side(s) {}
};

} // namespace titanium
