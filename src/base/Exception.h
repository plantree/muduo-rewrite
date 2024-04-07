#ifndef MUDUO_REWRITE_BASE_EXCEPTION_H
#define MUDUO_REWRITE_BASE_EXCEPTION_H

#include <exception>
#include <string>

namespace muduo_rewrite {

class Exception : public std::exception {
  public:
    Exception(std::string what);
    ~Exception() noexcept override = default;

    // default copy-ctor, move-ctor, copy-assignment, move-assignment.

    const char* what() const noexcept override { return message_.c_str(); }

    const char* stackTrace() const noexcept { return stack_.c_str(); }

  private:
    std::string message_;
    std::string stack_;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_EXCEPTION_H