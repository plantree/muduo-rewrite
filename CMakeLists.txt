cmake_minimum_required(VERSION 3.10)

project(muduo-rewrite CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -march=native -rdynamic")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Process Abseil's CMake build system
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/abseil-cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/base)
add_subdirectory(src/net)
