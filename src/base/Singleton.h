#ifndef MUDUO_REWROTE_BASE_SINGLETON_H
#define MUDUO_REWROTE_BASE_SINGLETON_H

#include "base/no_destructor.h"
#include "base/noncopyable.h"

namespace muduo_rewrite {

template <typename T>
class Singleton : noncopyable {
  public:
    // Cannot be instantiated in stack.
    Singleton()  = delete;
    ~Singleton() = delete;

    static T& instance() {
        static NoDestructor<T> instance;
        return *instance;
    }
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWROTE_BASE_SINGLETON_H