#include "titanium/order_book.hpp"
#include "titanium/trade_logger.hpp"
#include "titanium/utils/rdtsc_calibration.hpp"
#include "titanium/utils/rdtsc.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>

namespace titanium {

// Lazy one-time calibration using static lambda (no warnings)
static const auto _ = [] {
    titanium::calibrate_rdtsc();
    return 0;
}();

void OrderBook::add_order(const Order &order) {
    if (order.side == Side::Buy) {
        bids[order.price].push_back(order);
        order_lookup[order.order_id] = &bids[order.price].back();
    } else {
        asks[order.price].push_back(order);
        order_lookup[order.order_id] = &asks[order.price].back();
    }
}

void OrderBook::match_order(const Order &incoming) {
    uint32_t remaining_qty = incoming.quantity;

    if (incoming.side == Side::Buy) {
        for (auto ask_it = asks.begin(); ask_it != asks.end() && ask_it->first <= incoming.price;) {
            auto &ask_orders = ask_it->second;

            while (!ask_orders.empty() && remaining_qty > 0) {
                Order &resting = ask_orders.front();
                uint32_t traded_qty = std::min(remaining_qty, resting.quantity);

                uint64_t cycles = titanium::rdtsc();
                uint64_t timestamp_ns = titanium::rdtsc_to_ns(cycles);

                TradeEvent trade{timestamp_ns, incoming.order_id, resting.order_id,
                                 resting.price, traded_qty};
                trade_log.push_back(trade);
                std::cout << trade.to_string() << '\n';

                if (logger_) {
                    logger_->log_trade(trade);
                }

                resting.quantity -= traded_qty;
                remaining_qty -= traded_qty;

                if (resting.quantity == 0) {
                    order_lookup.erase(resting.order_id);
                    ask_orders.erase(ask_orders.begin());
                }
            }

            if (ask_orders.empty())
                ask_it = asks.erase(ask_it);
            else
                ++ask_it;

            if (remaining_qty == 0)
                break;
        }

    } else { // Side::Sell
        for (auto bid_it = bids.begin(); bid_it != bids.end() && bid_it->first >= incoming.price;) {
            auto &bid_orders = bid_it->second;

            while (!bid_orders.empty() && remaining_qty > 0) {
                Order &resting = bid_orders.front();
                uint32_t traded_qty = std::min(remaining_qty, resting.quantity);

                uint64_t cycles = titanium::rdtsc();
                uint64_t timestamp_ns = titanium::rdtsc_to_ns(cycles);

                // Fix: incoming is seller, resting is buyer
                TradeEvent trade{timestamp_ns, incoming.order_id, resting.order_id,
                                 resting.price, traded_qty};
                trade_log.push_back(trade);
                std::cout << trade.to_string() << '\n';

                if (logger_) {
                    logger_->log_trade(trade);
                }

                resting.quantity -= traded_qty;
                remaining_qty -= traded_qty;

                if (resting.quantity == 0) {
                    order_lookup.erase(resting.order_id);
                    bid_orders.erase(bid_orders.begin());
                }
            }

            if (bid_orders.empty())
                bid_it = bids.erase(bid_it);
            else
                ++bid_it;

            if (remaining_qty == 0)
                break;
        }
    }

    if (remaining_qty > 0) {
        Order leftover = incoming;
        leftover.quantity = remaining_qty;
        add_order(leftover);
    }
}

bool OrderBook::cancel_order(uint64_t order_id) {
    auto cancel_in_book = [&](auto &book) -> bool {
        for (auto it = book.begin(); it != book.end(); ++it) {
            auto &orders = it->second;
            auto pos = std::find_if(orders.begin(), orders.end(),
                                    [&](const Order &o) {
                                        return o.order_id == order_id;
                                    });

            if (pos != orders.end()) {
                std::cout << "CANCELLED: Order ID " << order_id << " @ " << it->first << "\n";
                order_lookup.erase(order_id);
                orders.erase(pos);
                if (orders.empty())
                    book.erase(it);
                return true;
            }
        }
        return false;
    };

    return cancel_in_book(bids) || cancel_in_book(asks);
}

bool OrderBook::modify_order(uint64_t order_id, uint32_t new_price, uint32_t new_quantity) {
    Order modified_order{};
    bool found = false;

    auto try_modify = [&](auto &book) -> bool {
        for (auto it = book.begin(); it != book.end(); ++it) {
            auto &orders = it->second;
            for (auto oit = orders.begin(); oit != orders.end(); ++oit) {
                if (oit->order_id == order_id) {
                    modified_order = *oit;
                    order_lookup.erase(oit->order_id);
                    orders.erase(oit);
                    if (orders.empty())
                        book.erase(it);
                    return true;
                }
            }
        }
        return false;
    };

    found = try_modify(bids) || try_modify(asks);
    if (!found)
        return false;

    modified_order.price = new_price;
    modified_order.quantity = new_quantity;

    std::cout << "MODIFIED: Order ID " << order_id << " -> Price: "
              << new_price << ", Quantity: " << new_quantity << "\n";

    add_order(modified_order);
    return true;
}

void OrderBook::print_book() const {
    std::cout << "\nORDER BOOK:\n";

    std::cout << "Asks:\n";
    for (const auto &[price, orders] : asks) {
        for (const auto &o : orders)
            std::cout << "  Sell " << o.quantity << " @ " << price << " (id "
                      << o.order_id << ")\n";
    }

    std::cout << "Bids:\n";
    for (const auto &[price, orders] : bids) {
        for (const auto &o : orders)
            std::cout << "  Buy  " << o.quantity << " @ " << price << " (id "
                      << o.order_id << ")\n";
    }

    std::cout << std::endl;
}

void OrderBook::print_trades() const {
    std::cout << "\nTRADE LOG:\n";
    for (const auto &trade : trade_log) {
        std::cout << trade.to_string() << '\n';
    }
}

size_t OrderBook::get_order_lookup_size() const {
    return order_lookup.size();
}

void OrderBook::set_logger(TradeLoggerPtr logger) {
    logger_ = std::move(logger);
}

} // namespace titanium
