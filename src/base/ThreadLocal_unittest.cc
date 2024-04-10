#include "base/CurrentThread.h"
#include "base/Thread.h"
#include "base/ThreadLocal.h"
#include "base/noncopyable.h"

#include "absl/log/log.h"

class Test : muduo_rewrite::noncopyable {
public:
    Test() {
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", constructing " << this;
    }

    ~Test() {
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", destructing " << this << " name=" << name_;
    }

    const std::string& name() const { return name_; }
    void setName(const std::string& n) { name_ = n; }

private:
    std::string name_ = "name";
};

muduo_rewrite::ThreadLocal<Test> testObj1;
muduo_rewrite::ThreadLocal<Test> testObj2;

void print() {
    LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", obj1 " << testObj1.value().name() << " obj2 " << testObj2.value().name();
}

void threadFunc() {
    print();
    testObj1.value().setName("changed 1");
    testObj2.value().setName("changed 2");
    print();
}

int main() {
    testObj1.value().setName("main one");
    print();
    muduo_rewrite::Thread t1(threadFunc);
    t1.start();
    t1.join();

    testObj2.value().setName("main two");
    print();

    pthread_exit(0);
}