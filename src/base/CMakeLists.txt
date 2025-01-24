cmake_minimum_required(VERSION 3.10)

file(GLOB SRC_FILES *.cc)
list(FILTER SRC_FILES EXCLUDE REGEX ".*_unittest.cc")

add_library(base ${SRC_FILES})

target_link_libraries(base pthread stdc++fs)

find_package(Catch2 2 REQUIRED)

# file(GLOB TEST_FILES *_unittest.cc)
# add_executable(base_unittest ${TEST_FILES})

# target_link_libraries(base_unittest base Catch2::Catch2WithMain)

add_executable(Exception_unittest Exception_unittest.cc)
target_link_libraries(Exception_unittest base)

add_executable(Thread_unittest Thread_unittest.cc)
target_link_libraries(Thread_unittest base absl::log)

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

add_executable(Utils_unittest Utils_unittest.cc)
target_link_libraries(Utils_unittest base Catch2::Catch2WithMain absl::log)

add_executable(ProcessInfo_unittest ProcessInfo_unittest.cc)
target_link_libraries(ProcessInfo_unittest base absl::log)

add_executable(BlockingQueue_unittest BlockingQueue_unittest.cc)
target_link_libraries(BlockingQueue_unittest base absl::log)

add_executable(BoundedBlockingQueue_unittest BoundedBlockingQueue_unittest.cc)
target_link_libraries(BoundedBlockingQueue_unittest base absl::log)

add_executable(LogStream_unittest LogStream_unittest.cc)
target_link_libraries(LogStream_unittest base absl::log Catch2::Catch2WithMain)

add_executable(AsyncLogging_unittest AsyncLogging_unittest.cc)
target_link_libraries(AsyncLogging_unittest base absl::log)