#include "base/ThreadPool.h"
#include "base/CurrentThread.h"

#include <unistd.h>
#include "absl/log/log.h"

void print() {
    LOG(INFO) << "tid=" << muduo_rewrite::CurrentThread::tid();
}

void printString(const std::string& str) {
    LOG(INFO) << str;
    usleep(100 * 1000);
}

void test(int maxSize) {
    LOG(INFO) << "Test ThreadPool with max queue size = " << maxSize;
    muduo_rewrite::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    LOG(INFO) << "Adding";
    pool.run(print);
    pool.run(print);

    for (int i = 0; i < 100; ++i) {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }
    LOG(INFO) << "Done";
}

void longTask(int num) {
    LOG(INFO) << "longTask " << num;
    usleep(5000 * 1000);
}

void test2() {
    LOG(INFO) << "Stop early";
    muduo_rewrite::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(5);
    pool.start(3);

    muduo_rewrite::Thread thread1([&pool]() {
        for (int i = 0; i < 20; ++i) {
            pool.run(std::bind(longTask, i));
        }
    }, "thread1");
    thread1.start();

    LOG(INFO) << "stop pool";
    pool.stop();

    thread1.join();
    pool.run(print);
    LOG(INFO) << "test2 Done";
}

int main() {
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);

    test2();
    return 0;
}