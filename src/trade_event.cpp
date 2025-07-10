#include "titanium/trade_event.hpp"
#include <sstream>

namespace titanium {

std::string TradeEvent::to_string() const {
  std::ostringstream oss;
  oss << "TRADE [ts=" << timestamp << " ns] "
      << "Qty=" << quantity << " @ " << price << " [BUY:" << buy_order_id
      << " vs SELL:" << sell_order_id << "]";
  return oss.str();
}

} // namespace titanium
