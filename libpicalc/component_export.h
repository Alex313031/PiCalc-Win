// Defines we use for marking functions as regular or exportable,
// for shared libraries like .dlls and .so files.

#ifndef LIBPICALC_COMPONENT_EXPORT_H_
#define LIBPICALC_COMPONENT_EXPORT_H_

#if defined(COMPONENT_BUILD)
 #if defined(WIN32)
  #if defined(INSIDE_DLL)
   #define COMPONENT_EXPORT __declspec(dllexport)
  #else
   #define COMPONENT_EXPORT __declspec(dllimport)
  #endif
 #else // POSIX
  #define COMPONENT_EXPORT __attribute__((visibility("default")))
 #endif // defined(WIN32)
#else
 #define COMPONENT_EXPORT
#endif  // defined(COMPONENT_BUILD)

#endif // LIBPICALC_COMPONENT_EXPORT_H_
