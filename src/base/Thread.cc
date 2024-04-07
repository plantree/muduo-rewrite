#include "base/CurrentThread.h"
#include "base/Exception.h"
#include "base/Thread.h"

#include <sys/prctl.h>

namespace muduo_rewrite {

namespace detail {

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

    void RunInThread() {
        *tid_ = muduo_rewrite::CurrentThread::tid();
        tid_ = nullptr;
        muduo_rewrite::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
        ::prctl(PR_SET_NAME, muduo_rewrite::CurrentThread::t_threadName);
        try {
            func_();
            muduo_rewrite::CurrentThread::t_threadName = "finished";
        } catch (const Exception& e) {
            
        }
    }
};

}   // namespace detail

}   // namespace muduo_rewrite