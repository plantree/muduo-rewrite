#ifndef MUDUO_REWRITE_LOGSTREAM_H
#define MUDUO_REWRITE_LOGSTREAM_H

#include <cstring>  // memcpy

#include "absl/log/check.h"
#include "base/Utils.h"
#include "base/noncopyable.h"

namespace muduo_rewrite {

namespace detail {

constexpr int kSmallBuffer = 4000;
constexpr int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer : noncopyable {
 public:
  FixedBuffer() : cur_(data_) {}
  ~FixedBuffer() = default;

  void append(const char* buf, size_t len) {
    if (static_cast<size_t>(avail()) > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }

  int length() const { return static_cast<int>(cur_ - data_); }

  char* current() { return cur_; }

  int avail() const { return static_cast<int>(end() - cur_); }

  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }

  void bzero() { memZero(data_, sizeof data_); }

 private:
  const char* end() const { return data_ + sizeof data_; }

  char data_[SIZE];
  char* cur_ = nullptr;
};

}  // namespace detail

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_LOGSTREAM_H