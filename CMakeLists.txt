cmake_minimum_required(VERSION 3.10)
project(rk1210)

set(CMAKE_CXX_STANDARD 11)

# Add source files
add_library(rk1210
    src/rk1210.cpp
    # Add other source files here if needed
)

# Include directories
target_include_directories(rk1210 PRIVATE include)

#ADD TEST
add_executable(rk1210_test src/test.cpp)
target_include_directories(rk1210_test PRIVATE include)
target_link_libraries(rk1210_test rk1210)