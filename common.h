#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <sstream>
#include <string>

#include "framework.h"
#include "resource.h"

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

std::wstring getVersionString();

#endif // COMMON_H_
