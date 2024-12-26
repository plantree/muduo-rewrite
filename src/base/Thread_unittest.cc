#include <unistd.h>

#include "base/CurrentThread.h"
#include "base/Thread.h"

void mysleep(int seconds) {
    timespec t = {seconds, 0};
    nanosleep(&t, nullptr);
}

void threadFunc() {
    printf("tid=%d\n", muduo_rewrite::CurrentThread::tid());
}

void threadFunc2(int x) {
    printf("tid=%d, x=%d\n", muduo_rewrite::CurrentThread::tid(), x);
}

void threadFunc3() {
    printf("tid=%d\n", muduo_rewrite::CurrentThread::tid());
    mysleep(1);
}

class Foo {
public:
    explicit Foo(double x) : x_(x) {}

    void memberFunc() {
        printf("tid=%d, Foo::x=%f\n", muduo_rewrite::CurrentThread::tid(), x_);
    }

    void memberFunc2(const std::string& text) {
        printf("tid=%d, Foo::x=%f, text=%s\n", muduo_rewrite::CurrentThread::tid(), x_, text.c_str());
    }

private:
    double x_;
};

int main() {
    printf("pid=%d, tid=%d\n", ::getpid(), muduo_rewrite::CurrentThread::tid());

    muduo_rewrite::Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("t1.tid=%d\n", t1.tid());

    muduo_rewrite::Thread t2(std::bind(threadFunc2, 42), "thread for free function with argument");
    t2.start();
    t2.join();
    printf("t2.tid=%d\n", t2.tid());

    Foo foo(87.53);
    muduo_rewrite::Thread t3(std::bind(&Foo::memberFunc, &foo), "thread for member function without argument");
    t3.start();
    t3.join();
    printf("t3.tid=%d\n", t3.tid());

    muduo_rewrite::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("plantree")));
    t4.start();
    t4.join();
    printf("t4.tid=%d\n", t4.tid());

    {
        muduo_rewrite::Thread t5(threadFunc3);
        t5.start();
        printf("thread t5's name: %s\n", t5.name().c_str());
        // t5 may destruct eariler than thread creation.
    }

    mysleep(2);

    {
        muduo_rewrite::Thread t6(threadFunc3);
        t6.start();
        mysleep(2);
        // t6 destruct later than thread creation.
    }

    sleep(2);
    printf("number of created threads %d\n", muduo_rewrite::Thread::numCreated());
}