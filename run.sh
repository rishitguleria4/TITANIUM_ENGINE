#!/bin/bash
set -e  # Exit immediately on error

# Change to the script directory
cd "$(dirname "$0")"

echo "🧹 Cleaning previous build..."
rm -rf build
rm -f compile_commands.json

echo "📁 Creating fresh build directory..."
mkdir -p build

echo "⚙️ Running CMake configuration..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build

echo "🔗 Linking compile_commands.json for clangd..."
ln -sf build/compile_commands.json compile_commands.json

echo "🔨 Building project..."
cmake --build build -j$(nproc)

echo "✅ Build complete."

echo "🚀 Running trade_replay_test..."
./build/trade_replay_test || echo "trade_replay_test not built."

