cmake_minimum_required(VERSION 3.10)

file(GLOB SRC_FILES *.cc)
list(FILTER SRC_FILES EXCLUDE REGEX ".*_unittest.cc")

add_library(base ${SRC_FILES})
target_link_libraries(base pthread)

find_package(Catch2 2 REQUIRED)

# file(GLOB TEST_FILES *_unittest.cc)
# add_executable(base_unittest ${TEST_FILES})

# target_link_libraries(base_unittest base Catch2::Catch2WithMain)

add_executable(Exception_unittest Exception_unittest.cc)
target_link_libraries(Exception_unittest base)

add_executable(Thread_unitest Thread_unittest.cc)
target_link_libraries(Thread_unitest base)

add_executable(Check_unittest Check_unittest.cc)
target_link_libraries(Check_unittest base absl::log)

add_executable(ThreadLocal_unittest ThreadLocal_unittest.cc)
target_link_libraries(ThreadLocal_unittest base absl::log)

add_executable(no_destructor_unittest no_destructor_unittest.cc)
target_link_libraries(no_destructor_unittest base absl::log Catch2::Catch2WithMain)

add_executable(Singleton_unittest Singleton_unittest.cc)
target_link_libraries(Singleton_unittest base absl::log)

add_executable(ThreadPool_unittest ThreadPool_unittest.cc)
target_link_libraries(ThreadPool_unittest base absl::log)

add_executable(CountDownLatch_unittest CountDownLatch_unittest.cc)
target_link_libraries(CountDownLatch_unittest base absl::log)
