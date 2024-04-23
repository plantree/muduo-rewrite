#ifndef MUDUO_REWRITE_BASE_CURRENTTHREAD_H
#define MUDUO_REWRITE_BASE_CURRENTTHREAD_H

#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#include <string>

namespace muduo_rewrite {

namespace CurrentThread {
extern __thread pid_t t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;
void cacheTid();

inline int tid() {
  if (unlikely(t_cachedTid == 0)) {
    cacheTid();
  }
  return t_cachedTid;
}

inline const char* tidString() { return t_tidString; }

inline int tidStringLength() { return t_tidStringLength; }

bool isMainThread();

void sleepUsec(int64_t usec);

std::string stackTrace(bool demangle);
}  // namespace CurrentThread

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_CURRENTTHREAD_H