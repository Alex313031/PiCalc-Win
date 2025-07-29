#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

// We need this to use older Clang versions.
#ifndef _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH
 #define _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH
#endif // _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH

// Exclude rarely-used stuff from Windows headers
#ifdef _CONSOLE
 #define WIN32_LEAN_AND_MEAN
#endif // _CONSOLE

// To allow std::min and std::max
// instead of MIN/MAX macros
#define NOMINMAX

// Must include this before windows.h for deprecated converters
#include <codecvt>
#include <locale>

#include <tchar.h> // For _UNICODE

// Windows Header Files
// Fail early if _WINDOWS isn't defined, since this is a GUI Win32 app
#ifdef _WINDOWS
 #include <windows.h>
 #include <commctrl.h>
 // For Windows 2000/XP
 #include <psapi.h>
#endif // _WINDOWS

// To use Microsoft specific preprocessor math constant macros
#define _USE_MATH_DEFINES
#include <cmath>

// C Runtime Header Files
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

#endif // FRAMEWORK_H_
