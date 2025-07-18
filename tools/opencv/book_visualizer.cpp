#include "book_visualizer.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <iterator>
#include <numeric>

BookVisualizer::BookVisualizer() {
    canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
}

void BookVisualizer::update(const titanium::OrderBook& book) {
    canvas.setTo(cv::Scalar(30, 30, 30)); // clear canvas
    draw_order_book(book);
}

void BookVisualizer::run_loop_once() {
    cv::imshow(window_name, canvas);
    cv::waitKey(30); // ~33 FPS
}

void BookVisualizer::close() {
    cv::destroyWindow(window_name);
}

void BookVisualizer::draw_order_book(const titanium::OrderBook& book) {
    const auto& bids = book.get_bids();
    const auto& asks = book.get_asks();

    int centerY = height / 2;
    int barHeight = 18;
    int maxVisible = 10;

    // Determine max quantity for scaling
    int max_qty = 1;

    for (const auto& [price, orders] : bids)
        for (const auto& o : orders)
            max_qty = std::max(max_qty, static_cast<int>(o.quantity));

    for (const auto& [price, orders] : asks)
        for (const auto& o : orders)
            max_qty = std::max(max_qty, static_cast<int>(o.quantity));

    // Draw Bids
    int y = centerY;
    int shown = 0;
    for (auto it = bids.rbegin(); it != bids.rend() && shown < maxVisible; ++it, ++shown) {
        int total_qty = 0;
        for (const auto& o : it->second)
            total_qty += o.quantity;

        int barWidth = (total_qty * (width - 200)) / max_qty;
        cv::rectangle(canvas, cv::Rect(100, y, barWidth, barHeight),
                      cv::Scalar(0, 255, 0), cv::FILLED);
        cv::putText(canvas,
                    "Bid " + std::to_string(it->first) + " (" + std::to_string(total_qty) + ")",
                    cv::Point(10, y + barHeight - 3),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
        y -= barHeight + 4;
    }

    // Draw Asks
    y = centerY + barHeight + 4;
    shown = 0;
    for (auto it = asks.begin(); it != asks.end() && shown < maxVisible; ++it, ++shown) {
        int total_qty = 0;
        for (const auto& o : it->second)
            total_qty += o.quantity;

        int barWidth = (total_qty * (width - 200)) / max_qty;
        cv::rectangle(canvas, cv::Rect(100, y, barWidth, barHeight),
                      cv::Scalar(0, 0, 255), cv::FILLED);
        cv::putText(canvas,
                    "Ask " + std::to_string(it->first) + " (" + std::to_string(total_qty) + ")",
                    cv::Point(10, y + barHeight - 3),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
        y += barHeight + 4;
    }
}
