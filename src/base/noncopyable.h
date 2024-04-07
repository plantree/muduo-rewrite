#ifndef MUDUO_REWRITE_BASE_NONCOPYABLE_H
#define MUDUO_REWRITE_BASE_NONCOPYABLE_H

namespace muduo_rewrite {

class noncopyable {
  public:
    noncopyable(const noncopyable&)    = delete;
    void operator=(const noncopyable&) = delete;

  protected:
    noncopyable()  = default;
    ~noncopyable() = default;
};

}  // namespace muduo_rewrite

#endif  // MUDUO_REWRITE_BASE_NONCOPYABLE_H