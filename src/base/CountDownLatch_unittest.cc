#include "base/CountDownLatch.h"
#include "base/ThreadPool.h"

#include "absl/log/log.h"

void worker(int id, muduo_rewrite::CountDownLatch& latch) {
    LOG(INFO) << "worker " << id << " started";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG(INFO) << "worker " << id << " stopped";
    latch.countDown();
}

void test(int numWorkers) {
    muduo_rewrite::CountDownLatch latch(numWorkers);

    muduo_rewrite::ThreadPool pool("test");
    pool.start(numWorkers);

    for (int i = 0; i < numWorkers; ++i) {
        pool.run(std::bind(worker, i, std::ref(latch)));
    }

    latch.wait();
    LOG(INFO) << "all workers done";
}

int main() {
    test(0);
    test(1);
    test(5);
}