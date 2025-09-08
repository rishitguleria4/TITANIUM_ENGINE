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

# Make sure compiler finds headers under include/titanium/...
target_include_directories(titanium_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Tests / executables
add_executable(trade_replay_test
    test/test_trade_replay.cpp
)

# Link engine into the test executable
target_link_libraries(trade_replay_test PRIVATE titanium_engine)
