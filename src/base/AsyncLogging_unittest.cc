#include "base/AsyncLogging.h"
#include "base/CountDownLatch.h"
#include "base/Thread.h"

#include "absl/log/log.h"

#include <memory>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>

muduo_rewrite::AsyncLogging* g_asyncLog = nullptr;

int main() {
  muduo_rewrite::AsyncLogging log;
  g_asyncLog = &log;
  log.start();

  std::vector<std::unique_ptr<muduo_rewrite::Thread>> threads;
  muduo_rewrite::CountDownLatch latch(5);
  for (int i = 0; i < 5; ++i) {
    char buf[32];
    snprintf(buf, sizeof buf, "thread %d", i);
    threads.emplace_back(new muduo_rewrite::Thread([&latch, buf]{
      latch.countDown();
      for (int k = 0; k < 10000; ++k) {
        std::string message = std::string(buf) + ": hello world " + std::to_string(k) + "\n";
        g_asyncLog->append(message.c_str(), message.size());
      }
    }, std::string(buf)));
    threads[i]->start();
  }

  latch.wait();

  for (auto& thr: threads) {
    thr->join();
  }

  g_asyncLog->stop();

}