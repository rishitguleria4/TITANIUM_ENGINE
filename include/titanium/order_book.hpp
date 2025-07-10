#pragma once

#include "titanium/order.hpp"
#include "titanium/trade_event.hpp"
#include "titanium/trade_logger.hpp"
#include <cstdint>
#include <map>
#include <vector>

namespace titanium {

class OrderBook {
public:
  void add_order(const Order &order);
  void match_order(const Order &order);
  bool cancel_order(uint64_t order_id);
  bool modify_order(uint64_t order_id, uint32_t new_px, uint32_t new_qty);
  void print_book() const;
  void print_trades() const;
  void set_logger(TradeLoggerPtr logger);

private:
  std::map<uint32_t, std::vector<Order>, std::greater<>> bids;
  std::map<uint32_t, std::vector<Order>> asks;
  TradeLoggerPtr logger_;
  std::vector<TradeEvent> trade_log;
};

} // namespace titanium
