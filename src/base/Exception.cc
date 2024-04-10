#include "base/Exception.h"
#include "base/CurrentThread.h"

namespace muduo_rewrite {

Exception::Exception(std::string what)
    : message_(std::move(what)), stack_(CurrentThread::stackTrace(false)) {}

}  // namespace muduo_rewrite