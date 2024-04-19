#ifndef MUDUO_REWRITE_BASE_COUNTDOWNLATCH_H
#define MUDUO_REWRITE_BASE_COUNTDOWNLATCH_H

#include <condition_variable>
#include <mutex>

#include "base/thread_annotations.h"
#include "base/noncopyable.h"

namespace muduo_rewrite {

class CountDownLatch : noncopyable {
  public:
    explicit CountDownLatch(int count);
    void wait();

    void countDown();
    int  getCount() const;

  private:
    mutable std::mutex                 mutex_;
    std::condition_variable condition_ GUARDED_BY(mutex_);
    int count_                         GUARDED_BY(mutex_);
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_COUNTDOWNLATCH_H