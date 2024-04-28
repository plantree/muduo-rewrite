#ifndef MUDUO_REWRITE_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_REWRITE_BASE_BOUNDEDBLOCKINGQUEUE_H

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>

#include "absl/log/check.h"
#include "base/noncopyable.h"
#include "base/thread_annotations.h"

namespace muduo_rewrite {

template <typename T>
class BoundedBlockingQueue : noncopyable {
 public:
  explicit BoundedBlockingQueue(int maxSize)
      : mutex_(), notEmpty_(), notFull_(), queue_(), maxSize_(maxSize) {}

  void put(const T& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.size() >= maxSize_) {
      notFull_.wait(lock);
    }

    CHECK(queue_.size() < maxSize_);
    queue_.push_back(x);
    notEmpty_.notify_one();
  }

  void put(T&& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.size() >= maxSize_) {
      notFull_.wait(lock);
    }

    CHECK(queue_.size() < maxSize_);
    queue_.push_back(std::move(x));
    notEmpty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      notEmpty_.wait(lock);
    }

    CHECK(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    notFull_.notify_one();
    return front;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const { return maxSize_; }

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_ GUARDED_BY(mutex_);
  std::condition_variable notFull_ GUARDED_BY(mutex_);
  std::deque<T> queue_ GUARDED_BY(mutex_);
  size_t maxSize_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_BOUNDEDBLOCKINGQUEUE_H