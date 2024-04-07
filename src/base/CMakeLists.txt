cmake_minimum_required(VERSION 3.10)

file(GLOB SRC_FILES *.cc)

add_library(base ${SRC_FILES})

find_package(Catch2 2 REQUIRED)

file(GLOB TEST_FILES *_unittest.cc)
add_executable(base_unittest ${TEST_FILES})

target_link_libraries(base_unittest base Catch2::Catch2WithMain)

add_executable(Exception_unittest Exception_unittest.cc)
target_link_libraries(Exception_unittest base)