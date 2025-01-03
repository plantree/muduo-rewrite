#ifndef MUDUO_REWRITE_BASE_THREADLOCAL_H
#define MUDUO_REWRITE_BASE_THREADLOCAL_H

#include <pthread.h>

#include "absl/log/check.h"

#include "base/noncopyable.h"

namespace muduo_rewrite {

template <typename T>
class ThreadLocal : noncopyable {
 public:
  ThreadLocal() {
    int ret = pthread_key_create(&pkey_, &ThreadLocal::desctructor);
    CHECK(!ret) << "pthread_key_create failed, ret: " << ret;
  }

  ~ThreadLocal() {
    int ret = pthread_key_delete(pkey_);
    CHECK(!ret) << "pthread_key_delete failed, ret: " << ret;
  }

  T& value() {
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
    if (!perThreadValue) {
      T* newObj = new T();
      CHECK(!pthread_setspecific(pkey_, newObj))
          << "pthread_setspecific failed";
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:
  static void desctructor(void* x) {
    T* obj = static_cast<T*>(x);
    delete obj;
  }

  pthread_key_t pkey_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_THREADLOCAL_H