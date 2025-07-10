#include "titanium/order_book.hpp"
#include "titanium/trade_logger_file.hpp"
#include <iostream>
int main() {
  titanium::OrderBook book;

  book.set_logger(titanium::create_file_logger("trades.log"));
  // Add some BUY orders
  book.add_order({1, 100001, 10010, 10, titanium::Side::Buy});
  book.add_order({2, 100002, 10010, 5, titanium::Side::Buy});
  book.add_order({3, 100003, 10010, 2, titanium::Side::Buy});

  std::cout << "\nInitial book:\n";
  book.print_book();

  // Cancel an order
  bool result = book.cancel_order(2);
  std::cout << (result ? "Cancel succeeded.\n" : "Cancel failed.\n");

  // Modify an order
  book.modify_order(3, 10030, 15);

  std::cout << "\nAfter cancel + modify:\n";
  book.print_book();

  // Add a SELL order that matches with BUY
  book.match_order({4, 100004, 10010, 8, titanium::Side::Sell});

  std::cout << "\nFinal book:\n";
  book.print_book();

  // Print all trades executed
  book.print_trades();

  return 0;
}
