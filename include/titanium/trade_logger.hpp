#pragma once

#include "trade_event.hpp"
#include <memory>

namespace titanium {

// Abstract logger for trade events
class TradeLogger {
public:
  virtual ~TradeLogger() = default;

  // Called when a trade occurs
  virtual void log_trade(const TradeEvent &event) = 0;

  // Optional flush or close operation
  virtual void flush() {}
};

// Shared pointer alias for flexibility
using TradeLoggerPtr = std::shared_ptr<TradeLogger>;

} // namespace titanium
