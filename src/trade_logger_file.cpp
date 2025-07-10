#include "titanium/trade_logger_file.hpp"
#include "titanium/trade_event.hpp"
#include <fstream>
#include <stdexcept>

namespace titanium {

class FileTradeLogger : public TradeLogger {
public:
  FileTradeLogger(const std::string &filename) : file(filename, std::ios::app) {
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open trade log file");
    }
  }

  void log_trade(const TradeEvent &event) override {
    file << event.timestamp << "," << event.buy_order_id << ","
         << event.sell_order_id << "," << event.price << "," << event.quantity
         << "\n";
  }

  void flush() override { file.flush(); }

  ~FileTradeLogger() {
    flush();
    file.close();
  }

private:
  std::ofstream file;
};

std::shared_ptr<TradeLogger> create_file_logger(const std::string &filename) {
  return std::make_shared<FileTradeLogger>(filename);
}

} // namespace titanium
