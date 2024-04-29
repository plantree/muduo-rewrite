#include "base/AsyncLogging.h"

#include "absl/log/log.h"

#include <chrono>
#include <mutex>

namespace muduo_rewrite {

namespace detail {

std::string FormatNow() {
  std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
  auto t = std::chrono::system_clock::to_time_t(tp);
  std::string str(std::ctime(&t), 24);
  return str;
}

} // namespace detail

AsyncLogging::AsyncLogging() : running_(false), thread_(std::bind(&AsyncLogging::threadFunc, this), "AsyncLogging"), latch_(1), mutex_(), cond_(), currentBuffer_(new Buffer), nextBuffer_(new Buffer), buffers_() {
  currentBuffer_->bzero();
  nextBuffer_->bzero();
  buffers_.reserve(16);
}

void AsyncLogging::append(const char* logline, size_t len) {
  std::lock_guard<std::mutex> guard(mutex_);
  if (currentBuffer_->avail() > len) {
    currentBuffer_->append(logline, len);
  } else {
    buffers_.push_back(std::move(currentBuffer_));

    if (nextBuffer_) {
      currentBuffer_ = std::move(nextBuffer_);
    } else {
      currentBuffer_.reset(new Buffer);
    }

    currentBuffer_->append(logline, len);
    cond_.notify_one();
  }
}

void AsyncLogging::threadFunc() {
  CHECK(running_);
  latch_.countDown();

  BufferPtr newBuffer1(new Buffer);
  BufferPtr newBuffer2(new Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();

  BufferVector buffersToWrite;
  buffersToWrite.reserve(16);

  while (running_) {
    CHECK(newBuffer1 && newBuffer1->length() == 0);
    CHECK(newBuffer2 && newBuffer2->length() == 0);
    CHECK(buffersToWrite.empty());

    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (buffers_.empty()) {
        cond_.wait(lock);
      }

      buffers_.push_back(std::move(currentBuffer_));
      currentBuffer_ = std::move(newBuffer1);
      buffersToWrite.swap(buffers_);

      if (!nextBuffer_) {
        nextBuffer_ = std::move(newBuffer2);
      }
    }

    CHECK(!buffersToWrite.empty());

    if (buffersToWrite.size() > 25) {
       char buf[256];
       snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
        detail::FormatNow().c_str(), buffersToWrite.size() - 2);
        fputs(buf, stderr);
        buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
    }

    for (const auto& buffer : buffersToWrite) {
      fwrite(buffer->data(), 1, buffer->length(), stdout);
    }

    if (buffersToWrite.size() > 2) {
      buffersToWrite.resize(2);
    }

    if (!newBuffer1) {
      CHECK(!buffersToWrite.empty());
      newBuffer1 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer1->reset();
    }

    if (!newBuffer2) {
      CHECK(!buffersToWrite.empty());
      newBuffer2 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer2->reset();
    }

    buffersToWrite.clear();
    fflush(stdout);
  }
  fflush(stdout);
}

} // namespace muduo_rewrite