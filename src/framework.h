#pragma once

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#ifdef _CONSOLE
 #define WIN32_LEAN_AND_MEAN
#endif // _CONSOLE

// To allow std::min and std::max
// instead of MIN/MAX macros
#ifndef NOMINMAX
 #define NOMINMAX
#endif // NOMINMAX

// Allow old C++ standards and insecure functions
#ifndef _CRT_SECURE_NO_WARNINGS
 #define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

// To use Microsoft specific preprocessor math constant macros
#ifndef _USE_MATH_DEFINES
 #undef _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#ifdef _USE_MATH_DEFINES
 //#include <cmath>
#endif // _USE_MATH_DEFINES

// Must include this before windows.h for deprecated converters
#include <codecvt>
#include <locale>

// Windows Header Files
// Fail early if _WINDOWS isn't defined, since this is a GUI Win32 app
#ifdef _WINDOWS
 #include <windows.h>
#endif // _WINDOWS

// C Runtime Header Files
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// Always assume wide character support
#ifndef UNICODE
 #define UNICODE
#endif // UNICODE
#ifndef _UNICODE
 #define _UNICODE
#endif // _UNICODE

#endif // FRAMEWORK_H_
