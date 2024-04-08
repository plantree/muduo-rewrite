#include "base/CurrentThread.h"
#include "base/Exception.h"
#include "base/Thread.h"

#include <cassert>
#include <chrono>
#include <cstdio>
#include <thread>
#include <sys/prctl.h>

namespace muduo_rewrite {

constexpr int kMicroSecondsPerSecond = 1000 * 1000;

namespace detail {

pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork() {
    muduo_rewrite::CurrentThread::t_cachedTid = 0;
    muduo_rewrite::CurrentThread::t_threadName = "main";
    muduo_rewrite::CurrentThread::tid();
}

class ThreadNameInitializer {
public:
    ThreadNameInitializer() {
        muduo_rewrite::CurrentThread::t_threadName = "main";
        muduo_rewrite::CurrentThread::tid();
        pthread_atfork(nullptr, nullptr, &afterFork);
    }
};

ThreadNameInitializer init;

struct ThreadData {
    using ThreadFunc = Thread::ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    int* tid_;

    ThreadData(ThreadFunc func, const std::string& name, int* tid)
        : func_(std::move(func)), name_(name), tid_(tid) {}

    void runInThread() {
        *tid_ = muduo_rewrite::CurrentThread::tid();
        tid_ = nullptr;

        muduo_rewrite::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();

        ::prctl(PR_SET_NAME, muduo_rewrite::CurrentThread::t_threadName);

        try {
            func_();
            muduo_rewrite::CurrentThread::t_threadName = "finished";
        } catch (const Exception& e) {
            muduo_rewrite::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", e.what());
            fprintf(stderr, "stack trace:%s\n", e.stackTrace());
            abort();
        } catch (const std::exception& e) {
            muduo_rewrite::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", e.what());
            abort();
        } catch (...) {
            muduo_rewrite::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
            throw;
        }
    }
};

void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return nullptr;
}

}   // namespace detail

void CurrentThread::cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread() {
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

std::atomic<int32_t> Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& name)
    : started_(false), joined_(false), thread_(), func_(std::move(func)), name_(name) {
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_.detach();
    }
}

void Thread::setDefaultName() {
    int num = numCreated_.fetch_add(1);
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start() {
    assert(!started_);

    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(std::move(func_), name_, &tid_);
    thread_ = std::thread(detail::startThread, data);
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    thread_.join();
    return tid_;
}

}   // namespace muduo_rewrite