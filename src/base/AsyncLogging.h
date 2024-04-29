#ifndef MUDUO_REWRITE_BASE_ASYNCLOGGING_H
#define MUDUO_REWRITE_BASE_ASYNCLOGGING_H

// Only designed to output to console.
// Just a toy to learn how to write a logger.
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

#include "base/BlockingQueue.h"
#include "base/BoundedBlockingQueue.h"
#include "base/CountDownLatch.h"
#include "base/LogStream.h"
#include "base/Thread.h"

namespace muduo_rewrite {

class AsyncLogging : noncopyable {
 public:
  AsyncLogging();
  ~AsyncLogging() {
    if (running_) {
      stop();
    }
  }

  void append(const char* logline, size_t len);

  void start() {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop() NO_THREAD_SAFETY_ANALYSIS {
    running_ = false;
    cond_.notify_one();
    thread_.join();
  }

 private:
  void threadFunc();

  using Buffer = detail::FixedBuffer<32>;
  using BufferVector = std::vector<std::unique_ptr<Buffer>>;
  using BufferPtr = BufferVector::value_type;

  std::atomic<bool> running_{false};
  muduo_rewrite::Thread thread_;
  muduo_rewrite::CountDownLatch latch_;
  std::mutex mutex_;
  std::condition_variable cond_ GUARDED_BY(mutex_);
  BufferPtr currentBuffer_ GUARDED_BY(mutex_);
  BufferPtr nextBuffer_ GUARDED_BY(mutex_);
  BufferVector buffers_ GUARDED_BY(mutex_);
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_ASYNCLOGGING_H