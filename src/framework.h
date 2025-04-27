#pragma once

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#ifdef _CONSOLE
 #define WIN32_LEAN_AND_MEAN
#endif _CONSOLE

// To allow std::min and std::max
// instead of MIN/MAX macros
#define NOMINMAX

// Must include this before windows.h for deprecated converters
#include <codecvt>
#include <locale>

// Windows Header Files
// Fail early if _WINDOWS isn't defined, since this is a GUI Win32 app
#ifdef _WINDOWS
 #include <windows.h>
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
#include <tchar.h>

// Always assume wide character support
#ifndef UNICODE
 #define UNICODE
#endif // UNICODE
#ifndef _UNICODE
 #define _UNICODE
#endif // _UNICODE

#endif // FRAMEWORK_H_
