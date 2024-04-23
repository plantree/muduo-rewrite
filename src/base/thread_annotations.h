#ifndef MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H
#define MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H

// Partly copy from chromium.

#if defined(__clang__)
#define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)  // no-op
#endif

#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__((guarded_by(x)))

#endif  // MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H