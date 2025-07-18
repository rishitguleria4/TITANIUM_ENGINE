cmake_minimum_required(VERSION 3.14)
project(TitaniumEngine)

set(CMAKE_CXX_STANDARD 17)

# Main engine sources
add_library(titanium_engine
    src/order_book.cpp
    src/trade_event.cpp
    src/trade_logger_file.cpp
    src/utils/rdtsc.cpp
    src/utils/rdtsc_calibration.cpp
)

target_include_directories(titanium_engine PUBLIC include)

# ======================
# ✅ OpenCV Integration
# ======================
find_package(OpenCV REQUIRED)

# Trade replay test
add_executable(trade_replay_test
    test/test_trade_replay.cpp
    tools/trade_replay.cpp
)

target_include_directories(trade_replay_test PRIVATE include tools tools/opencv)
target_link_libraries(trade_replay_test PRIVATE titanium_engine ${OpenCV_LIBS})

# Book visualizer test
add_executable(book_visualizer_test
    test/basic_test.cpp
    tools/opencv/book_visualizer.cpp
)

target_include_directories(book_visualizer_test PRIVATE include tools tools/opencv)
target_link_libraries(book_visualizer_test PRIVATE titanium_engine ${OpenCV_LIBS})
