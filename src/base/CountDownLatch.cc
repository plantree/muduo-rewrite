#include "base/CountDownLatch.h"
#include <mutex>


namespace muduo_rewrite {

CountDownLatch::CountDownLatch(int count)
    : mutex_(),
      condition_(),
      count_(count) {}

void CountDownLatch::wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_ > 0) {
        condition_.wait(lock);
    }
}

void CountDownLatch::countDown() {
    std::lock_guard<std::mutex> guard(mutex_);
    --count_;
    if (count_ == 0) {
        condition_.notify_all();
    }
}

int CountDownLatch::getCount() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return count_;
}

}  // namespace muduo_rewrite