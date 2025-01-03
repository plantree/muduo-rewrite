#ifndef MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H
#define MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H

// Partly borrowed from chromium::base::NoDestructor.
// https://chromium.googlesource.com/chromium/src/base/+/master/no_destructor.h

// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <new>
#include <type_traits>
#include <utility>

namespace muduo_rewrite {

// Helper type to create a function-local static variable of type `T` when `T`
// has a non-trivial destructor. Storing a `T` in a `base::NoDestructor<T>` will
// prevent `~T()` from running, even when the variable goes out of scope.
//
// ## Thread safety
//
// Initialisation of function-local static variables is thread-safe since C++11.
// The standard guarantees that:
//
// - function-local static variables will be initialised the first time
//   execution passes through the declaration.
//
// - if another thread's execution concurrently passes through the declaration
//   in the middle of initialisation, that thread will wait for the in-progress
//   initialisation to complete.
template <typename T>
class NoDestructor {
 public:
  static_assert(!(std::is_trivially_constructible_v<T> &&
                  std::is_trivially_destructible_v<T>),
                "T is trivially constructible and destructible; please use a "
                "constinit object of type T directly instead");
  static_assert(
      !std::is_trivially_destructible_v<T>,
      "T is trivially destructible; please use a function-local static "
      "of type T directly instead");

  template <typename... Args>
  explicit NoDestructor(Args&&... args) {
    new (storage_) T(std::forward<Args>(args)...);
  }

  // Allow copy and move construction.
  explicit NoDestructor(const T& x) { new (storage_) T(x); }
  explicit NoDestructor(T&& x) { new (storage_) T(std::move(x)); }

  NoDestructor(const NoDestructor&) = delete;
  NoDestructor& operator=(const NoDestructor&) = delete;

  ~NoDestructor() = default;

  const T& operator*() const { return *get(); }
  T& operator*() { return *get(); }

  const T* operator->() const { return get(); }
  T* operator->() { return get(); }

  const T* get() const { return reinterpret_cast<const T*>(storage_); }
  T* get() { return reinterpret_cast<T*>(storage_); }

 private:
  alignas(T) char storage_[sizeof(T)];
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_NO_DESTRUCTOR_H