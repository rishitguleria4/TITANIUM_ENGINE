#include "tools/opencv/book_visualizer.hpp"
#include "titanium/order_book.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <thread>

int main() {
    titanium::OrderBook book;

    // Simulate a few bids and asks
    book.add_order({1, 100001, 10010, 10, titanium::Side::Buy});
    book.add_order({2, 100002, 10015, 5, titanium::Side::Buy});
    book.add_order({3, 100003, 10020, 12, titanium::Side::Sell});
    book.add_order({4, 100004, 10025, 7, titanium::Side::Sell});

    while (true) {
        cv::Mat canvas(600, 800, CV_8UC3, cv::Scalar(30, 30, 30)); // black background
        titanium::viz::draw_order_book(canvas, book);
        cv::imshow("Order Book Visualizer", canvas);

        int key = cv::waitKey(100);
        if (key == 27) break; // ESC to exit

        // Simulate engine state changes here
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
