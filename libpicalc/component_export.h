#ifndef COMPONENT_EXPORT_H_
#define COMPONENT_EXPORT_H_

// Defines we use for marking functions as regular or exportable,
// for shared libraries like .dlls and .so files.

#if defined(COMPONENT_BUILD)
 #if defined(WIN32)
  #define COMPONENT_EXPORT __declspec(dllexport)
 #else // POSIX
  #define COMPONENT_EXPORT __attribute__((visibility("default")))
 #endif // defined(WIN32)
#else
 #if defined(WIN32)
  #define COMPONENT_EXPORT
 #else // POSIX
  #define COMPONENT_EXPORT
 #endif // defined(WIN32)
#endif  // defined(COMPONENT_BUILD)

#endif // COMPONENT_EXPORT_H_
