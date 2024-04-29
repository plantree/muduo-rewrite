#ifndef MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H
#define MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H

// Partly copy from chromium.

#if defined(__clang__)
#define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)  // no-op
#endif

// Documents if a shared field or global variable needs to be protected by a
// mutex. GUARDED_BY() allows the user to specify a particular mutex that
// should be held when accessing the annotated variable.
#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__((guarded_by(x)))

// Turns off thread safety checking within the body of a particular function.
// This annotation is used to mark functions that are known to be correct, but
// the locking behavior is more complicated than the analyzer can handle.
#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE__((no_thread_safety_analysis))

#endif  // MUDUO_REWRITE_BASE_THREAD_ANNOTATIONS_H