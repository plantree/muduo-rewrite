#include "base/ProcessInfo.h"

#include "absl/log/log.h"

int main() {
    LOG(INFO) << "pid = " << muduo_rewrite::ProcessInfo::pid();
    LOG(INFO) << "uid = " << muduo_rewrite::ProcessInfo::uid();
    LOG(INFO) << "euid = " << muduo_rewrite::ProcessInfo::euid();
    LOG(INFO) << "hostname = " << muduo_rewrite::ProcessInfo::hostname();
    LOG(INFO) << "opened files = " << muduo_rewrite::ProcessInfo::openedFiles();
    LOG(INFO) << "threads size = " << muduo_rewrite::ProcessInfo::numThreads();
    LOG(INFO) << "max open files = " << muduo_rewrite::ProcessInfo::maxOpenFiles();
    LOG(INFO) << "status = " << muduo_rewrite::ProcessInfo::procStatus();
}