#include <string>

#include "absl/log/log.h"

#include "base/CurrentThread.h"
#include "base/Singleton.h"
#include "base/Thread.h"

class Test : muduo_rewrite::noncopyable {
public:
    Test() {
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", constructing " << this;
    }

    ~Test() {
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", destructing " << this;
    }

    const std::string name() const {
        return name_;
    }
    void setName(const std::string& name) {
        name_ = name;
    }

private:
    std::string name_;
};

class TrivalTest {};

void threadFunc() {
    LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", " << muduo_rewrite::Singleton<Test>::instance().name();
    muduo_rewrite::Singleton<Test>::instance().setName("only one, changed");
}

int main() {
    muduo_rewrite::Singleton<Test>::instance().setName("only one");
    muduo_rewrite::Thread t1(threadFunc);
    t1.start();
    t1.join();

    LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", " << muduo_rewrite::Singleton<Test>::instance().name();
    
    muduo_rewrite::Singleton<Test>::instance().setName("only one, changed again");
    LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", " << muduo_rewrite::Singleton<Test>::instance().name();
    
    // Forbid to instantiate in stack.
    // muduo_rewrite::Singleton<Test> s;

    // Forbid to instantiate trivially constructible and destructible class.
    // muduo_rewrite::Singleton<TrivalTest>::instance();

    return 0;
}

