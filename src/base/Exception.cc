#include "Exception.h"
#include "CurrentThread.h"

namespace muduo_rewrite {

Exception::Exception(std::string what)
    : message_(std::move(what)), stack_(CurrentThread::stackTrace(false)) {}

}  // namespace muduo_rewrite