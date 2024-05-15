cmake_minimum_required(VERSION 3.10)

file(GLOB SRC_FILES *.cc)
list(FILTER SRC_FILES EXCLUDE REGEX ".*_unittest.cc")

add_library(net ${SRC_FILES})

target_link_libraries(net base absl::log)

find_package(Catch2 2 REQUIRED)

add_executable(Endian_unittest Endian_unittest.cc)
target_link_libraries(Endian_unittest Catch2::Catch2WithMain net)

add_executable(SocketsOps_unittest SocketsOps_unittest.cc)
target_link_libraries(SocketsOps_unittest Catch2::Catch2WithMain net)

add_executable(InetAddress_unittest InetAddress_unittest.cc)
target_link_libraries(InetAddress_unittest Catch2::Catch2WithMain net)