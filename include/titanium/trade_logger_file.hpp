#pragma once

#include "titanium/trade_logger.hpp"
#include <memory>
#include <string>

namespace titanium {

// Factory function to create file-based logger
std::shared_ptr<TradeLogger> create_file_logger(const std::string &filename);

} // namespace titanium
