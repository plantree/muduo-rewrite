#ifndef MUDUO_REWRITE_MUTEX_H
#define MUDUO_REWRITE_MUTEX_H

#include <pthread.h>

#include "absl/log/check.h"

#include "base/noncopyable.h"
#include "base/CurrentThread.h"

namespace muduo_rewrite {

class MutexLock : noncopyable {
public:
    MutexLock() : holder_(0) {
        int ret = pthread_mutex_init(&mutex_, nullptr);
        CHECK(ret == 0);
    }

    ~MutexLock() {
        CHECK(holder_ == 0);
        int ret = pthread_mutex_destroy(&mutex_);
        CHECK(ret == 0);
    }

    // must be called when locked, i.e. for assertion
    bool isLockedByThisThread() const {
        return holder_ == CurrentThread::tid();
    }

    
private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_MUTEX_H