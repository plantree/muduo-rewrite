#ifndef MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H
#define MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H

// Partly copy from chromium.

#define GUARDED_BY(x) __attribute__((guarded_by(x)))

#endif  // MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H