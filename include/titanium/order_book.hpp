#pragma once

#include "titanium/order.hpp"
#include "titanium/trade_event.hpp"
#include "titanium/trade_logger.hpp"
#include <cstdint>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>  // For std::greater<>

namespace titanium {

class OrderBook {
public:
  const std::map<uint32_t, std::vector<Order>, std::greater<>>& get_bids() const {
    return bids;
  }

  const std::map<uint32_t, std::vector<Order>>& get_asks() const {
    return asks;
  }

  void add_order(const Order &order);
  void match_order(const Order &order);
  bool cancel_order(uint64_t order_id);
  bool modify_order(uint64_t order_id, uint32_t new_px, uint32_t new_qty);
  void print_book() const;
  void print_trades() const;
  void set_logger(TradeLoggerPtr logger);
  size_t get_order_lookup_size() const;
  
  const std::vector<TradeEvent>& get_trade_log() const {
    return trade_log;
  }

private:
  std::map<uint32_t, std::vector<Order>, std::greater<>> bids;
  std::map<uint32_t, std::vector<Order>> asks;
  std::unordered_map<uint64_t, Order*> order_lookup;
  std::vector<TradeEvent> trade_log;
  TradeLoggerPtr logger_;
};

} // namespace titanium
