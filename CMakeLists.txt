cmake_minimum_required(VERSION 3.20)
project(TITANIUM_ENGINE LANGUAGES CXX)

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto -Wall -Wextra -DNDEBUG")

# Create the engine library
add_library(titanium_engine STATIC
    src/order_book.cpp
    src/trade_event.cpp  
    src/trade_logger_file.cpp 
)
set(SOURCES
    src/order_book.cpp
    src/trade_event.cpp
    src/trade_logger_file.cpp  # <-- Add this
)


# Specify include path for the engine
target_include_directories(titanium_engine PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Create the test binary
add_executable(basic_test
    test/basic_test.cpp
)

# Link test with the engine
target_link_libraries(basic_test PRIVATE titanium_engine)
set(CMAKE_CXX_STANDARD 17)  # ✅ safer than 20 or 23
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_sources(titanium_engine PRIVATE
    src/order_book.cpp
    src/trade_logger_file.cpp   # ✅ Add this
)
