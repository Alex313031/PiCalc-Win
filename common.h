#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <sstream>
#include <string>

#include "framework.h"
#include "resource.h"

// Easier line endings
#ifndef ENDL
 #define ENDL std::endl
#endif // ENDL
#ifndef NL
 // Newline
 #define NL "\n"
 // "wide" newline
 #define WNL L"\n"
#endif // ENDL

// Maximum .res loadstring length
#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING MAX_PATH // Traditionally 255
#endif // MAX_LOADSTRING

// Exit code defines
#ifndef SUCCESS_CODE
 #define SUCCESS_CODE 0
#endif
#ifndef ERROR_CODE
 #define ERROR_CODE 1
#endif

// Return codes
#if !defined(SUCC)
 static constexpr int SUCC = SUCCESS_CODE;
#endif // !defined(SUCC)

#if !defined(FAIL)
 static constexpr int FAIL = ERROR_CODE;
#endif // !defined(FAIL)

// Functions to get main and child window handles from other code
extern HWND getMainHwnd();
extern HWND getChildHwnd();

// Other common functions
namespace common {
std::wstring getVersionString();
};

#endif // COMMON_H_
