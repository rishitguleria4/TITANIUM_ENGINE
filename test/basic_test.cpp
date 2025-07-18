#include "titanium/order_book.hpp"
#include "titanium/trade_logger_file.hpp"
#include <iostream>
#include "book_visualizer.hpp"
#include <opencv2/highgui.hpp> // ✅ Add this line

using std::cout;

int main() {
  titanium::OrderBook book;
  BookVisualizer visualizer;

  book.add_order({1, 100001, 10010, 10, titanium::Side::Buy});
  book.add_order({2, 100002, 10005, 15, titanium::Side::Buy});
  book.add_order({3, 100003, 10025, 20, titanium::Side::Sell});
  book.add_order({4, 100004, 10020, 8,  titanium::Side::Sell});

  visualizer.update(book);
  visualizer.run_loop_once();

  std::cout << "Press any key in window to exit...\n";
  cv::waitKey(0);
  return 0;
}
