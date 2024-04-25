#ifndef MUDUO_REWRITE_BASE_BLOCKINGQUEUE_H
#define MUDUO_REWRITE_BASE_BLOCKINGQUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>

#include "absl/log/check.h"
#include "base/noncopyable.h"

namespace muduo_rewrite {

template <typename T>
class BlockingQueue : noncopyable {
 public:
  using queue_type = std::deque<T>;

  BlockingQueue() : mutex_(), notEmpty_(), queue_() {}

  void put(const T& x) {
    std::lock_guard<std::mutex> guard(mutex_);
    queue_.push_back(x);
    notEmpty_.notify_one();
  }

  void put(T&& x) {
    std::lock_guard<std::mutex> guard(mutex_);
    queue_.push_back(std::move(x));
    notEmpty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      notEmpty_.wait(lock);
    }

    CHECK(!queue_.empty()) << "queue_ should not be empty";
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
  }

  queue_type drain() {
    queue_type queue;
    {
      std::lock_guard<std::mutex> guard(mutex_);
      queue = std::move(queue_);
      CHECK(queue_.empty());
    }
    return queue;
  }

  size_t size() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return queue_.size();
  }

  bool empty() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return queue_.empty();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  queue_type queue_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_BLOCKINGQUEUE_H