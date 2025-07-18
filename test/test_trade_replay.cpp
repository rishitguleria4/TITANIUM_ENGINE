#include "titanium/order_book.hpp"
#include "trade_replay.hpp"
#include "titanium/trade_logger_file.hpp"

int main() {
    titanium::OrderBook book;

    // Simulate trades
    book.add_order({1, 100001, 10010, 10, titanium::Side::Buy});
    book.add_order({2, 100002, 10010, 5, titanium::Side::Buy});
    book.match_order({3, 100003, 10010, 12, titanium::Side::Sell});

    // Visual replay
    TradeReplayTool replay;
    replay.load_trades(book.get_trade_log());
    replay.run_replay(500);  // 500 ms delay between frames

    return 0;
}


