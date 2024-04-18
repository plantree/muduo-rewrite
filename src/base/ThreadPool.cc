#include <cstdio>
#include <mutex>

#include "absl/log/check.h"
#include "absl/log/log.h"

#include "base/Exception.h"
#include "base/ThreadPool.h"

namespace muduo_rewrite {

ThreadPool::ThreadPool(const std::string& name)
    : name_(name), maxQueueSize_(0), running_(false), mutex_(), notEmpty_(), notFull_() {}

ThreadPool::~ThreadPool() {
    if (running_) {
        stop();
    }
}

void ThreadPool::start(int numThreads) {
    CHECK(threads_.empty()) << "ThreadPool has no threads";

    running_ = true;
    threads_.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        char id[32];
        snprintf(id, sizeof id, "%d", i + 1);
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::runInThread, this), name_ + id));
        threads_[i]->start();
    }

    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
}

void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lg(mutex_);
        running_ = false;
        notEmpty_.notify_all();
        notFull_.notify_all();
    }

    for (auto& thread : threads_) {
        thread->join();
    }
}

size_t ThreadPool::queueSize() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

void ThreadPool::run(Task f) {
    if (threads_.empty()) {
        f();
    } else {
        std::unique_lock lk(mutex_);
        while (isFull() && running_) {
            notFull_.wait(lk);
        }

        if (!running_) {
            return;
        }

        CHECK(!isFull());

        queue_.push_back(std::move(f));
        notEmpty_.notify_one();
    }
}

ThreadPool::Task ThreadPool::take() {
    std::unique_lock lk(mutex_);
    while (queue_.empty() && running_) {
        notEmpty_.wait(lk);
    }

    Task task;
    if (!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
        if (maxQueueSize_ > 0) {
            notFull_.notify_one();
        }
    }

    return task;
}

bool ThreadPool::isFull() const {
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread() {
    try {
        if (threadInitCallback_) {
            threadInitCallback_();
        }
        while (running_) {
            Task task(take());
            if (task) {
                task();
            }
        }
    } catch (const Exception& ex) {
        LOG(FATAL) << "exception caught in ThreadPool " << name_;
        LOG(FATAL) << "reason: " << ex.what();
        LOG(FATAL) << "stack trace: " << ex.stackTrace();
        abort();
    } catch (const std::exception& ex) {
        LOG(FATAL) << "exception caught in ThreadPool " << name_;
        LOG(FATAL) << "reason: " << ex.what();
        abort();
    } catch (...) {
        LOG(FATAL) << "unknown exception caught in ThreadPool " << name_;
        throw;
    }
}

}  // namespace muduo_rewrite
