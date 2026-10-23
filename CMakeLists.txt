
cmake_minimum_required(VERSION 3.20)

project(
    AirlineOfferEngineBenchmark
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main airline offer example
add_executable(
    airline-engine
    cpp/main.cpp
)

# Performance benchmark
add_executable(
    benchmark-cpp
    cpp/benchmark.cpp
)

# Enable compiler warnings
if(MSVC)
    target_compile_options(airline-engine PRIVATE /W4)
    target_compile_options(benchmark-cpp PRIVATE /W4)
else()
    target_compile_options(airline-engine PRIVATE -Wall -Wextra)
    target_compile_options(benchmark-cpp PRIVATE -Wall -Wextra)
endif()
