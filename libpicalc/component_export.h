#ifndef COMPONENT_EXPORT_H_
#define COMPONENT_EXPORT_H_

#if defined(WIN32)
 #define COMPONENT_EXPORT __declspec(dllexport)
#else
 #define COMPONENT_EXPORT __attribute__((visibility("default")))
#endif

#endif // COMPONENT_EXPORT_H_
