#ifndef MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H
#define MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H

// Partly borrowed from chromium::base::NoDestructor.
// https://chromium.googlesource.com/chromium/src/base/+/master/no_destructor.h

#include <new>
#include <type_traits>
#include <utility>

namespace muduo_rewrite {

template <typename T>
class NoDestructor {
 public:
  static_assert(!(std::is_trivially_constructible_v<T> &&
                  std::is_trivially_destructible_v<T>),
                "T is trivially constructible and destructible, NoDestructor "
                "is unnecessary");
  static_assert(!std::is_trivially_destructible_v<T>,
                "T is trivially destructible, NoDestructor is unnecessary");

  template <typename... Args>
  explicit NoDestructor(Args&&... args) {
    new (storage_) T(std::forward<Args>(args)...);
  }

  // Allow copy and move construction.
  explicit NoDestructor(const T& x) { new (storage_) T(x); }
  explicit NoDestructor(T&& x) { new (storage_) T(std::move(x)); }

  const T& operator*() const { return *get(); }
  T& operator*() { return *get(); }

  const T* operator->() const { return get(); }
  T* operator->() { return get(); }

  const T* get() const { return reinterpret_cast<const T*>(storage_); }
  T* get() { return reinterpret_cast<T*>(storage_); }

  ~NoDestructor() = default;

 private:
  alignas(T) char storage_[sizeof(T)];
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H