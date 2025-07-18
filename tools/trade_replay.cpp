#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include "titanium/trade_event.hpp"

struct TradeReplayTool {
    int width = 800;
    int height = 600;
    std::vector<titanium::TradeEvent> trades;

    void load_trades(const std::vector<titanium::TradeEvent>& trade_log) {
        trades = trade_log;
        std::sort(trades.begin(), trades.end(), [](const auto& a, const auto& b) {
            return a.timestamp < b.timestamp;
        });
    }

    void run_replay_ns() {
        if (trades.empty()) return;

        cv::namedWindow("Trade Replay", cv::WINDOW_AUTOSIZE);
        uint64_t start_ts = trades.front().timestamp;

        for (size_t i = 0; i < trades.size(); ++i) {
            const auto& trade = trades[i];

            // Wait until the correct timestamp relative to the first
            if (i > 0) {
                uint64_t wait_ns = trades[i].timestamp - trades[i - 1].timestamp;
                std::this_thread::sleep_for(std::chrono::nanoseconds(wait_ns));
            }

            render_trade(trade);
        }

        // Wait after last trade
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    void render_trade(const titanium::TradeEvent& trade) {
        cv::Mat canvas(height, width, CV_8UC3, cv::Scalar(30, 30, 30));

        cv::circle(canvas, cv::Point(width / 2, height / 2), 50,
                   cv::Scalar(0, 100, 255), -1);

        std::string label = "Price: " + std::to_string(trade.price) +
                            " | Qty: " + std::to_string(trade.quantity);
        cv::putText(canvas, label, {50, 80}, cv::FONT_HERSHEY_SIMPLEX,
                    0.9, cv::Scalar(255, 255, 255), 2);

        std::string ts = "TS: " + std::to_string(trade.timestamp) + " ns";
        cv::putText(canvas, ts, {50, 130}, cv::FONT_HERSHEY_SIMPLEX,
                    0.8, cv::Scalar(100, 200, 255), 2);

        cv::imshow("Trade Replay", canvas);
        cv::waitKey(1);  // Process GUI events
    }
};
