#ifndef MUDUO_REWRITE_BASE_THREADPOOL_H
#define MUDUO_REWRITE_BASE_THREADPOOL_H

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

#include "base/Thread.h"
#include "base/noncopyable.h"
#include "base/thread_annotations.h"

namespace muduo_rewrite {

class ThreadPool : noncopyable {
 public:
  using Task = std::function<void()>;

  explicit ThreadPool(const std::string& name = std::string("ThreadPool"));
  ~ThreadPool();

  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb) { threadInitCallback_ = cb; }

  void start(int numThreads);
  void stop();

  const std::string& name() const { return name_; }

  size_t queueSize() const;

  void run(Task f);

 private:
  bool isFull() const;
  void runInThread();
  Task take();

  std::string name_;
  int maxQueueSize_;
  Task threadInitCallback_;
  bool running_;
  std::vector<std::unique_ptr<Thread>> threads_;
  std::deque<Task> queue_ GUARDED_BY(mutex_);

  mutable std::mutex mutex_;
  std::condition_variable notEmpty_ GUARDED_BY(mutex_);
  std::condition_variable notFull_ GUARDED_BY(mutex_);
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_THREADPOOL_H