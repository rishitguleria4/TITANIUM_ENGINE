#!/bin/bash

set -e  # Exit immediately on error

# Change to the script directory
cd "$(dirname "$0")"

echo "🧹 Cleaning previous build..."
rm -rf build
rm -f build/trade_replay_test build/book_visualizer_test

echo "📁 Creating fresh build directory..."
mkdir -p build

echo "⚙️ Running CMake configuration..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build

echo "🔗 Linking compile_commands.json for clangd..."
ln -sf build/compile_commands.json compile_commands.json

echo "🔨 Compiling OpenCV trade_replay_test..."
g++ test/test_trade_replay.cpp \
    src/order_book.cpp \
    src/trade_logger_file.cpp \
    src/trade_event.cpp \
    src/utils/rdtsc.cpp \
    src/utils/rdtsc_calibration.cpp \
    tools/opencv/trade_replay.cpp \
    -Iinclude -Itools -Itools/opencv \
    -o build/trade_replay_test `pkg-config --cflags --libs opencv4`

echo "🔨 Compiling OpenCV trade_replay_test..."
g++ test/test_trade_replay.cpp \
    src/order_book.cpp \
    src/trade_logger_file.cpp \
    src/trade_event.cpp \
    src/utils/rdtsc.cpp \
    src/utils/rdtsc_calibration.cpp \
    tools/trade_replay.cpp \
    -Iinclude -Itools -Itools/opencv \
    -o build/trade_replay_test `pkg-config --cflags --libs opencv4`

echo "✅ Build complete."

echo "🚀 Running trade_replay_test..."
./build/trade_replay_test

echo "🚀 Running book_visualizer_test..."
./build/book_visualizer_test
