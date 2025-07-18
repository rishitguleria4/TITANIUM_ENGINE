#pragma once

#include <opencv2/core.hpp>
#include <string>
#include "titanium/order_book.hpp"

class BookVisualizer {
public:
    BookVisualizer();
    void update(const titanium::OrderBook& book);
    void run_loop_once();
    void close();

private:
    void draw_order_book(const titanium::OrderBook& book);

    int width = 800;
    int height = 600;
    std::string window_name = "Order Book Visualizer";
    cv::Mat canvas;
};
