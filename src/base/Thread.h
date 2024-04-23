#ifndef MUDUO_REWRITE_BASE_THREAD_H
#define MUDUO_REWRITE_BASE_THREAD_H

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "noncopyable.h"

namespace muduo_rewrite {

class Thread : noncopyable {
 public:
  using ThreadFunc = std::function<void()>;

  explicit Thread(ThreadFunc, const std::string& name = std::string());
  ~Thread();

  void start();
  int join();
  pid_t tid() const { return tid_; }

  bool started() const { return started_; }
  const std::string& name() const { return name_; }

  static int numCreated() { return numCreated_; }

 private:
  void setDefaultName();

  pid_t tid_;
  bool started_;
  bool joined_;
  std::thread thread_;
  ThreadFunc func_;
  std::string name_;

  static std::atomic_int numCreated_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_THREAD_H