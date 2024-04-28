#include "base/BoundedBlockingQueue.h"
#include "base/CountDownLatch.h"
#include "base/CurrentThread.h"
#include "base/Thread.h"

#include "absl/log/log.h"

#include <stdio.h>
#include <functional>
#include <string>
#include <vector>

class Test {
public:
    Test(int numThreads) : queue_(20), latch_(numThreads) {
        threads_.reserve(numThreads);
        for (int i = 0; i < numThreads; ++i) {
            char buf[32];
            snprintf(buf, sizeof buf, "hello %d", i);
            threads_.emplace_back(new muduo_rewrite::Thread(
                std::bind(&Test::threadFunc, this), std::string(buf)));
        }
        for (auto& thr: threads_) {
            thr->start();
        }
    }

    void run(int times) {
        LOG(INFO) << "waiting for count down latch";
        latch_.wait();
        LOG(INFO) << "all threads started";

        for (int i = 0; i < times; ++i) {
            char buf[32];
            snprintf(buf, sizeof buf, "hello %d", i);
            queue_.put(buf);
            LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", data = " << buf << ", size = " << queue_.size();
        }
    }

    void joinAll() {
        for (size_t i = 0; i < threads_.size(); ++i) {
            queue_.put("stop");
        }

        for (auto& thr: threads_) {
            thr->join();
        }
    }

private:
    void threadFunc() {
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", " << muduo_rewrite::CurrentThread::name() << " started.";
        latch_.countDown();
        bool running = true;
        while (running) {
            std::string d(queue_.take());
            LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", data = " << d << ", size = " << queue_.size();
            running = (d != "stop");
        }
        LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid() << ", " << muduo_rewrite::CurrentThread::name() << " stopped.";
    }

    muduo_rewrite::BoundedBlockingQueue<std::string> queue_;
    muduo_rewrite::CountDownLatch latch_;
    std::vector<std::unique_ptr<muduo_rewrite::Thread>> threads_;    
};

void testMove() {
    muduo_rewrite::BoundedBlockingQueue<std::string> queue(10);
    queue.put("hello");
    std::string str = queue.take();
    LOG(INFO) << "testMove " << str;
    queue.put("world");
    LOG(INFO) << "testMove " << queue.take();
}

int main() {
    testMove();
    Test t(5);
    t.run(100);
    t.joinAll();

    LOG(INFO) << "number of created threads " << muduo_rewrite::Thread::numCreated();
    return 0;
}
