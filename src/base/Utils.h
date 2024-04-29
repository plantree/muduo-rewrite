#ifndef MUDUO_REWRITE_BASE_UTILS_H
#define MUDUO_REWRITE_BASE_UTILS_H

#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace muduo_rewrite {

inline void memZero(void* p, size_t n) { memset(p, 0, n); }

inline std::string as_string(std::string_view v) {
  return {v.data(), v.size()};
}

std::string readFile(fs::path path);

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_UTILS_H