#ifndef MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H
#define MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H

// Partly copy from absl.
// Reference:
// https://github.com/abseil/abseil-cpp/blob/master/absl/base/thread_annotations.h

#if defined(__clang__)
#define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)  // no-op
#endif

// Documents if a shared field or global variable needs to be protected by a
// mutex. GUARDED_BY() allows the user to specify a particular mutex that
// should be held when accessing the annotated variable.
#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__((guarded_by(x)))

#endif  // MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H