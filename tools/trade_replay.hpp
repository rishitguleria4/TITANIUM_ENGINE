#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "titanium/trade_event.hpp"
#include <algorithm>
#include <string>

struct TradeReplayTool {
    std::vector<titanium::TradeEvent> trades;
    int window_width = 800;
    int window_height = 600;
    std::string window_name = "Trade Replay";

    void load_trades(const std::vector<titanium::TradeEvent>& trade_log) {
        trades = trade_log;
        std::sort(trades.begin(), trades.end(), [](const auto& a, const auto& b) {
            return a.timestamp < b.timestamp;
        });
    }

    void run_replay(int delay_ms = 300) {
        cv::Mat canvas(window_height, window_width, CV_8UC3, cv::Scalar(30, 30, 30));
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

        for (size_t i = 0; i < trades.size(); ++i) {
            fade_canvas(canvas); // smooth heatmap effect

            draw_trade(canvas, trades[i]);

            draw_progress_bar(canvas, i);

            cv::imshow(window_name, canvas);
            int key = cv::waitKey(delay_ms);

            if (key == 27) break;            // ESC to quit
            else if (key == '+') delay_ms = std::max(10, delay_ms - 50);  // speed up
            else if (key == '-') delay_ms += 50;  // slow down
        }

        cv::destroyWindow(window_name);
    }

private:
    void draw_trade(cv::Mat& canvas, const titanium::TradeEvent& trade) {
        int radius = std::min(trade.quantity * 5, 100u);
        cv::Point center(window_width / 2, window_height / 2);
        cv::Scalar color = (trade.buy_order_id < trade.sell_order_id)
                         ? cv::Scalar(0, 255, 0)   // green for buy-initiated
                         : cv::Scalar(0, 0, 255);  // red for sell-initiated

        cv::circle(canvas, center, radius, color, cv::FILLED);

        std::string label = "Price: " + std::to_string(trade.price) +
                            " | Qty: " + std::to_string(trade.quantity);
        cv::putText(canvas, label, {30, 50}, cv::FONT_HERSHEY_SIMPLEX, 0.8,
                    cv::Scalar(255, 255, 255), 2);
    }

    void draw_progress_bar(cv::Mat& canvas, size_t index) {
        int bar_height = 6;
        int progress = static_cast<int>((index / static_cast<float>(trades.size())) * window_width);
        cv::rectangle(canvas, {0, window_height - bar_height}, {progress, window_height},
                      cv::Scalar(200, 200, 50), cv::FILLED);
    }

    void fade_canvas(cv::Mat& canvas) {
        cv::Mat faded;
        cv::addWeighted(canvas, 0.85, canvas, 0.15, 0.0, faded); // fading effect
        faded.copyTo(canvas);
    }
};
