#ifndef MUDUO_REWRITE_BASE_THREAD_H
#define MUDUO_REWRITE_BASE_THREAD_H

#include <pthread.h>

#include <atomic>
#include <functional>
#include <memory>
#include <string>

#include "base/CountDownLatch.h"
#include "noncopyable.h"

namespace muduo_rewrite {

class Thread : noncopyable {
 public:
  using ThreadFunc = std::function<void()>;

  explicit Thread(ThreadFunc, const std::string& name = std::string());
  ~Thread();

  void start();
  int join();

  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

  static int numCreated() {
    return numCreated_.load(std::memory_order_relaxed);
  }

 private:
  void setDefaultName();

  bool started_;
  bool joined_;
  pthread_t pthreadId_;
  pid_t tid_;
  ThreadFunc func_;
  std::string name_;
  CountDownLatch latch_;

  static std::atomic_int numCreated_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_THREAD_H